// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECOLLECTIONVIEW_HPP_
#define ECORE_ECOLLECTIONVIEW_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/ETreeIterator.hpp"
namespace ecore
{
    template <typename T>
    class ECollectionView
    {
    };

    template <>
    class ECollectionView<std::shared_ptr<EObject>>
    {
    public:
        ECollectionView( const std::shared_ptr<EObject>& eObject )
            : elements_( eObject->eContents() )
        {
        }

        ECollectionView( const std::shared_ptr<const EList<std::shared_ptr<EObject>>>& elements )
            : elements_( elements )
        {
        }

        ETreeIterator<std::shared_ptr<EObject>> begin() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>(
                elements_, []( const std::shared_ptr<EObject>& eObject ) { return eObject->eContents(); } );
        }

        ETreeIterator<std::shared_ptr<EObject>> end() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>();
        }

    private:
        std::shared_ptr<const EList<std::shared_ptr<EObject>>> elements_;
    };
} // namespace ecore

#endif /* ECORE_ECOLLECTIONVIEW_HPP_ */
