#ifndef SOURCEPATHMANAGER_H
#define SOURCEPATHMANAGER_H

// Local
#include "pathbase.h"

class SourcePathManager : public PathBase
{
    Q_OBJECT

public:
    explicit SourcePathManager(QObject *parent = NULL);
    ~SourcePathManager();
};

#endif // SOURCEPATHMANAGER_H
