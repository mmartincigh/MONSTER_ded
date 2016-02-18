// Local
#include "destinationpathmanager.h"

DestinationPathManager::DestinationPathManager(QObject *parent) :
    PathBase("DSTPM", parent)
{
    this->debug("Destination path manager created");
}

DestinationPathManager::~DestinationPathManager()
{
    this->debug("Destination path manager disposed of");
}
