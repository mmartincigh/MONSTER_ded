#ifndef SOURCEPATHMANAGER_H
#define SOURCEPATHMANAGER_H

// Local
#include "pathmanagerbase.h"

class SourcePathManager : public PathManagerBase
{
    Q_OBJECT

public:
    explicit SourcePathManager(QObject *parent = NULL);
    ~SourcePathManager();
};

#endif // SOURCEPATHMANAGER_H
