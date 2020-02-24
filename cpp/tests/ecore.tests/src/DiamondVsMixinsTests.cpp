#include <boost/test/unit_test.hpp>

#include <memory>
#include <iostream>
#include <chrono>

const int NB_ITERATIONS = 1000000;

#define LOG 1

namespace diamond
{
    class IA
    {
    public:
        virtual ~IA() = default;

        virtual void testA() = 0;
    };

    class IB : public virtual IA
    {
    public:
        virtual ~IB() = default;

        virtual void testB() = 0;
    };

    class IC : public virtual IB
    {
    public:
        virtual ~IC() = default;

        virtual void tesC() = 0;
    };

    class ID : public virtual IC
    {
    public:
        virtual ~ID() = default;

        virtual void testD() = 0;
    };

    class IE
    {
    public:
        virtual ~IE() = default;

        virtual void testE() = 0;
    };

    class IF
    {
    public:
        virtual ~IF() = default;

        virtual void testF() = 0;
    };

    class II : public virtual IB
    {
    public:
        virtual ~II() = default;

        virtual void testI() = 0;
    };

    template <typename... T>
    class A : public virtual T...
    {
    public:
        A() {}
        virtual ~A() {}

        virtual void testA() { a++; }
    private:
        int a{ 0 };
    };

    class B : public virtual A<II>
    {
    public:
        B() {}
        virtual ~B() {}

        virtual void testB() { b++; }

        virtual void testI() {}
    private:
        int b{ 0 };
    };


    class C : public virtual B, public virtual IC
    {
    public:
        C() {}
        virtual ~C() {}

        virtual void tesC() { c++; };
    private:
        int c{ 0 };
    };

    class CExt : public virtual C {
    public:
        CExt() {}
        virtual ~CExt() {}

        virtual void tesC() { ce++; };
    private:
        int ce{ 0 };
    };

    class D : public virtual CExt, public virtual ID
    {
    public:
        D() {}
        virtual ~D() {}

        virtual void testD() { d++; }
    private:
        int d{ 0 };
    };


    class E : public virtual D, public virtual IE
    {
    public:
        E() {}
        virtual ~E() {}

        virtual void testE() { e++; }
    private:
        int e{ 0 };
    };

    class F : public virtual E, public virtual IF
    {
    public:
        F() {}
        virtual ~F() {}

        virtual void testF() { f++; }
    private:
        int f{ 0 };
    };

    class FExt : public virtual F
    {
    public:
        FExt() {}
        virtual ~FExt() {}

        virtual void testF() { fe++; }
    private:
        int fe{ 0 };
    };
}

namespace mixins
{
    class IA
    {
    public:
        virtual ~IA() = default;

        virtual void testA() = 0;
    };

    class IB : public IA
    {
    public:
        virtual ~IB() = default;

        virtual void testB() = 0;
    };

    class II : public  IB
    {
    public:
        virtual ~II() = default;

        virtual void testI() = 0;
    };

    class IC : public II
    {
    public:
        virtual ~IC() = default;

        virtual void tesC() {};
    };

    class ID : public IC
    {
    public:
        virtual ~ID() = default;

        virtual void testD() {};
    };

    class IE : public ID
    {
    public:
        virtual ~IE() = default;

        virtual void testE() = 0;
    };

    class IF : public IE
    {
    public:
        virtual ~IF() = default;

        virtual void testF() = 0;
    };

  

    template<typename... T>
    class AbstractA : public T... {
    public:
        AbstractA() {}

        virtual void testA() { a++; }
    private:
        int a{ 0 };
    };

    typedef AbstractA<IA> A;

    template<typename... T>
    class AbstractB : public AbstractA<T...> {
    public:
        AbstractB(){}
        virtual ~AbstractB() {}

        virtual void testB() { b++; }

        virtual void testI(){}
    private:
        int b{ 0 };
    };

    typedef AbstractB<II> B;

    template<typename... T>
    class AbstractC : public AbstractB<T...> {
    public:
        AbstractC() {}
        virtual ~AbstractC() {}

        virtual void testC() { c++; }
    private:
        int c{ 0 };
    };

    typedef AbstractC<IC> C;

    template<typename... T>
    class AbstractCExt : public AbstractC<T...> {
    public:
        AbstractCExt() {}
        virtual ~AbstractCExt() {}

        virtual void testC() { ce++; }
    private:
        int ce{ 0 };
    };

    typedef AbstractCExt<IC> CExt;
    

    template<typename... T>
    class AbstractD : public AbstractCExt<T...> {
    public:
        AbstractD() {}
        virtual ~AbstractD() {}

        virtual void testD() { d++; }
    private:
        int d{ 0 };
    };

    typedef AbstractD<ID> D;
    
    template<typename... T>
    class AbstractE : public AbstractD<T...>
    {
    public:
        AbstractE() {}
        virtual ~AbstractE() {}

        virtual void testE() { e++; }
    private:
        int e{ 0 };
    };

    typedef AbstractE<IE> E;

    template<typename... T>
    class AbstractF : public AbstractE<T...>
    {
    public:
        AbstractF() {}
        virtual ~AbstractF() {}

        virtual void testF() { f++; }
    private:
        int f{ 0 };
    };

    typedef AbstractF<IF> F;

    template<typename... T>
    class AbstractFExt : public AbstractF<T...>
    {
    public:
        AbstractFExt() {}
        virtual ~AbstractFExt() {}

        virtual void testF() { fe++;  }
    private:
        int fe{ 0 };
    };

    typedef AbstractFExt<IF> FExt;

}

BOOST_AUTO_TEST_SUITE( DiamondVsMixinsTests, *boost::unit_test::disabled() )

BOOST_AUTO_TEST_CASE( Performance )
{
    long long diamondTimes, mixinTimes;
    {
        auto start = std::chrono::steady_clock::now();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto f = std::dynamic_pointer_cast<diamond::IF>(a);
            BOOST_CHECK( f );
            f->testF();
        }
        auto end = std::chrono::steady_clock::now();
        diamondTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Diamond:" << (double)diamondTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto f = std::static_pointer_cast<mixins::IF>(a);
            BOOST_CHECK( f );
            f->testF();
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        mixinTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Mixins:" << (double)mixinTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    BOOST_CHECK_GE( diamondTimes, mixinTimes );
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto f = std::dynamic_pointer_cast<diamond::FExt>(a);
            BOOST_CHECK( f );
            f->testF();
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        diamondTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Diamond:" << (double)diamondTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto f = std::static_pointer_cast<mixins::FExt>(a);
            BOOST_CHECK( f );
            f->testF();
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        mixinTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Mixins:" << (double)mixinTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    BOOST_CHECK_GE( diamondTimes, mixinTimes );
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto id = std::dynamic_pointer_cast<diamond::ID>(a);
            BOOST_CHECK( id );
            id->testD();
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        diamondTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Diamond:" << (double)diamondTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::FExt>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto id = std::static_pointer_cast<mixins::ID>(a);
            BOOST_CHECK( id );
            id->testD();
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        mixinTimes = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
#if LOG
        std::cout << "Mixins:" << (double)mixinTimes / NB_ITERATIONS << " us" << std::endl;
#endif
    }
    BOOST_CHECK_GE( diamondTimes, mixinTimes );

    std::cout << "Size:" << sizeof(diamond::B) << " " << sizeof(mixins::B) << std::endl;
    std::cout << "Size:" << sizeof(diamond::D) << " " << sizeof(mixins::D) << std::endl;
    std::cout << "Size:" << sizeof(diamond::FExt) << " " << sizeof(mixins::FExt) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()

