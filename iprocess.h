#ifndef IPROCESS_H
#define IPROCESS_H

// Qt
#include <QObject>

// Local
#include "base.h"
#include "enums.h"

class IProcess : public Base
{
    Q_OBJECT
    Q_PROPERTY(bool isEnabled READ isEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(Enums::ProcessState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString stateDescription READ stateDescription NOTIFY stateDescriptionChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString progressString READ progressString NOTIFY progressStringChanged)
    Q_PROPERTY(unsigned long long processedBytes READ processedBytes NOTIFY processedBytesChanged)
    Q_PROPERTY(QString processedBytesString READ processedBytesString NOTIFY processedBytesStringChanged)
    Q_PROPERTY(unsigned long long bytesToProcess READ bytesToProcess NOTIFY bytesToProcessChanged)
    Q_PROPERTY(QString bytesToProcessString READ bytesToProcessString NOTIFY bytesToProcessStringChanged)
    Q_PROPERTY(int errors READ errors NOTIFY errorsChanged)
    Q_PROPERTY(int warnings READ warnings NOTIFY warningsChanged)
    Q_PROPERTY(int skipped READ skipped NOTIFY skippedChanged)
    Q_PROPERTY(int overwritten READ overwritten NOTIFY overwrittenChanged)
    Q_PROPERTY(int processed READ processed NOTIFY processedChanged)
    Q_PROPERTY(QString currentInputFile READ currentInputFile NOTIFY currentInputFileChanged)

public:
    explicit IProcess(const QString &logTag, QObject *parent = NULL) : Base(logTag, parent) {}
    virtual ~IProcess() {}

public:
    virtual void initialize() = 0;
    virtual void uninitialize() = 0;
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

public slots:
    virtual void onProcess() = 0;
    virtual void onProcess(const QString &inputFile) = 0;
    virtual void onProcess(const QStringList &inputFiles) = 0;
    virtual void onPause() = 0;
    virtual void onResume() = 0;
    virtual void onStop() = 0;

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
    void progressChanged(float progress);
    void progressStringChanged(const QString &progressString);
    void processedBytesChanged(unsigned long long processedBytes);
    void processedBytesStringChanged(const QString &processedBytesString);
    void bytesToProcessChanged(unsigned long long bytesToProcess);
    void bytesToProcessStringChanged(const QString &bytesToProcessString);
    void errorsChanged(int errors);
    void warningsChanged(int warnings);
    void skippedChanged(int skipped);
    void overwrittenChanged(int overwritten);
    void processedChanged(int processed);
    void currentInputFileChanged(const QString &currentInputFile);
};

#endif // IPROCESS_H
