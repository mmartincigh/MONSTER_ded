// Local
#include "secureFileManager.h"

SecureFileManager::SecureFileManager(const QStringList &nameFilters, QObject *parent) :
    FileManagerBase(nameFilters, "SCRFM", parent)
{
    this->debug("Secure file manager created");
}

SecureFileManager::~SecureFileManager()
{
    this->debug("Secure file manager disposed of");
}
