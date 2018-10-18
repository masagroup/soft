// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICEOBJECT_HPP_
#define ECORE_BASICEOBJECT_HPP_

#include "ecore/EObject.hpp"

namespace ecore
{
    class BasicEObject: public virtual ecore::EObject
    {
    protected:
        BasicEObject();

        virtual ~BasicEObject();

        virtual EList<EAdapter*>& eAdapters();

        virtual bool eDeliver();

        virtual void eSetDeliver( bool deliver );

        virtual void eNotify( const ENotification& notification );

        bool eNotificationRequired();

    protected:
        std::shared_ptr< EList<EAdapter*> > eAdapters_;
        bool eDeliver_;
    };

}

#endif /* ECORE_BASICEOBJECT_HPP_ */
