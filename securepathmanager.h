#ifndef SECUREPATHMANAGER_H
#define SECUREPATHMANAGER_H

// Local
#include "pathbase.h"

class SecurePathManager : public PathBase
{
    Q_OBJECT

public:
    explicit SecurePathManager(QObject *parent = NULL);
    ~SecurePathManager();
};

#endif // SECUREPATHMANAGER_H
