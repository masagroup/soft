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

		virtual void add( const T_Element& e ) = 0;

		virtual void add( std::size_t pos, const T_Element& e) = 0;

		virtual const T_Element& get( std::size_t pos ) const = 0;

		virtual T_Element get( std::size_t pos ) = 0;

		virtual void set( std::size_t pos, const T_Element& e) = 0;

		virtual T_Element remove( std::size_t pos ) = 0;

		virtual bool remove( const T_Element& e ) = 0;

		virtual std::size_t size() const = 0;

		virtual void clear() = 0;


	};

}

#endif /* ECORE_ELIST_HPP_ */
