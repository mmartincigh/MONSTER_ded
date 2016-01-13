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

#include "base.h"
#include "enums.h"

class FileEncryptor : public Base
{
    Q_OBJECT

private:
    static const unsigned long long m_ENCRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_ENCRYPTION_CHUNK_SIZE;

public:
    explicit FileEncryptor(QObject *parent = NULL);
    ~FileEncryptor();

public:
    int encryptFile(const QString &input, const QString &output, const QString &passphrase, QTime &encryptionTime, bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition);

private:
    bool processStateCheckpoint(bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition);

signals:
    void bytesEncryptedChanged(unsigned long long bytesEncryptedChanged);
    void stateChanged(Enums::State state);
    void working();
    void paused();
    void stopped();
};

#endif // FILEENCRYPTOR_H
