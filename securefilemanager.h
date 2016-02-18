#ifndef SECUREFILEMANAGER_H
#define SECUREFILEMANAGER_H

// Local
#include "filebase.h"

class SecureFileManager : public FileBase
{
    Q_OBJECT

public:
    explicit SecureFileManager(const QStringList &nameFilters, QObject *parent = NULL);
    ~SecureFileManager();
};

#endif // SECUREFILEMANAGER_H
