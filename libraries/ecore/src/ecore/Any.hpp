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
            {
                rhs.table_->copy( rhs.storage_, this->storage_ );
            }
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

        /// Same effect as this->clear().
        ~Any()
        {
            this->clear();
        }

        /// If not empty, destroys the contained object.
        void clear() noexcept
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
            using RequiresAllocation = std::integral_constant<bool, Safe::value && Fits>;


        struct Table
        {
            // Note: The caller is responssible for doing .vtable = nullptr after destructful operations
            // such as destroy() and/or move().

            /// The type of the object this vtable is for.
            const std::type_info& ( *type )( ) noexcept;

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
        };

        template <typename T>
        struct TablePlacement
        {
            static const std::type_info& type() noexcept
            {
                return typeid( T );
            }

            static void destroy( Storage& storage ) noexcept
            {
                std::reinterpret_cast<T*>( &storage.buffer_ )->~T();
            }

            static void copy( const Storage& src, Storage& dest )
            {
                new ( &dest.buffer_ ) T( std::reinterpret_cast<const T&>( src.buffer_ ) );
            }

            static void move( Storage& src, Storage& dest ) noexcept
            {
                // one of the conditions for using vtable_stack is a nothrow move constructor,
                // so this move constructor will never throw a exception.
                new ( &dest.buffer_ ) T( std::move( std::reinterpret_cast<T&>( src.buffer_ ) ) );
                destroy( src );
            }

            static void swap( Storage& lhs, Storage& rhs ) noexcept
            {
                Storage tmp;
                move( rhs, tmp );
                move( lhs, rhs );
                move( tmp, lhs );
            }
        };

        template <typename T>
        struct TableAllocation
        {
            static const std::type_info& type() noexcept
            {
                return typeid( T );
            }

            static void destroy( Storage& storage ) noexcept
            {
                delete std::reinterpret_cast<T*>( storage.ptr_ );
            }

            static void copy( const Storage& src, Storage& dest )
            {
                dest.ptr_ = new T( *std::reinterpret_cast<const T*>( src.ptr_ ) );
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
        };

        /// Returns the pointer to the Table of the type T.
        template<typename T>
        static Table* getTable()
        {
            using TableType = typename std::conditional<RequiresAllocation<T>::value, TableAllocation<T>, TablePlacement<T>>::type;
            static Table table = {
                TableType::type, TableType::destroy,
                TableType::copy, TableType::move,
                TableType::swap,
            };
            return &table;
        }

    private:
        Storage storage_; // on offset(0) so no padding for align
        Table* table_;
    };



    static const Any NO_VALUE;

}

#endif /* ECORE_ANY_HPP_ */
