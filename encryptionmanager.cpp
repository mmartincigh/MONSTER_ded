#include "encryptionmanager.h"

const QString EncryptionManager::m_ENCRYPTED_FILE_EXTENSION("mef");
const QString EncryptionManager::m_PASSPHRASE("Let's pretend that this is a clever passphrase.");

EncryptionManager::EncryptionManager(bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    QObject(parent),
    m_isEncrypting(false),
    m_files(),
    m_destination("D:\\Temp\\EncryptionTest"),
    m_overwrite(true),
    m_bytesToEncrypt(0),
    m_bytesEncrypted(0),
    m_progress(0),
    m_encryptionTime(0, 0, 0, 0),
    m_state(EncryptionIdle),
    m_pause(pause),
    m_stop(stop),
    m_mutex(mutex),
    m_waitCondition(waitCondition),
    m_stateMachine(this),
    m_idle(new QState()),
    m_encrypting(new QState()),
    m_completed(new QState()),
    m_exit(new QFinalState()),
    m_fileEncryptor(new FileEncryptor(this))
{
    QObject::connect(m_fileEncryptor, SIGNAL(bytesEncrypted(qint64)), this, SLOT(onBytesEncrypted(qint64)));
    QObject::connect(m_fileEncryptor, SIGNAL(stateChanged(EncryptionStates)), this, SLOT(onStateChanged(EncryptionStates)));
    QObject::connect(m_fileEncryptor, SIGNAL(paused()), this, SIGNAL(paused()));
    QObject::connect(m_fileEncryptor, SIGNAL(resumed()), this, SIGNAL(resumed()));
    QObject::connect(m_fileEncryptor, SIGNAL(stopped()), this, SIGNAL(stopped()));
    QObject::connect(m_fileEncryptor, SIGNAL(exit()), this, SIGNAL(exit()));
    QObject::connect(m_fileEncryptor, SIGNAL(log(QString)), this, SIGNAL(log(QString)));

    QObject::connect(m_idle, SIGNAL(entered()), this, SLOT(onStateChanged()));
    QObject::connect(m_idle, SIGNAL(entered()), this, SLOT(onIdle()));

    QObject::connect(m_encrypting, SIGNAL(entered()), this, SLOT(onStateChanged()));
    QObject::connect(m_encrypting, SIGNAL(entered()), this, SLOT(onEncrypt()));

    QObject::connect(m_completed, SIGNAL(entered()), this, SLOT(onStateChanged()));
    QObject::connect(m_completed, SIGNAL(entered()), this, SLOT(onComplete()));

    QObject::connect(m_exit, SIGNAL(entered()), this, SLOT(onStateChanged()));
    QObject::connect(m_exit, SIGNAL(entered()), this, SLOT(onExit()));

    m_stateMachine.addState(m_idle);
    m_stateMachine.addState(m_encrypting);
    m_stateMachine.addState(m_completed);
    m_stateMachine.addState(m_exit);
    m_stateMachine.setInitialState(m_idle);

    m_idle->addTransition(this, SIGNAL(encrypt()), m_encrypting);
    m_idle->addTransition(this, SIGNAL(exit()), m_exit);

    m_encrypting->addTransition(this, SIGNAL(complete()), m_completed);
    m_encrypting->addTransition(this, SIGNAL(exit()), m_exit);

    m_completed->addTransition(this, SIGNAL(exit()), m_exit);

    qDebug() << "EM> Encryption manager created";
}

EncryptionManager::~EncryptionManager()
{
    qDebug() << "EM> Encryption manager disposed of";
}

void EncryptionManager::setIsEncrypting(bool isEncrypting)
{
    if (m_isEncrypting == isEncrypting)
    {
        return;
    }

    m_isEncrypting = isEncrypting;
    emit this->isEncryptingChanged(m_isEncrypting);

    qDebug() << "EM> Is encrypting changed:" << m_isEncrypting;
}

void EncryptionManager::setFiles(const QStringList &files)
{
    if (m_files.count() == files.count())
    {
        bool files_equal = true;
        for (int i = 0; i < m_files.count() && files_equal; i++)
        {
            files_equal = m_files.at(i) != files.at(i);
        }
        if (files_equal)
        {
            return;
        }
    }

    m_files = files;
    emit this->filesChanged(m_files);

    qDebug().nospace() << "EM> Files changed: " << m_files.count() << " file" << (m_files.count() == 1 ? "" : "s");
}

void EncryptionManager::setDestination(const QDir &destination)
{
    if (m_destination == destination)
    {
        return;
    }

    m_destination = destination;
    emit this->destinationChanged(m_destination);

    qDebug() << "EM> Destination changed:" << qPrintable(m_destination.absolutePath());
}

void EncryptionManager::setOverwrite(bool overwrite)
{
    if (m_overwrite == overwrite)
    {
        return;
    }

    m_overwrite = overwrite;
    emit this->overwriteChanged(m_overwrite);

    qDebug() << "EM> Overwrite changed:" << m_overwrite;
}

void EncryptionManager::setBytesToEncrypt(quint64 bytesToEncrypt)
{
    if (m_bytesToEncrypt == bytesToEncrypt)
    {
        return;
    }

    m_bytesToEncrypt = bytesToEncrypt;
    emit this->bytesToEncryptChanged(m_bytesToEncrypt);

    qDebug() << "EM> Bytes to encrypt changed:" << m_bytesToEncrypt;
}

void EncryptionManager::setBytesEncrypted(quint64 bytesEncrypted)
{
    if (m_bytesEncrypted == bytesEncrypted)
    {
        return;
    }

    m_bytesEncrypted = bytesEncrypted;
    emit this->bytesEncryptedChanged(m_bytesEncrypted);

    qDebug() << "EM> Bytes encrypted changed:" << m_bytesEncrypted;
}

void EncryptionManager::setProgress(double progress)
{
    if (m_progress == progress)
    {
        return;
    }

    m_progress = progress;
    emit this->progressChanged(m_progress);

    qDebug() << "EM> Progress changed:" << qPrintable(QString::number(m_progress, 'f'));
}

void EncryptionManager::setEncryptionTime(const QTime &encryptionTime)
{
    if (m_encryptionTime == encryptionTime)
    {
        return;
    }

    m_encryptionTime = encryptionTime;
    emit this->encryptionTimeChanged(m_encryptionTime);

    qDebug() << "EM> Encryption time changed:" << qPrintable(m_encryptionTime.toString("HH:mm:ss:zzz"));
}

void EncryptionManager::setState(EncryptionStates state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit this->stateChanged(m_state);

    qDebug() << "EM> State changed:" << qPrintable(EncryptionManagerUtils::stateToString(m_state));
}

void EncryptionManager::setPause(bool pause)
{
    if (*m_pause == pause)
    {
        return;
    }

    *m_pause = pause;
    emit this->pauseChanged(*m_pause);

    qDebug() << "EM> Pause changed:" << *m_pause;
}

void EncryptionManager::setStop(bool stop)
{
    if (*m_stop == stop)
    {
        return;
    }

    *m_stop = stop;
    emit this->stopChanged(*m_stop);

    qDebug() << "EM> Stop changed:" << *m_stop;
}

void EncryptionManager::onStateChanged()
{
    if (m_stateMachine.configuration().contains(m_idle))
    {
        qDebug() << "EM> FSM state changed: idle";
    }
    else if (m_stateMachine.configuration().contains(m_encrypting))
    {
        qDebug() << "EM> FSM state changed: encrypting";
    }
    else if (m_stateMachine.configuration().contains(m_completed))
    {
        qDebug() << "EM> FSM state changed: completed";
    }
    else if (m_stateMachine.configuration().contains(m_exit))
    {
        qDebug() << "EM> FSM state changed: exit";
    }
}

void EncryptionManager::onIdle()
{
    qDebug() << "EM> Starting encryption...";

    // Calculate the total size of the data to encrypt.
    quint64 bytes_to_encrypt = 0;
    for (int i = 0; i < m_files.count(); i++)
    {
        QFileInfo file_to_encrypt_info(m_files.at(i));

        bytes_to_encrypt += file_to_encrypt_info.size();
    }

    qDebug().nospace() << "EM> Total size of the data to encrypt: " << bytes_to_encrypt << "B";

    this->setIsEncrypting(true);
    this->setBytesToEncrypt(bytes_to_encrypt);
    this->setBytesEncrypted(0);
    this->setProgress(0);
    this->setEncryptionTime(QTime(0, 0, 0, 0));

    emit this->encrypt();

    qDebug() << "EM> Encryption started";
}

void EncryptionManager::onEncrypt()
{
    emit this->log(QString("Encrypting %1 file%2...").arg(m_files.count()).arg(m_files.count() == 1 ? "" : "s"));

    qDebug().nospace() << "EM> Encrypting " << m_files.count() << " file" << (m_files.count() == 1 ? "" : "s") << "...";

    this->setState(EncryptionEncrypting);

    emit this->started();

    // ------------------------------------
    emit this->complete();

    return;
    // ------------------------------------

    for (int i = 0; i < m_files.count(); i++)
    {
        // Check whether the process should be paused, resumed or stopped.
        m_mutex->lock();
        if (*m_pause)
        {
            this->setState(EncryptionPaused);

            emit this->paused();

            qDebug() << "EM> Encryption paused";

            m_waitCondition->wait(m_mutex);

            this->setState(EncryptionEncrypting);

            emit this->resumed();

            qDebug() << "EM> Encryption resumed";
        }
        if (*m_stop)
        {
            this->setState(EncryptionStopped);

            emit this->stopped();

            qDebug() << "EM> Encryption stopped";

            m_mutex->unlock();

            emit this->exit();

            return;
        }
        m_mutex->unlock();

        // Prepare the file encryption.
        QFileInfo input_file_info(m_files.at(i));
        QString output_filename(m_destination.filePath(input_file_info.fileName().append(".").append(m_ENCRYPTED_FILE_EXTENSION)));
        QFileInfo output_file_info(output_filename);
        if (output_file_info.exists()
                && !m_overwrite)
        {
            emit this->log(QString("Skipping existing file: %1").arg(output_filename));

            qDebug() << "EM> Skipping existing file:" << qPrintable(output_filename);

            continue;
        }

        // Encrypt the file.
        QTime encryption_time(0, 0, 0, 0);
        int ret_val = m_fileEncryptor->encryptFile(m_files.at(i), output_filename, m_PASSPHRASE, encryption_time, m_pause, m_stop, m_mutex, m_waitCondition);
        if (ret_val < 0)
        {
            emit this->log(QString("ERROR: Cannot encrypt file: %1").arg(input_file_info.fileName()));

            qCritical() << "EM> ERROR: Cannot encrypt file:" << qPrintable(input_file_info.fileName());

            continue;
        }

        int encryption_time_milliseconds = encryption_time.hour() * 3600000 + encryption_time.minute() * 60000 + encryption_time.second() * 1000 + encryption_time.msec();
        this->setEncryptionTime(m_encryptionTime.addMSecs(encryption_time_milliseconds));
    }

    emit this->complete();
}

void EncryptionManager::onComplete()
{
    qDebug() << "EM> Completing encryption...";

    this->setState(EncryptionCompleted);

    emit this->completed();

    emit this->log(QString("Encryption completed [%1]").arg(m_encryptionTime.toString("HH:mm:ss:zzz")));

    qDebug().nospace() << "EM> Encryption completed [" << qPrintable(m_encryptionTime.toString("HH:mm:ss:zzz")) << "]";

    qDebug() << "EM> Encryption completed";

    emit this->exit();
}

void EncryptionManager::onExit()
{
    qDebug() << "EM> Exiting encryption...";

    m_stateMachine.stop();

    this->setState(EncryptionIdle);

    this->setIsEncrypting(false);
    this->setFiles(QStringList());
    this->setPause(false);
    this->setStop(false);

    qDebug() << "EM> Encryption exited";
}

void EncryptionManager::onBytesEncrypted(qint64 bytesEncrypted)
{
    this->setBytesEncrypted(m_bytesEncrypted + bytesEncrypted);

    if (m_bytesToEncrypt <= 0)
    {
        return;
    }

    double progress = static_cast<double>(m_bytesEncrypted) / static_cast<double>(m_bytesToEncrypt);

    this->setProgress(progress);
}

void EncryptionManager::onStateChanged(EncryptionStates state)
{
    this->setState(state);
}

bool EncryptionManager::isEncrypting() const
{
    return m_isEncrypting;
}

double EncryptionManager::progress() const
{
    return m_progress;
}

EncryptionStates EncryptionManager::state() const
{
    return m_state;
}

void EncryptionManager::onStart(const QStringList &files)
{
    this->setFiles(files);

    m_stateMachine.start();
}
