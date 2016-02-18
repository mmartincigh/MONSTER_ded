#ifndef IFILE_H
#define IFILE_H

// Local
#include "base.h"

class IFile : public Base
{
    Q_OBJECT

public:
    explicit IFile(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IFile() {}

public:
    virtual QStringList inputFiles() const = 0;
    virtual int inputFilesCount() const = 0;
    virtual bool overwriteOutputFiles() const = 0;
    virtual void setOverwriteOutputFiles(bool overwriteOutputFiles) = 0;
};

#endif // IFILE_H
