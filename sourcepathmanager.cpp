// Local
#include "sourcepathmanager.h"

SourcePathManager::SourcePathManager(QObject *parent) :
    PathManagerBase("SRCPM", parent)
{
    this->debug("Source path manager created");
}

SourcePathManager::~SourcePathManager()
{
    this->debug("Source path manager disposed of");
}
