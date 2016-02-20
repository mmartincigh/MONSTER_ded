#ifndef IPROCESSIMPL_H
#define IPROCESSIMPL_H

// Qt
#include <QObject>

// Local
#include "base.h"
#include "enums.h"

class IProcessImpl : public Base
{
    Q_OBJECT

public:
    explicit IProcessImpl(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IProcessImpl() {}

public:
    virtual bool isEnabled() const = 0;
    virtual Enums::ProcessState state() const = 0;
    virtual QString stateDescription() const = 0;
    virtual float progress() const = 0;
    virtual QString progressString() const = 0;
    virtual unsigned long long processedBytes() const = 0;
    virtual QString processedBytesString() const = 0;
    virtual unsigned long long bytesToProcess() const = 0;
    virtual QString bytesToProcessString() const = 0;
    virtual int errors() const = 0;
    virtual int warnings() const = 0;
    virtual int skipped() const = 0;
    virtual int overwritten() const = 0;
    virtual int processed() const = 0;
    virtual QString currentInputFile() const = 0;
    virtual void setPause(bool pause) = 0;
    virtual void setStop(bool stop) = 0;

public slots:
    virtual void onProcess() = 0;
    virtual void onProcess(const QString &inputFile) = 0;
};

#endif // IPROCESSIMPL_H
