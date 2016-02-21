#ifndef SECUREPATHMANAGER_H
#define SECUREPATHMANAGER_H

// Local
#include "pathmanagerbase.h"

class SecurePathManager : public PathManagerBase
{
    Q_OBJECT

public:
    explicit SecurePathManager(QObject *parent = NULL);
    ~SecurePathManager();
};

#endif // SECUREPATHMANAGER_H
