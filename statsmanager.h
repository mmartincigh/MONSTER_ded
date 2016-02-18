#ifndef STATSMANAGER_H
#define STATSMANAGER_H

// Local
#include "base.h"

class StatsManager : public Base
{
    Q_OBJECT
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

private:
    static const QString m_CURRENT_INPUT_FILE_NONE;
    unsigned long long m_processedBytes;
    QString m_processedBytesString;
    unsigned long long m_bytesToProcess;
    QString m_bytesToProcessString;
    int m_errors;
    int m_warnings;
    int m_skipped;
    int m_overwritten;
    int m_processed;
    QString m_currentInputFile;

public:
    explicit StatsManager(QObject *parent = NULL);
    ~StatsManager();

public:
    unsigned long long processedBytes() const;
    QString processedBytesString() const;
    unsigned long long bytesToProcess() const;
    QString bytesToProcessString() const;
    int errors() const;
    int warnings() const;
    int skipped() const;
    int overwritten() const;
    int processed() const;
    QString currentInputFile() const;

private:
    void setProcessedBytes(unsigned long long processedBytes);
    void setProcessedBytesString(const QString &processedBytesString);
    void setBytesToProcess(unsigned long long bytesToProcess);
    void setBytesToProcessString(const QString& bytesToProcessString);
    void setErrors(int errors);
    void setWarnings(int warnings);
    void setSkipped(int skipped);
    void setOverwritten(int overwritten);
    void setProcessed(int processed);
    void setCurrentInputFile(const QString &currentInputFile);

public slots:
    void onProcessedBytesChanged(unsigned long long processedBytes);
    void onBytesToProcessChanged(unsigned long long bytesToProcess);
    void onErrorsChanged(int errors);
    void onWarningsChanged(int warnings);
    void onSkippedChanged(int skipped);
    void onOverwrittenChanged(int overwritten);
    void onProcessedChanged(int processed);
    void onCurrentInputFileChanged(const QString &currentInputFile);

signals:
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

#endif // STATSMANAGER_H
