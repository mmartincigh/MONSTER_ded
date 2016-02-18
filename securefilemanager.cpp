// Local
#include "secureFileManager.h"

SecureFileManager::SecureFileManager(const QStringList &nameFilters, QObject *parent) :
    FileBase(nameFilters, "SCRFM", parent)
{
    this->debug("Secure file manager created");
}

SecureFileManager::~SecureFileManager()
{
    this->debug("Secure file manager disposed of");
}
