// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EALLCONTENTSVIEW_HPP_
#define ECORE_EALLCONTENTSVIEW_HPP_

#include "ecore/EObject.hpp"
#include "ecore/ETreeIterator.hpp"
namespace ecore
{
    class EAllContentsView
    {
    public:
        EAllContentsView( const std::shared_ptr<EObject>& eObject )
            : eObject_( eObject )
        {

        }

        ETreeIterator< std::shared_ptr<EObject> > begin() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>( eObject_.lock(), []( const std::shared_ptr<EObject>& eObject)
            {
                return eObject->eContents();
            });
        }

        ETreeIterator< std::shared_ptr<EObject> > end() const
        {
            return ETreeIterator<std::shared_ptr<EObject>>();
        }

    private:
        std::weak_ptr<EObject> eObject_;
    };
}

#endif /* ECORE_EALLCONTENTSVIEW_HPP_ */
