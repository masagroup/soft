// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTELIST_HPP_
#define ECORE_EOBJECTELIST_HPP_

#include "ecore/BasicElist.hpp"

#include <memory>
#include <algorithm>

namespace ecore
{
	class BasicEObject;

	template <typename T, bool containement = false, bool inverse = false, bool opposite = false >
	class EObjectEList : public BasicEList
	{
	public:
		EObjectEList(const std::shared_ptr<BasicEObject>& owner, int featureID )
			: owner_(owner)
			, featureID_(featureID)
			, inverseFeatureID_( -1 )
		{
			std::shared_ptr<EReference> reference = dynamic_pointer_cast<EReference>( owner->getFeature(featureID) );
			if (reference)
			{
				std::shared_ptr<EReference> opposite = reference->getEOpposite();
				if (opposite)
					inverseFeatureID_ = opposite->getFeatureID();
			}
		}

		EObjectEList( const std::shared_ptr<BasicEObject>& owner, int featureID, int inverseFeatureID )
			: owner_( owner )
			, featureID_( featureID )
			, inverseFeatureID_( inverseFeatureID )
		{
		}


		virtual ~EObjectList()
		{
		}

		virtual void addUnique(const T& e)
		{
			if (isNotificationRequired())
			{
				int index = size();
				doAddUnique(object);
				std::shared_ptr< ENotification > notification = createNotification(ENotification::ADD, nullptr, e, index);
				inverse_.inverseAdd(e);
				dispatchNotification(notification);
			}
			else
				inverse_.inverseAdd(e);
		}
		
	
	protected:

		template <typename Q, opposite = false >
		struct Opposite
		{
			Opposite(EObjectEList& list) : list_(list) {}

			inline void inverseAdd(const Q& q) { q->eInverseAdd(list_.owner_, BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_); }

			inline void inverseRemove(const Q& q) { q->eInverseRemove(list_.owner_, BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_); }

			EObjectEList& list_;
		}

		template <typename Q, true >
		struct Opposite
		{
			Opposite(EObjectEList& list) : list_(list) {}

			inline void inverseAdd(const Q& q) { q->eInverseAdd(list_.owner_, list_.inverseFeatureID_); }

			inline void inverseRemove(const Q& q) { q->eInverseRemove(list_.owner_, list_.inverseFeatureID_); }

			EObjectEList& list_;
		}

		template <typename Q, bool inverse = false, bool opposite = false >
		struct Inverse
		{
			inline Inverse(EObjectEList& list) {}

			inline void inverseAdd(const Q& q) {}

			inline void inverseRemove(const Q& q) {}
		};

		template <typename Q, true , bool opposite = false >
		struct Inverse
		{
			inline Inverse(EObjectEList& list) : opposite_(list) {}

			inline void inverseAdd(const Q& q) { opposite_.inverseAdd(q); }

			inline void inverseRemove(const Q& q) { opposite_.inverseRemove(q);  }

			Opposite<Q,opposite> opposite_;
		};

	private :

		bool isNotificationRequired() const {
			return (auto owner = owner_.lock()) ? p->eNotificationRequired() : false;
		}

		std::shared_ptr< ENotification > createNotification(ENotification::EventType eventType, const T& oldValue, const T& newValue, int position) const
		{
			return (auto owner = owner_.lock()) ? std::make_shared<ENotification>(eventType, owner, owner->getFeature(featureID_) , oldValue, newValue, position) : nullptr;
		}

		void dispatchNotification(const std::shared_ptr<ENotification>& notification) const
		{
			if (auto owner = owner_.lock())
				owner->eNotify(notification);
		}

	private:
		std::weak_ptr<BasicEObject> owner_;
		int featureID_;
		int inverseFeatureID_;
		Inverse<T, inverse, opposite> inverse_;
	};

}



#endif /* ECORE_EOBJECTELIST_HPP_ */
