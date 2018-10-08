// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#define ECORE_ELIST_HPP_

#include <memory>
#include <vector>

namespace ecore {

	template <typename T>
	class EList {

	public:
		typedef typename std::shared_ptr<T> T_Element;

		virtual ~EList() {}

		virtual bool add( const T_Element& e ) = 0;

		virtual void add( std::size_t pos, const T_Element& e) = 0;

		// virtual bool addAll( const EList<T>& o ) = 0;

		// virtual bool addAll( std::size_t pos, const EList<T>& o) = 0;

		virtual const T_Element& get( std::size_t pos ) const = 0;

		virtual T_Element get( std::size_t pos ) = 0;

		virtual void set( std::size_t pos, const T_Element& e) = 0;

		virtual T_Element remove( std::size_t pos ) = 0;

		virtual bool remove( const T_Element& e ) = 0;

		virtual std::size_t size() const = 0;

		virtual void clear() = 0;

		virtual bool contains(const T_Element& e) const = 0;

		virtual std::size_t indexOf(const T_Element& e, std::size_t from = 0) const = 0;

        typedef typename std::vector<T_Element>::iterator iterator;

        typedef typename std::vector<T_Element>::const_iterator const_iterator;

        virtual iterator begin() = 0;

        virtual const_iterator begin() const = 0;

        virtual iterator end() = 0;

        virtual const_iterator end() const = 0;


	};

}

#endif /* ECORE_ELIST_HPP_ */
