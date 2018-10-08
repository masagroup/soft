// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTELIST_HPP_
#define ECORE_ABSTRACTELIST_HPP_

#include "ecore/EList.hpp"

namespace ecore
{

    template<typename T>
    class AbstractEList: public EList<T>
    {

    public:
        typedef typename std::shared_ptr<T> T_Element;

        virtual ~AbstractEList()
        {
        }

        virtual bool add(const T_Element& e)
        {
            if (isUnique() && contains(e))
                return false;
            else
            {
                addUnique(e);
                return true;
            }
        }

        virtual void addUnique(const T_Element& object) = 0;

        virtual void add(std::size_t pos, const T_Element& e)
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE(index <= size());
            _SCL_SECURE_ALWAYS_VALIDATE(!isUnique() || (isUnique() && !contains(e)));
            addUnique(pos, e);
        }

        virtual void addUnique(std::size_t pos, const T_Element& e) = 0;

        virtual void set(std::size_t pos, const T_Element& e)
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE(index <= size());
            if (isUnique())
            {
                int currentIndex = indexOf(e);
                _SCL_SECURE_ALWAYS_VALIDATE(
                        currentIndex == -1 || currentIndex == pos);
            }
            setUnique(pos, e);
        }

        virtual void setUnique(std::size_t pos, const T_Element& e) = 0;


        virtual bool remove(const T_Element& e)
        {
            std::size_t index = indexOf(e);
            if (index >= 0)
            {
                remove(index);
                return true;
            }
            else
            {
                return false;
            }
        }

    protected:
        
        virtual T_Element primitiveGet(std::size_t pos) const = 0;

        virtual void didSet(std::size_t pos, const T_Element& newObject, const T_Element& oldObject)
        {
            // Do nothing.
        }

        virtual void didAdd(std::size_t pos, const T_Element& newObject)
        {
            // Do nothing.
        }

        virtual void didRemove(std::size_t pos, const T_Element& oldObject)
        {
            // Do nothing.
        }

        virtual void didClear( int size , T_Element oldObjects[] ) {
            for ( int i = 0 ; i <size ; ++i )
                didRemove( i , oldObjects[i] );
        }

        virtual void didMove(std::size_t pos, const T_Element& movedObject, std::size_t oldIndex)
        {
            // Do nothing.
        }

        virtual void didChange()
        {
            // Do nothing.
        }

    };

}

#endif /* ECORE_ABSTRACTELIST_HPP_ */
