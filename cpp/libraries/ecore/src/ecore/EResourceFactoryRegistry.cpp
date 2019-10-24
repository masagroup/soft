#include "ecore/EResourceFactoryRegistry.hpp"

using namespace ecore;

std::shared_ptr<EResourceFactoryRegistry> EResourceFactoryRegistry::getInstance()
{
    static auto instance = std::make_shared<EResourceFactoryRegistry>();
    return instance;
}
