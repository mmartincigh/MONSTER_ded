#ifndef SOURCEFILEMANAGER_H
#define SOURCEFILEMANAGER_H

// Local
#include "filebase.h"

class SourceFileManager : public FileBase
{
    Q_OBJECT

public:
    explicit SourceFileManager(const QStringList &nameFilters, QObject *parent = NULL);
    ~SourceFileManager();
};

#endif // SOURCEFILEMANAGER_H
