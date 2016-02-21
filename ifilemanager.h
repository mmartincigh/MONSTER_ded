#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H

// Local
#include "base.h"

class IFileManager : public Base
{
    Q_OBJECT

public:
    explicit IFileManager(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IFileManager() {}

public:
    virtual QStringList inputFiles() const = 0;
    virtual int inputFilesCount() const = 0;
    virtual bool overwriteOutputFiles() const = 0;
    virtual void setOverwriteOutputFiles(bool overwriteOutputFiles) = 0;
};

#endif // IFILEMANAGER_H
