#ifndef SECUREFILEMANAGER_H
#define SECUREFILEMANAGER_H

// Local
#include "filemanagerbase.h"

class SecureFileManager : public FileManagerBase
{
    Q_OBJECT

public:
    explicit SecureFileManager(const QStringList &nameFilters, QObject *parent = NULL);
    ~SecureFileManager();
};

#endif // SECUREFILEMANAGER_H
