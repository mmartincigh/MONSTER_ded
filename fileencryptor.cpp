#include "fileencryptor.h"

const unsigned long long FileEncryptor::m_ENCRYPTION_THRESHOLD_SIZE(10485760);
const unsigned long long FileEncryptor::m_ENCRYPTION_CHUNK_SIZE(10485760);

FileEncryptor::FileEncryptor(QObject *parent) :
    Base("FE", parent)
{
    this->debug("File encryptor created");
}

FileEncryptor::~FileEncryptor()
{
    this->debug("File encryptor disposed of");
}

int FileEncryptor::encryptFile(const QString &input, const QString &output, const QString &passphrase, QTime &encryptionTime, bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition)
{
    if (pause == NULL
            || stop == NULL
            || mutex == NULL
            || waitCondition == NULL)
    {
        this->error("Invalid parameter(s)");

        return -1;
    }

    QFileInfo input_file_info(input);
    if (!input_file_info.exists())
    {
        this->warning("Attempted to encrypt non-existing file: " + input);

        return -2;
    }

    long long input_size = input_file_info.size();

    this->debug("Encrypting file: " + input + "[" + QString::number(input_size) + "B]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(output.toUtf8().constData());
        CryptoPP::DefaultEncryptorWithMAC *encryptor = new CryptoPP::DefaultEncryptorWithMAC(passphrase.toUtf8().constData(), file_sink);
        CryptoPP::FileSource file_source(input.toUtf8().constData(), false, encryptor);

        if (input_size < m_ENCRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            emit this->bytesEncryptedChanged(input_size);
        }
        else
        {
            this->debug("File \"" + input + "\" bigger than " + QString::number(m_ENCRYPTION_THRESHOLD_SIZE) + "B, encrypting by pumping chunks of " + QString::number(m_ENCRYPTION_CHUNK_SIZE) + "B...");

            long long total_encrypted_bytes = 0;
            while (total_encrypted_bytes < input_size)
            {
                // Check whether the encryption should be paused, resumed or stopped.
                if (!this->processStateCheckpoint(pause, stop, mutex, waitCondition))
                {
                    this->warning("The encryption was stopped in the middle of the file \"" + input + "\", the resulting file may be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

                    return 0;
                }

                long long encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                emit this->bytesEncryptedChanged(encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot encrypt file \"" + input + "\": " + e.what());

        return -3;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);

    QFileInfo output_file_info(output);

    this->debug("File encrypted to: " + output + "[" + QString::number(output_file_info.size()) + "B] [" + encryption_time.toString("HH:mm:ss:zzz") + "]");

    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return 0;
}

bool FileEncryptor::processStateCheckpoint(bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition)
{
    if (pause == NULL
            || stop == NULL
            || mutex == NULL
            || waitCondition == NULL)
    {
        this->error("Invalid parameter(s)");

        return false;
    }

    QMutexLocker mutex_locker(mutex);
    Q_UNUSED(mutex_locker)

    // Check whether the process should be paused, resumed or stopped.
    if (*pause)
    {
        // The process should be paused.
        emit this->stateChanged(Enums::Paused);
        emit this->paused();
        this->debug("Encryption paused");
        waitCondition->wait(mutex);

        if (!(*stop))
        {
            // The process has been resumed.
            emit this->stateChanged(Enums::Working);
            emit this->working();
            this->debug("Encryption resumed");
        }
    }

    if (*stop)
    {
        // The process should be stopped.
        this->stateChanged(Enums::Stopped);
        emit this->stopped();
        this->debug("Encryption stopped");
        *stop = false;

        return false;
    }

    return true;
}
