// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NOTIFICATION_HPP_
#define ECORE_NOTIFICATION_HPP_

#include "ecore/Exports.hpp"
#include "ecore/impl/AbstractNotification.hpp"

namespace ecore
{
    class EObject;
}

namespace ecore::impl
{

    class ECORE_API Notification : public AbstractNotification
    {
    public:

        Notification( const std::shared_ptr<EObject>& notifier,
                      EventType type,
                      const std::shared_ptr<EStructuralFeature>& feature,
                      const Any& oldValue,
                      const Any& newValue,
                      std::size_t position = NO_INDEX );
            

        Notification( const std::shared_ptr<EObject>& notifier,
                        EventType type,
                        int featureID,
                        const Any& oldValue,
                        const Any& newValue,
                        std::size_t position = NO_INDEX );
       

        virtual ~Notification();
        

        virtual std::shared_ptr<ENotifier> getNotifier() const;
       
        virtual std::shared_ptr<EStructuralFeature> getFeature() const;
       
        virtual int getFeatureID() const;
       
    protected:

        std::shared_ptr<EObject> notifier_;
        std::shared_ptr<EStructuralFeature> feature_;
        int featureID_;
    
    };
}



#endif /* ECORE_NOTIFICATION_HPP_ */
