// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ENOTIFICATION_HPP_
#define ECORE_ENOTIFICATION_HPP_

#include <memory>
#include <boost/any.hpp>

namespace ecore
{
	class ENotifier;
	class EStructuralFeature;

	class ENotification
	{
	public:

		enum EventType
		{
			CREATE,
			SET,
			UNSET,
			ADD,
			REMOVE,
			ADD_MANY,
			REMOVE_MANY,
			MOVE,
			REMOVING_ADAPTER,
			RESOLVE,
			EVENT_TYPE_COUNT
		};

		template< typename T >
		ENotification(EventType type, const std::shared_ptr<ENotifier>& notifier,
			const std::shared_ptr<EStructuralFeature>& feature,
			const T & oldValue,
			const T & newValue) :
			eventType_(type), notifier_(notifier), feature_(feature),
			oldValue_(oldValue), newValue_(newValue), position_(-1)
		{
		}

		template< typename T >
		ENotification(EventType type, const std::shared_ptr<ENotifier>& notifier,
			const std::shared_ptr<EStructuralFeature>& feature,
			const T & oldValue,
			const T & newValue,
			int position) :
			eventType_(type), notifier_(notifier), feature_(feature),
			oldValue_(oldValue), newValue_(newValue), position_(position)
		{
		}

		virtual ~ENotification()
		{
		}

		EventType getEventType() const
		{
			return eventType_;
		}

		std::shared_ptr<ENotifier> getNotifier() const
		{
			return notifier_;
		}

		std::shared_ptr<EStructuralFeature> getFeature() const
		{
			return feature_;
		}

		const boost::any& getOldValue() const
		{
			return oldValue_;
		}
		const boost::any& getNewValue() const
		{
			return newValue_;
		}

		int getPosition() const
		{
			return position_;
		}

	protected:

		EventType eventType_;
		std::shared_ptr<ENotifier> notifier_;
		std::shared_ptr<EStructuralFeature> feature_;
		boost::any oldValue_;
		boost::any newValue_;
		int position_;
	};
}
#endif
