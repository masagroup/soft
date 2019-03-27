// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_LAZY_HPP_
#define ECORE_LAZY_HPP_

#include <functional>
#include <memory>

namespace ecore::impl
{
    template <typename T , typename Q = T>
    struct Lazy
    {
        Lazy( std::function<Q()> initializer , const T& value = T() )
            : value_(value), init_(value) , initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( const U& other )
        {
            value_ = other;
            return *this;
        }

        operator const T&( ) const
        {
            return get();
        }

        const T& get() const
        {
            if (  value_ == init_ )
                value_ = initializer_();
            return value_;
        }

        void reset()
        {
            value_ = init_;
        }

        const T& value() const
        {
            return value_;
        }
    
    private:
        mutable T value_;
        T init_;
        std::function<Q()> initializer_;
    };

    template <typename T>
    struct Lazy<T ,void>
    {
        Lazy( std::function<void()> initializer, const T& value = T() )
            : value_( value ), init_( value ), initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( const U& other )
        {
            value_ = other;
            return *this;
        }

        operator const T&( ) const
        {
            return get();
        }

        const T& get() const
        {
            if( value_ == init_ )
                initializer_();
            return value_;
        }

        void reset()
        {
            value_ = init_;
        }

        const T& value() const
        {
            return value_;
        }

    private:
        mutable T value_;
        T init_;
        std::function<void()> initializer_;
    };

    template<typename T, typename Q, typename O>
    bool operator==( const O& left, const Lazy<T, Q>& right ) noexcept
    {	
        return ( left == right.value() );
    }

    template<typename T, typename Q , typename O>
    bool operator==( const Lazy<T, Q>& left, const O& right ) noexcept
    {	
        return ( left.value() == right );
    }

    template<typename T, typename Q, typename O>
    bool operator!=( const Lazy<T, Q>& left, const O& right ) noexcept
    {	
        return ( !( left == right ) );
    }

    template<typename T, typename Q, typename O>
    bool operator!=( const O& left, const Lazy<T, Q>& right ) noexcept
    {	
        return ( !( left == right ) );
    }

    template<typename T, typename Q>
    bool operator==( std::nullptr_t, const Lazy<T, Q>& right ) noexcept
    {
        return ( nullptr == right.value() );
    }

    template<typename T, typename Q>
    bool operator==( const Lazy<T, Q>& left, std::nullptr_t ) noexcept
    {
        return ( left.value() == nullptr );
    }

    template<typename T, typename Q>
    bool operator!=( const Lazy<T, Q>& left, std::nullptr_t ) noexcept
    {
        return ( !( left == nullptr ) );
    }

    template<typename T, typename Q>
    bool operator!=( std::nullptr_t, const Lazy<T, Q>& right ) noexcept
    {
        return ( !( nullptr == right ) );
    }

    template <typename T>
    struct Lazy<std::shared_ptr<T>, std::shared_ptr<T>>
    {
       
        Lazy( std::function<std::shared_ptr<T>()> initializer  )
            : initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( const U& other )
        {
            value_ = other;
            return *this;
        }

        template<typename U>
        Lazy& operator =( const U&& other )
        {
            value_ = std::move( other );
            return *this;
        }

        operator bool() const
        {
            return static_cast<bool>(value_);
        }


        T* operator ->() const
        {
            return value_.get();
        }

        operator const std::shared_ptr<T>&() const
        {
            return get();
        }

        const std::shared_ptr<T>& get() const
        {
            if( !value_ )
                value_ = std::move( initializer_() );
            return value_;
        }

        void reset()
        {
            value_.reset();
        }

        const std::shared_ptr<T>& value() const
        {
            return value_;
        }

    private:
        mutable std::shared_ptr<T> value_;
        std::function<std::shared_ptr<T>()> initializer_;
    };

    template <typename T>
    struct Lazy<std::shared_ptr<T>, void>
    {

        Lazy( std::function<void()> initializer )
            : initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( const U& other )
        {
            value_ = other;
            return *this;
        }

        template<typename U>
        Lazy& operator =( const U&& other )
        {
            value_ = std::move( other );
            return *this;
        }

        operator bool() const
        {
            return static_cast<bool>(value_);
        }

        T* operator ->() const
        {
            return value_.get();
        }

        operator const std::shared_ptr<T>&( ) const
        {
            return get();
        }

        const std::shared_ptr<T>& get() const
        {
            initializer_();
            return value_;
        }

        void reset()
        {
            value_.reset();
        }

        const std::shared_ptr<T>& value() const
        {
            return value_;
        }

    private:
        mutable std::shared_ptr<T> value_;
        std::function<void()> initializer_;
    };

    template<typename T, typename Q>
    bool operator==( std::nullptr_t, const Lazy<std::shared_ptr<T>,Q>& right ) noexcept
    {	// test if nullptr == shared_ptr
        return ( nullptr == right.value() );
    }

    template<typename T, typename Q>
    bool operator==( const Lazy<std::shared_ptr<T>, Q>& left, nullptr_t ) noexcept
    {	// test if nullptr == shared_ptr
        return ( left.value() == nullptr );
    }

    template<typename T, typename Q>
    bool operator!=( const Lazy<std::shared_ptr<T>, Q>& left, std::nullptr_t right ) noexcept
    {	// test if shared_ptr != nullptr
        return ( !( left == right ) );
    }

    template<typename T, typename Q>
    bool operator!=( std::nullptr_t left,
                     const Lazy<std::shared_ptr<T>, Q>& right ) noexcept
    {	// test if nullptr != shared_ptr
        return ( !( left == right ) );
    }

    template <typename T>
    struct Lazy<std::unique_ptr<T>, std::unique_ptr<T>>
    {

        Lazy( std::function<std::unique_ptr<T>()> initializer )
            : initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( U& other )
        {
            value_ = other;
            return *this;
        }

        template<typename U>
        Lazy& operator =( U&& other )
        {
            value_ = std::move( other );
            return *this;
        }

        operator const std::unique_ptr<T>&( ) const
        {
            return get();
        }

        const std::unique_ptr<T>& get() const
        {
            if( !value_ )
                value_ = std::move( initializer_() );
            return value_;
        }

        void reset()
        {
            value_.reset();
        }

        const std::unique_ptr<T>& value() const
        {
            return value_;
        }


    private:
        mutable std::unique_ptr<T> value_;
        std::function<std::unique_ptr<T>()> initializer_;
    };

    template <typename T>
    struct Lazy<std::unique_ptr<T>, void>
    {

        Lazy( std::function<void()> initializer )
            : initializer_( initializer )
        {
        }

        template<typename U>
        Lazy& operator =( U& other )
        {
            value_ = other;
            return *this;
        }

        template<typename U>
        Lazy& operator =( U&& other )
        {
            value_ = std::move( other );
            return *this;
        }
        
        operator const std::unique_ptr<T>&( ) const
        {
            return get();
        }

        const std::unique_ptr<T>& get() const
        {
            if ( !value_ )
                initializer_();
            return value_;
        }

        void reset()
        {
            value_.reset();
        }

        const std::unique_ptr<T>& value() const
        {
            return value_;
        }

    private:
        mutable std::unique_ptr<T> value_;
        std::function<void()> initializer_;
    };

    template<typename T, typename Q>
    bool operator==( std::nullptr_t, const Lazy<std::unique_ptr<T>, Q>& right ) noexcept
    {	// test if nullptr == shared_ptr
        return ( nullptr == right.value() );
    }

    template<typename T, typename Q>
    bool operator==( const Lazy<std::unique_ptr<T>, Q>& left, nullptr_t ) noexcept
    {	// test if nullptr == shared_ptr
        return ( left.value() == nullptr );
    }

    template<typename T, typename Q>
    bool operator!=( const Lazy<std::unique_ptr<T>, Q>& left, std::nullptr_t right ) noexcept
    {	// test if shared_ptr != nullptr
        return ( !( left == right ) );
    }

    template<typename T, typename Q>
    bool operator!=( std::nullptr_t left,
                     const Lazy<std::unique_ptr<T>, Q>& right ) noexcept
    {	// test if nullptr != shared_ptr
        return ( !( left == right ) );
    }

}

#endif // ECORE_LAZY_HPP_
