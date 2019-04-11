#include "ecore/ext/EPackageExt.hpp"
#include "ecore/ext/EFactoryExt.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"
#include "ecore/impl/AbstractAdapter.hpp"

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

class EPackageExt::Adapter : public AbstractAdapter
{
public:
    Adapter( EPackageExt& ePackage )
        : ePackage_( ePackage )
    {
    }

    virtual void notifyChanged( const std::shared_ptr<ENotification>& notification )
    {
        int eventType = notification->getEventType();
        if (eventType != ENotification::REMOVING_ADAPTER)
        {
            int featureID = notification->getFeatureID();
            if (featureID == EcorePackage::EPACKAGE__ECLASSIFIERS)
            {
                ePackage_.nameToClassifierMap_.reset();
            }
        }
    }

private:
    EPackageExt& ePackage_;
};

EPackageExt::EPackageExt()
    : adapter_( std::make_unique<Adapter>( *this ) )
{
}

void EPackageExt::setThisPtr( const std::shared_ptr<EPackageExt>& thisPtr )
{
    EPackageImpl::setThisPtr( thisPtr );
    eAdapters().add( adapter_.get() );
}

EPackageExt::~EPackageExt()
{
    eAdapters().remove( adapter_.get() );
}

std::shared_ptr<EClassifier> EPackageExt::getEClassifier( const std::string & name )
{
    if (!nameToClassifierMap_)
    {
        nameToClassifierMap_ = std::make_unique< std::unordered_map< std::string, std::shared_ptr<EClassifier>>>();
        for (const auto& eClassifier : *getEClassifiers())
            nameToClassifierMap_->operator[]( eClassifier->getName() ) = eClassifier;
    }
    return nameToClassifierMap_->operator[](name);
}

