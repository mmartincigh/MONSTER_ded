#ifndef SOURCEFILEMANAGER_H
#define SOURCEFILEMANAGER_H

// Local
#include "filemanagerbase.h"

class SourceFileManager : public FileManagerBase
{
    Q_OBJECT

public:
    explicit SourceFileManager(const QStringList &nameFilters, QObject *parent = NULL);
    ~SourceFileManager();
};

#endif // SOURCEFILEMANAGER_H
