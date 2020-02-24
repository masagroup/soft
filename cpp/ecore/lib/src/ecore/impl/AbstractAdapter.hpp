// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMPL_ABSTRACT_ADAPTER_HPP_
#define ECORE_IMPL_ABSTRACT_ADAPTER_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EAdapter.hpp"

namespace ecore::impl
{

    class ECORE_API AbstractAdapter : public EAdapter
    {
    public:
        AbstractAdapter() = default;
        virtual ~AbstractAdapter() = default;

        AbstractAdapter( AbstractAdapter const& ) = delete;
        AbstractAdapter& operator=( AbstractAdapter const& ) = delete;
        
        virtual std::shared_ptr<ENotifier> getTarget() const;

        virtual void setTarget( const std::shared_ptr<ENotifier>& target );

    protected:
        std::weak_ptr<ENotifier> target_;
    };

} // namespace ecore

#endif
