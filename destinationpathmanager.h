#ifndef DESTINATIONPATHMANAGER_H
#define DESTINATIONPATHMANAGER_H


// Local
#include "pathbase.h"

class DestinationPathManager : public PathBase
{
    Q_OBJECT

public:
    explicit DestinationPathManager(QObject *parent = NULL);
    ~DestinationPathManager();
};

#endif // DESTINATIONPATHMANAGER_H
