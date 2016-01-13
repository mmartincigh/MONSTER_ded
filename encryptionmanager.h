#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

#include <QObject>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QStateMachine>
#include <QFinalState>
#include <QDebug>

#include "base.h"
#include "fileencryptor.h"
#include "encryptionstates.h"
#include "encryptionmanagerutils.h"

class EncryptionManager : public Base
{
    Q_OBJECT

private:
    static const QString m_ENCRYPTED_FILE_EXTENSION;
    static const QString m_PASSPHRASE;
    bool m_isEncrypting;
    QStringList m_files;
    QDir m_destination;
    bool m_overwrite;
    unsigned long long m_bytesToEncrypt;
    unsigned long long m_bytesEncrypted;
    double m_progress;
    QTime m_encryptionTime;
    EncryptionStates m_state;
    bool *m_pause;
    bool *m_stop;
    QMutex *m_mutex;
    QWaitCondition *m_waitCondition;
    QStateMachine m_stateMachine;
    QState *m_idle;
    QState *m_encrypting;
    QState *m_completed;
    QFinalState *m_exit;
    FileEncryptor *m_fileEncryptor;

public:
    explicit EncryptionManager(bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~EncryptionManager();

private:
    void setIsEncrypting(bool isEncrypting);
    void setFiles(const QStringList &files);
    void setDestination(const QDir &destination);
    void setOverwrite(bool overwrite);
    void setBytesToEncrypt(unsigned long long bytesToEncrypt);
    void setBytesEncrypted(unsigned long long bytesEncrypted);
    void setProgress(double progress);
    void setEncryptionTime(const QTime &encryptionTime);
    void setState(EncryptionStates state);
    void setPause(bool pause);
    void setStop(bool stop);

private slots:
    void onStateChanged();
    void onIdle();
    void onEncrypt();
    void onComplete();
    void onExit();
    void onBytesEncryptedChanged(unsigned long long bytesEncrypted);
    void onStateChanged(EncryptionStates state);

public:
    bool isEncrypting() const;
    double progress() const;
    EncryptionStates state() const;

public slots:
    void onStart(const QStringList &files);

signals:
    void isEncryptingChanged(bool isEncrypting);
    void filesChanged(const QStringList &files);
    void destinationChanged(const QDir &destination);
    void overwriteChanged(bool overwrite);
    void bytesToEncryptChanged(unsigned long long bytesToEncrypt);
    void bytesEncryptedChanged(unsigned long long bytesEncrypted);
    void progressChanged(double progress);
    void encryptionTimeChanged(const QTime &encryptionTime);
    void stateChanged(EncryptionStates state);
    void pauseChanged(bool pause);
    void stopChanged(bool stop);
    void encrypt();
    void complete();
    void exit();
    void started();
    void paused();
    void resumed();
    void stopped();
    void completed();
    void log(const QString &log);
    void warning(const QString &warning);
    void error(const QString &error, bool restartRequired = false);
};

#endif // ENCRYPTIONMANAGER_H
