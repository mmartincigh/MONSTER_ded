// Local
#include "securepathmanager.h"

SecurePathManager::SecurePathManager(QObject *parent) :
    PathBase("SCRPM", parent)
{
    this->debug("Secure path manager created");
}

SecurePathManager::~SecurePathManager()
{
    this->debug("Secure path manager disposed of");
}
