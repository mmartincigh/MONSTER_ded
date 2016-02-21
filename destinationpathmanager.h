#ifndef DESTINATIONPATHMANAGER_H
#define DESTINATIONPATHMANAGER_H


// Local
#include "pathmanagerbase.h"

class DestinationPathManager : public PathManagerBase
{
    Q_OBJECT

public:
    explicit DestinationPathManager(QObject *parent = NULL);
    ~DestinationPathManager();
};

#endif // DESTINATIONPATHMANAGER_H
