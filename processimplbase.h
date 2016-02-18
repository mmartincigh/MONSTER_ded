#ifndef PROCESSIMPLBASE_H
#define PROCESSIMPLBASE_H

// Qt
#include <QWaitCondition>

// Crypto++
#include <osrng.h>

// Local
#include "iprocessimpl.h"

class ProcessImplBase : public IProcessImpl
{
    Q_OBJECT

protected:
    static const QString m_CURRENT_INPUT_FILE_NONE;
    static const QString m_PASSPHRASE;
    static const QString m_AES_KEY_FILE_NAME;
    static const QString m_AES_IV_FILE_NAME;
    bool m_isEnabled;
    unsigned long long m_processedBytes;
    int m_errors;
    int m_warnings;
    int m_skipped;
    int m_overwritten;
    int m_processed;
    CryptoPP::SecByteBlock m_key;
    unsigned char m_iv[CryptoPP::AES::BLOCKSIZE];

private:
    Enums::ProcessState m_state;
    QString m_stateDescription;
    float m_progress;
    QString m_progressString;
    QString m_processedBytesString;
    unsigned long long m_bytesToProcess;
    QString m_bytesToProcessString;
    QString m_currentInputFile;
    bool m_pause;
    bool m_stop;
    QMutex *m_mutex;
    QWaitCondition *m_waitCondition;

public:
    explicit ProcessImplBase(QMutex *mutex, QWaitCondition *waitCondition, const QString &logTag, QObject *parent = NULL);
    ~ProcessImplBase();

public:
    virtual bool isEnabled() const;
    virtual Enums::ProcessState state() const;
    virtual QString stateDescription() const;
    virtual float progress() const;
    virtual QString progressString() const;
    virtual unsigned long long processedBytes() const;
    virtual QString processedBytesString() const;
    virtual unsigned long long bytesToProcess() const;
    virtual QString bytesToProcessString() const;
    virtual int errors() const;
    virtual int warnings() const;
    virtual int skipped() const;
    virtual int overwritten() const;
    virtual int processed() const;
    virtual QString currentInputFile() const;
    virtual void setPause(bool pause);
    virtual void setStop(bool stop);

protected:
    void setIsEnabled(bool isEnabled);
    void setState(Enums::ProcessState state);
    void setProgress(float progress);
    void setProcessedBytes(unsigned long long processedBytes);
    void setBytesToProcess(unsigned long long bytesToProcess);
    void setErrors(int errors);
    void setWarnings(int warnings);
    void setSkipped(int skipped);
    void setOverwritten(int overwritten);
    void setProcessed(int processed);
    void setCurrentInputFile(const QString &currentInputFile);
    bool readKeyFromFile();
    bool readIvFromFile();
    bool processStateCheckpoint();

private:
    void setStateDescription(const QString &stateDescription);
    void setProgressString(const QString &progressString);
    void setProcessedBytesString(const QString &processedBytesString);
    void setBytesToProcessString(const QString& bytesToProcessString);

private slots:
    void onStateChanged(Enums::ProcessState state);
    void onProgressChanged(float progress);
    void onProcessedBytesChanged(unsigned long long processedBytes);
    void onBytesToProcessChanged(unsigned long long bytesToProcess);

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
    void pauseChanged(bool pause);
    void stopChanged(bool stop);
    void working();
    void paused();
    void stopped();
};

#endif // PROCESSIMPLBASE_H
