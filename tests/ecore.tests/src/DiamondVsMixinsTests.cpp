#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/timer/timer.hpp>
#include <memory>
#include <iostream>

using namespace boost::timer;

const int NB_ITERATIONS = 1000000;

#define LOG 0

namespace diamond
{
    class IA
    {
    public:
        virtual ~IA() = default;

        virtual void testA() = 0;
    };

    class A : public virtual IA
    {
    public:
        A() {}
        virtual ~A() {}

        virtual void testA() {}
    };

    class IB : public virtual IA
    {
    public:
        virtual ~IB() = default;

        virtual void testB() = 0;
    };

    class B : public virtual A , public virtual IB
    {
    public:
        B() {}
        virtual ~B() {}

        virtual void testB() {}
    };

    class IC : public virtual IB
    {
    public:
        virtual ~IC() = default;

        virtual void tesC() = 0;
    };

    class C : public virtual B, public virtual IC
    {
    public:
        C() {}
        virtual ~C() {}

        virtual void tesC() {};
    };

    class ID : public virtual IC
    {
    public:
        virtual ~ID() = default;

        virtual void testD() = 0;
    };

    class D : public virtual C, public virtual ID
    {
    public:
        D() {}
        virtual ~D() {}

        virtual void testD() {}
    };

    class IE
    {
    public:
        virtual ~IE() = default;

        virtual void testE() = 0;
    };

    class E : public virtual D, public virtual IE
    {
    public:
        E() {}
        virtual ~E() {}

        virtual void testE() {}
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

    template<typename... T>
    class AbstractA : public T... {
    public:
        AbstractA() {}

        virtual void testA() {}
    };

    typedef AbstractA<IA> A;

    class IB : public IA
    {
    public:
        virtual ~IB() = default;

        virtual void testB() = 0;
    };

    template<typename... T>
    class AbstractB : public AbstractA<T...> {
    public:
        AbstractB(){}
        virtual ~AbstractB() {}

        virtual void testB() {}
    };

    typedef AbstractB<IB> B;

    class IC : public IB
    {
    public:
        virtual ~IC() = default;

        virtual void tesC() {};
    };

    template<typename... T>
    class AbstractC : public AbstractB<T...> {
    public:
        AbstractC() {}
        virtual ~AbstractC() {}

        virtual void testC() {}
    };

    typedef AbstractC<IC> C;

    class ID : public IC
    {
    public:
        virtual ~ID() = default;

        virtual void testD() {};
    };

    template<typename... T>
    class AbstractD : public AbstractC<T...> {
    public:
        AbstractD() {}
        virtual ~AbstractD() {}

        virtual void testD() {}
    };

    typedef AbstractD<ID> D;

    class IE
    {
    public:
        virtual ~IE() = default;

        virtual void testE() = 0;
    };

    class E : public AbstractD<ID, IE>
    {
    public:
        E() {}
        virtual ~E() {}

        virtual void testE() {}
    };

}

BOOST_AUTO_TEST_SUITE( DiamondVsMixinsTests )

BOOST_AUTO_TEST_CASE( Performance )
{
#ifdef BENCHMARK
    cpu_timer timer;
    cpu_times diamondTimes , mixinTimes;
    {
        timer.start();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::D>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto d = std::dynamic_pointer_cast<diamond::ID>(a);
            BOOST_CHECK( d );
        }
        diamondTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( diamondTimes );
#endif
    }
    {
        timer.start();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::D>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto d = std::static_pointer_cast<mixins::ID>(a);
            BOOST_CHECK( d );
        }
        mixinTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( mixinTimes );
#endif
    }
    BOOST_CHECK_GE( diamondTimes.user, mixinTimes.user );
    {
        timer.start();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::D>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto d = std::dynamic_pointer_cast<diamond::D>(a);
            BOOST_CHECK( d );
        }
        diamondTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( diamondTimes );
#endif
    }
    {
        timer.start();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::D>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto d = std::static_pointer_cast<mixins::D>(a);
            BOOST_CHECK( d );
        }
        mixinTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( mixinTimes );
#endif
    }
    BOOST_CHECK_GE( diamondTimes.user, mixinTimes.user );
    {
        timer.start();
        std::shared_ptr<diamond::IA> a = std::make_shared<diamond::E>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto ie = std::dynamic_pointer_cast<diamond::IE>(a);
            BOOST_CHECK( ie );
        }
        diamondTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( diamondTimes );
#endif
    }
    {
        timer.start();
        std::shared_ptr<mixins::IA> a = std::make_shared<mixins::E>();
        for (int i = 0; i < NB_ITERATIONS; ++i)
        {
            auto e = std::static_pointer_cast<mixins::E>(a);
            auto ie = std::static_pointer_cast<mixins::IE>(e);
            BOOST_CHECK( ie );
        }
        mixinTimes = timer.elapsed();
        timer.stop();
#if LOG
        std::cout << format( mixinTimes );
#endif
    }
    BOOST_CHECK_GE( diamondTimes.user, mixinTimes.user );

#endif
}

BOOST_AUTO_TEST_SUITE_END()

