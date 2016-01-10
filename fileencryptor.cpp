#include "fileencryptor.h"

const quint64 FileEncryptor::m_ENCRYPTION_THRESHOLD_SIZE(10485760);
const quint64 FileEncryptor::m_ENCRYPTION_CHUNK_SIZE(10485760);

FileEncryptor::FileEncryptor(QObject *parent) :
    QObject(parent)
{
    qDebug() << "FE> File encryptor created";
}

FileEncryptor::~FileEncryptor()
{
    qDebug() << "FE> File encryptor disposed of";
}

int FileEncryptor::encryptFile(const QString &input, const QString &output, const QString &passphrase, QTime &encryptionTime, bool *pause, bool *stop, QMutex *mutex, QWaitCondition *waitCondition)
{
    QFileInfo input_file_info(input);
    if (!input_file_info.exists())
    {
        emit this->log(QString("WARNING: Attempted to encrypt non-existing file: %1").arg(input));

        qWarning() << "FE> WARNING: Attempted to encrypt non-existing file:" << qPrintable(input);

        return -1;
    }

    qint64 input_size = input_file_info.size();

    emit this->log(QString("Encrypting file: %1[%2B]").arg(input).arg(input_size));

    qDebug().nospace() << "FE> Encrypting file: " << qPrintable(input) << "[" << input_size << "B]";

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

            emit this->bytesEncrypted(input_size);
        }
        else
        {
            emit this->log(QString("File \"%1\" bigger than %2B, encrypting by pumping chunks of %3B...").arg(input).arg(m_ENCRYPTION_THRESHOLD_SIZE).arg(m_ENCRYPTION_CHUNK_SIZE));

            qDebug().nospace() << "FE> File " << input << " bigger than " << m_ENCRYPTION_THRESHOLD_SIZE << "B, encrypting by pumping chunks of " << m_ENCRYPTION_CHUNK_SIZE << "B...";

            qint64 total_encrypted_bytes = 0;
            while (total_encrypted_bytes < input_size)
            {
                // Check whether the process should be paused, resumed or stopped.
                mutex->lock();
                if (*pause)
                {
                    emit this->stateChanged(EncryptionPaused);

                    emit this->paused();

                    qDebug() << "FE> Encryption paused";

                    waitCondition->wait(mutex);

                    emit this->stateChanged(EncryptionEncrypting);

                    emit this->resumed();

                    qDebug() << "FE> Encryption resumed";
                }
                if (*stop)
                {
                    emit this->stateChanged(EncryptionStopped);

                    emit this->stopped();

                    qDebug() << "FE> Encryption stopped";

                    mutex->unlock();

                    emit this->log(QString("ERROR: Encryption stopped in the middle of the file \"%1\", the resulting file may be corrupted and/or non-readable").arg(input));

                    qCritical().nospace() << "FE> ERROR: Encryption stopped in the middle of the file " << input << ", the resulting file may be corrupted and/or non-readable";

                    emit this->exit();

                    return 0;
                }
                mutex->unlock();

                qint64 encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                emit this->bytesEncrypted(encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        emit this->log(QString("ERROR: Cannot encrypt file \"%1\": %2").arg(input).arg(e.what()));

        qCritical().nospace() << "FE> ERROR: Cannot encrypt file " << input << ": " << e.what();

        return -2;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);

    emit this->log(QString("File encrypted to: %1 [%2]").arg(output).arg(encryption_time.toString("HH:mm:ss:zzz")));

    QFileInfo output_file_info(output);

    qDebug().nospace() << "FE> File encrypted to: " << qPrintable(output) << "[" << output_file_info.size() << "B] [" << qPrintable(encryption_time.toString("HH:mm:ss:zzz")) << "]";

    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return 0;
}
