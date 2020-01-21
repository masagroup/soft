// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_NOTIFIER_HPP_
#define ECORE_ABSTRACT_NOTIFIER_HPP_

#include <memory>
#include "ecore/EAdapter.hpp"
#include "ecore/impl/ArrayEList.hpp"

namespace ecore::impl
{

    template <typename... I>
    class AbstractNotifier : public I...
    {
    public:
        virtual ~AbstractNotifier() = default;

        virtual EList<EAdapter*>& eAdapters() const
        {
            return *eAdapters_;
        }

        virtual bool eDeliver() const
        {
            return eDeliver_;
        }
        
        virtual void eSetDeliver( bool deliver )
        {
            eDeliver_ = deliver;
        }

        virtual void eNotify( const std::shared_ptr<ENotification>& notification )
        {
            for( auto eAdapter : *eAdapters_ )
                eAdapter->notifyChanged( notification );
        }

        bool eNotificationRequired()
        { 
            return eDeliver_ && eAdapters_->size() > 0;
        }

        void setThisPtr( const std::shared_ptr<AbstractNotifier>& thisPtr )
        {
            thisPtr_ = thisPtr;
        }
        
        std::shared_ptr<AbstractNotifier> getThisPtr() const
        {
            return thisPtr_.lock();
        }

    private:

        class AdapterList : public ArrayEList<EAdapter*>
        {
        public:
            AdapterList( AbstractNotifier& notifier )
                : notifier_( notifier )
            {

            }

        protected:
            
            virtual void didAdd( std::size_t pos, const ValueType& adapter ) override
            {
                std::shared_ptr<ENotifier> notifier = notifier_.thisPtr_.lock();
                const_cast<EAdapter*>( adapter )->setTarget( notifier );
            }

            virtual void didRemove( std::size_t pos, const ValueType& adapter ) override
            {
                const_cast<EAdapter*>( adapter )->setTarget( nullptr );
            }

        private:
            AbstractNotifier& notifier_;
        };

    protected:
        std::weak_ptr<AbstractNotifier> thisPtr_;
        std::unique_ptr<EList<EAdapter*>> eAdapters_{new AdapterList(*this)};
        bool eDeliver_{ true };
    };
}



#endif /* ECORE_ABSTRACT_NOTIFIER_HPP_ */
