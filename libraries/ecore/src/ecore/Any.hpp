// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ANY_HPP_
#define ECORE_ANY_HPP_

#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <iostream>

namespace ecore
{
    class BadAnyCast : public std::bad_cast
    {
    public:
        const char* what() const noexcept override
        {
            return "bad any cast";
        }
    };

    class Any final
    {
        template<typename T, typename Decayed = std::decay_t<T>>
        using Decay = std::enable_if_t<!std::is_same<Decayed, Any>::value, Decayed>;

        template <typename T>
        using IsValidCast = std::disjunction < std::is_reference<T>, std::is_copy_constructible<T> >;

    public:
        /// Constructs an object of type any with an empty state.
        Any()
            : table_( nullptr )
        {
        }

        /// Constructs an object of type any with an equivalent state as other.
        Any( const Any& rhs )
            : table_( rhs.table_ )
        {
            if( !rhs.empty() )
                rhs.table_->copy( rhs.storage_, this->storage_ );
        }

        /// Constructs an object of type any with a state equivalent to the original state of other.
        /// rhs is left in a valid but otherwise unspecified state.
        Any( Any&& rhs ) noexcept 
            : table_( rhs.table_ )
        {
            if( !rhs.empty() )
            {
                rhs.table_->move( rhs.storage_, this->storage_ );
                rhs.table_ = nullptr;
            }
        }

        /// Constructs an object of type any that contains an object of type T direct-initialized with std::forward<ValueType>(value).
        ///
        /// T shall satisfy the CopyConstructible requirements, otherwise the program is ill-formed.
        /// This is because an `any` may be copy constructed into another `any` at any time, so a copy should always be allowed.
        template <typename ValueType, typename T = Decay<ValueType>, typename TableType = TableType<T>>
        Any( ValueType&& value )
        {
            static_assert( std::is_copy_constructible<T>::value, "T shall satisfy the CopyConstructible requirements." );
            TableType::create( storage_, std::forward<ValueType>( value ) );
            table_ = getTable<T>();
        }

        /// Same effect as this->clear().
        ~Any()
        {
            this->reset();
        }


        /// Has the same effect as any(rhs).swap(*this). No effects if an exception is thrown.
        Any& operator=( const Any& rhs )
        {
            *this = Any( rhs );
            return *this;
        }

        /// Has the same effect as any(std::move(rhs)).swap(*this).
        ///
        /// The state of *this is equivalent to the original state of rhs and rhs is left in a valid
        /// but otherwise unspecified state.
        Any& operator=( Any&& rhs ) noexcept
        {
            if( rhs.empty() )
                reset();
            else if ( this != &rhs )
            {
                reset();
                table_ = rhs.table_;
                table_->move( rhs.storage_, storage_ );
                rhs.table_ = nullptr;
            }
            return *this;
        }

        /// Has the same effect as any(std::forward<ValueType>(value)).swap(*this). No effect if a exception is thrown.
        ///
        /// T shall satisfy the CopyConstructible requirements, otherwise the program is ill-formed.
        /// This is because an `any` may be copy constructed into another `any` at any time, so a copy should always be allowed.
        template<typename ValueType>
        std::enable_if_t<std::is_copy_constructible<Decay<ValueType>>::value, Any&> operator=( ValueType&& rhs )
        {
            *this = Any( std::forward<ValueType>( rhs ) );
            return *this;
        }


        /// If not empty, destroys the contained object.
        void reset() noexcept
        {
            if( !empty() )
            {
                this->table_->destroy( storage_ );
                this->table_ = nullptr;
            }
        }

        /// Returns true if *this has no contained object, otherwise false.
        bool empty() const noexcept
        {
            return this->table_ == nullptr;
        }

        /// If *this has a contained object of type T, typeid(T); otherwise typeid(void).
        const std::type_info& type() const noexcept
        {
            return empty() ? typeid( void ) : this->table_->type();
        }

        /// Exchange the states of *this and rhs.
        void swap( Any& rhs ) noexcept
        {
            if( this->table_ != rhs.table_ )
            {
                Any tmp( std::move( rhs ) );

                // move from *this to rhs.
                rhs.table_ = this->table_;
                if( this->table_ != nullptr )
                {
                    this->table_->move( this->storage_, rhs.storage_ );
                    //this->table_ = nullptr; -- uneeded, see below
                }

                // move from tmp (previously rhs) to *this.
                this->table_ = tmp.table_;
                if( tmp.table_ != nullptr )
                {
                    tmp.table_->move( tmp.storage_, this->storage_ );
                    tmp.table_ = nullptr;
                }
            }
            else // same types
            {
                if( this->table_ != nullptr )
                    this->table_->swap( this->storage_, rhs.storage_ );
            }
        }

        bool operator ==( const Any& rhs ) const
        {
            if( table_ == rhs.table_ )
            {
                if( table_ )
                    return table_->equals( storage_, rhs.storage_ );
                else
                    return true;
            }
            return false;
        }

        bool operator !=( const Any& rhs ) const
        {
            return !operator ==( rhs );
        }

        friend inline std::ostream&operator << ( std::ostream& s, const Any& obj )
        {
            return obj.table_ ? obj.table_->stream_out( s, obj.storage_ ) : s;
        }

    private:
        // Holds either pointer to a heap object or the contained object itself.
        union Storage
        {
            constexpr Storage(): ptr_{ nullptr }
            {
            }

            // Prevent trivial copies of this type, buffer might hold a non-POD.
            Storage( const Storage& ) = delete;
            Storage& operator=( const Storage& ) = delete;

            void* ptr_;
            std::aligned_storage< sizeof( void* ), alignof( void* )>::type buffer_;
        };

        template<typename T, typename Safe = std::is_nothrow_move_constructible<T>,
            bool Fits = ( sizeof( T ) <= sizeof( Storage ) )
            && ( alignof( T ) <= alignof( Storage ) )>
            using IsInternal = std::integral_constant<bool, Safe::value && Fits>;


        struct Table
        {
            // Note: The caller is responssible for doing .vtable = nullptr after destructful operations
            // such as destroy() and/or move().

            /// The type of the object this vtable is for.
            const std::type_info& ( *type )( ) noexcept;

            void* ( *access )( Storage& ) noexcept;

            /// Destroys the object in the union.
            /// The state of the union after this call is unspecified, caller must ensure not to use src anymore.
            void( *destroy )( Storage& ) noexcept;

            /// Copies the **inner** content of the src union into the yet unitialized dest union.
            /// As such, both inner objects will have the same state, but on separate memory locations.
            void( *copy )( const Storage& src, Storage& dest );

            /// Moves the storage from src to the yet unitialized dest union.
            /// The state of src after this call is unspecified, caller must ensure not to use src anymore.
            void( *move )( Storage& src, Storage& dest ) noexcept;

            /// Exchanges the storage between lhs and rhs.
            void( *swap )( Storage& lhs, Storage& rhs ) noexcept;

            bool (* equals )( const Storage& lhs, const Storage& rhs ) noexcept;

            std::ostream& (*stream_out)(std::ostream&, const Storage& lhs);
        };

        template <typename T>
        struct TableInternal
        {
            template<typename Up>
            static void create( Storage& storage, Up&& value )
            {
                void* addr = &storage.buffer_;
                ::new ( addr ) T( std::forward<Up>( value ) );
            }

            template<typename... Args>
            static void create( Storage& storage, Args&&... args )
            {
                void* addr = &storage.buffer_;
                ::new ( addr ) T( std::forward<Args>( args )... );
            }

            static const std::type_info& type() noexcept
            {
                return typeid( T );
            }

            static void* access( Storage& storage ) noexcept
            {
                return &storage.buffer_;
            }

            static void destroy( Storage& storage ) noexcept
            {
                reinterpret_cast<T*>( &storage.buffer_ )->~T();
            }

            static void copy( const Storage& src, Storage& dest )
            {
                new ( &dest.buffer_ ) T( reinterpret_cast<const T&>( src.buffer_ ) );
            }

            static void move( Storage& src, Storage& dest ) noexcept
            {
                // one of the conditions for using vtable_stack is a nothrow move constructor,
                // so this move constructor will never throw a exception.
                new ( &dest.buffer_ ) T( std::move( reinterpret_cast<T&>( src.buffer_ ) ) );
                destroy( src );
            }

            static void swap( Storage& lhs, Storage& rhs ) noexcept
            {
                Storage tmp;
                move( rhs, tmp );
                move( lhs, rhs );
                move( tmp, lhs );
            }

            static bool equals( const Storage& lhs, const Storage& rhs ) noexcept
            {
                const auto& l = *reinterpret_cast<const T*>( &lhs.buffer_ );
                const auto& r = *reinterpret_cast<const T*>( &rhs.buffer_ );
                return l == r;
            }

            static std::ostream& stream_out(std::ostream& s, const Storage& lhs)
            {
                s << *reinterpret_cast<const T*>(&lhs.buffer_);
                return s;
            }
        };

        template <typename T>
        struct TableExternal
        {
            template<typename Up>
            static void create( Storage& storage, Up&& value )
            {
                storage.ptr_ = new T( std::forward<Up>( value ) );
            }
            
            template<typename... Args>
            static void create( Storage& storage, Args&&...args )
            {
                storage.ptr_ = new T( std::forward<Args>( args )... );
            }

            static const std::type_info& type() noexcept
            {
                return typeid( T );
            }

            static void* access( Storage& storage ) noexcept
            {
                return storage.ptr_;
            }

            static void destroy( Storage& storage ) noexcept
            {
                delete reinterpret_cast<T*>( storage.ptr_ );
            }

            static void copy( const Storage& src, Storage& dest )
            {
                dest.ptr_ = new T( *reinterpret_cast<const T*>( src.ptr_ ) );
            }

            static void move( Storage& src, Storage& dest ) noexcept
            {
                dest.ptr_ = src.ptr_;
                src.ptr_ = nullptr;
            }

            static void swap( Storage& lhs, Storage& rhs ) noexcept
            {
                std::swap( lhs.ptr_, rhs.ptr_ );
            }

            static bool equals( const Storage& lhs, const Storage& rhs ) noexcept
            {
                const auto& l = *reinterpret_cast<const T*>( lhs.ptr_ );
                const auto& r = *reinterpret_cast<const T*>( rhs.ptr_ );
                return l == r;
            }

            static std::ostream& stream_out( std::ostream& s, const Storage& lhs )
            {
                s << *reinterpret_cast<const T*>(lhs.ptr_);
                return s;
            }
        };

        template <typename T>
        using TableType = typename std::conditional_t<IsInternal<T>::value, TableInternal<T>, TableExternal<T> >;

        /// Returns the pointer to the Table of the type T.
        template<typename T>
        static Table* getTable()
        {
            using TableType = TableType<T>;
            static Table table = {
                TableType::type,TableType::access,
                TableType::destroy,TableType::copy,
                TableType::move,TableType::swap,
                TableType::equals,TableType::stream_out
            };
            return &table;
        }

    protected:

        template<typename T>
        friend const T* _anyCast( const Any* operand ) noexcept;
        
        template<typename T>
        friend T* _anyCast( Any* operand ) noexcept;

        /// Casts (with no type_info checks) the storage pointer as void*.
        template<typename T>
        T* cast() noexcept
        {
            if( table_ == getTable<T>() )
                return reinterpret_cast<T*>(table_->access( storage_ ));
            return nullptr;
        }
        
    private:
        Storage storage_; // on offset(0) so no padding for align
        Table* table_;
    };

    /// Exchange the states of two @c any objects.
    inline void swap( Any& x, Any& y ) noexcept
    {
        x.swap( y );
    }


    template<typename ValueType>
    inline ValueType anyCast( const Any& any )
    {
        using Up = typename std::remove_reference_t<ValueType>;
        static_assert( Any::IsValidCast<ValueType>(),
                       "Template argument must be a reference or CopyConstructible type" );
        static_assert( std::is_constructible_v<ValueType, const Up&>,
                       "Template argument must be constructible from a const value." );

        auto p = _anyCast<Up>( &any );
        if( p )
            return static_cast<ValueType>( *p );
        throw BadAnyCast();
    }

    template<typename ValueType>
    inline ValueType anyCast( Any& any )
    {
        using Up = typename std::remove_reference_t<ValueType>;
        static_assert( Any::IsValidCast<ValueType>(),
                       "Template argument must be a reference or CopyConstructible type" );
        static_assert( std::is_constructible_v<ValueType, Up&>,
                       "Template argument must be constructible from an lvalue." );

        auto p = _anyCast<Up>( &any );
        if( p )
            return static_cast<ValueType>( *p );
        throw BadAnyCast();
    }

    template<typename ValueType>
    inline ValueType anyCast( Any&& any )
    {
        using Up = typename std::remove_reference_t<ValueType>;
        static_assert( Any::IsValidCast<ValueType>(),
                       "Template argument must be a reference or CopyConstructible type" );
        static_assert( std::is_constructible_v<ValueType, Up>,
                       "Template argument must be constructible from an rvalue." );
        auto p = _anyCast<Up>( &any );
        if( p )
            return static_cast<ValueType>( std::move( *p ) );
        throw BadAnyCast();
    }

    template<typename ValueType>
    inline ValueType* _anyCast( Any* any ) noexcept
    {
        return any ? static_cast<ValueType*>( any->cast<ValueType>() ) : nullptr;
    }

    template<typename ValueType>
    inline const ValueType* _anyCast( const Any* any ) noexcept
    {
        return any ? static_cast<const ValueType*>( const_cast<Any*>(any)->cast<ValueType>() ) : nullptr;
    }

    static const Any NO_VALUE;

}

#endif /* ECORE_ANY_HPP_ */
