// Local
#include "sourcefilemanager.h"

SourceFileManager::SourceFileManager(const QStringList &nameFilters, QObject *parent) :
    FileBase(nameFilters, "FM", parent)
{
    this->debug("File manager created");
}

SourceFileManager::~SourceFileManager()
{
    this->debug("File manager disposed of");
}
