#ifndef FILEENCRYPTOR_H
#define FILEENCRYPTOR_H

#include <QObject>
#include <QFileInfo>
#include <QTime>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#include <default.h>
#include <files.h>

#include "encryptionstates.h"

class FileEncryptor : public QObject
{
    Q_OBJECT

private:
    static const quint64 m_ENCRYPTION_THRESHOLD_SIZE;
    static const quint64 m_ENCRYPTION_CHUNK_SIZE;

public:
    explicit FileEncryptor(QObject *parent = NULL);
    ~FileEncryptor();

public:
    int encryptFile(const QString &input, const QString &output, const QString &passphrase, QTime &encryptionTime, bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition);

signals:
    void bytesEncrypted(qint64 bytesEncrypted);
    void stateChanged(EncryptionStates state);
    void paused();
    void resumed();
    void stopped();
    void exit();
    void log(const QString &log);
};

#endif // FILEENCRYPTOR_H
