#if defined(Q_OS_WIN)
#include <Windows.h>
#else
#define MAX_PATH 260
#endif

// Qt
#include <QDir>
#include <QTime>

// Crypto++
#include <default.h>
#include <files.h>

// Local
#include "encryptionmanagerimpl.h"
#include "utils.h"

const QString EncryptionManagerImpl::m_OUTPUT_FILE_EXTENSION(".mef");
const unsigned long long EncryptionManagerImpl::m_ENCRYPTION_THRESHOLD_SIZE(10485760);
const unsigned long long EncryptionManagerImpl::m_ENCRYPTION_CHUNK_SIZE(1048576);

EncryptionManagerImpl::EncryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    ProcessImplBase(mutex, waitCondition, "EMI", parent)
{
    this->debug("Encryption manager implementation created");
}

EncryptionManagerImpl::~EncryptionManagerImpl()
{
    this->debug("Encryption manager implementation disposed of");
}

bool EncryptionManagerImpl::checkIfEnabled()
{
    bool is_source_url_path_valid = false;
    emit this->isSourcePathUrlValid(&is_source_url_path_valid);
    if (!is_source_url_path_valid)
    {
        return false;
    }

    bool is_destination_url_path_valid = false;
    emit this->isDestinationPathUrlValid(&is_destination_url_path_valid);
    if (!is_destination_url_path_valid)
    {
        return false;
    }

    return true;
}

EncryptionManagerImpl::EncryptionState EncryptionManagerImpl::encryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime)
{
    this->debug("Encrypting file with MAC: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Construct the encryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::DefaultEncryptorWithMAC *encryptor = new CryptoPP::DefaultEncryptorWithMAC(m_PASSPHRASE.toUtf8().constData(), file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, encryptor);

        // Encrypt the file.
        if (inputFileSize <= m_ENCRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setProcessedBytes(m_processedBytes + inputFileSize);
        }
        else
        {
            this->debug("File \"" + inputFile + "\" is bigger than " + Utils::bytesToString(m_ENCRYPTION_THRESHOLD_SIZE) + ", encrypting by pumping chunks of " + Utils::bytesToString(m_ENCRYPTION_CHUNK_SIZE) + "...");

            unsigned long long total_encrypted_bytes = 0;
            while (total_encrypted_bytes < inputFileSize)
            {
                // Check whether the encryption should be paused, resumed or stopped.
                if (!this->processStateCheckpoint())
                {
                    this->warning("The encryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file may be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

                    return EncryptionState_Warning;
                }

                long long encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                this->setProcessedBytes(m_processedBytes + encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot encrypt file \"" + inputFile + "\" with MAC. " + e.what());

        return EncryptionState_Error;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return EncryptionState_Success;
}

EncryptionManagerImpl::EncryptionState EncryptionManagerImpl::encryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime)
{
    this->debug("Encrypting file with AES: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Construct the encryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption aes_encryptor(m_key, m_key.size(), m_iv);
        CryptoPP::StreamTransformationFilter *stream_transformation_filter_sptr = new CryptoPP::StreamTransformationFilter(aes_encryptor, file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, stream_transformation_filter_sptr);

        // Encrypt the file.
        if (inputFileSize <= m_ENCRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setProcessedBytes(m_processedBytes + inputFileSize);
        }
        else
        {
            this->debug("File \"" + inputFile + "\" is bigger than " + Utils::bytesToString(m_ENCRYPTION_THRESHOLD_SIZE) + ", encrypting by pumping chunks of " + Utils::bytesToString(m_ENCRYPTION_CHUNK_SIZE) + "...");

            unsigned long long total_encrypted_bytes = 0;
            while (total_encrypted_bytes < inputFileSize)
            {
                // Check whether the encryption should be paused, resumed or stopped.
                if (!this->processStateCheckpoint())
                {
                    this->warning("The encryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file may be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

                    return EncryptionState_Warning;
                }

                long long encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                this->setProcessedBytes(m_processedBytes + encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot encrypt file \"" + inputFile + "\" with AES. " + e.what());

        return EncryptionState_Error;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return EncryptionState_Success;
}

void EncryptionManagerImpl::onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid)
{
    this->debug("Is source path URL vaild: " + QString(isSourcePathUrlValid ? "true" : "false"));

    if (!isSourcePathUrlValid)
    {
        this->setIsEnabled(false);

        return;
    }

    this->setIsEnabled(this->checkIfEnabled());
}

void EncryptionManagerImpl::onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid)
{
    this->debug("Is destination path URL vaild: " + QString(isDestinationPathUrlValid ? "true" : "false"));

    if (!isDestinationPathUrlValid)
    {
        this->setIsEnabled(false);

        return;
    }

    this->setIsEnabled(this->checkIfEnabled());
}

void EncryptionManagerImpl::onProcess()
{
    this->debug("Encrypting files...");

    if (!m_isEnabled)
    {
        this->error("File encryption not enabled");

        return;
    }

    // Get the encryption key from file.
    bool ret_val = this->readKeyFromFile();
    if (!ret_val)
    {
        this->error("Cannot get the encryption key from file");

        return;
    }
    this->debug("Encryption key read from file");

    // Get the initialization vector from file.
    ret_val = this->readIvFromFile();
    if (!ret_val)
    {
        this->error("Cannot get the initialization vector from file");

        return;
    }
    this->debug("Initialization vector read from file");

    // Get the source path.
    QString source_path;
    emit this->sourcePath(&source_path);
    if (source_path.isEmpty())
    {
        this->error("Source path is empty");

        return;
    }
    QDir source_directory(source_path);
    if (!source_directory.exists())
    {
        this->error("The source directory does not exist: " + source_directory.path());

        return;
    }
    this->debug("Source path: " + source_path);

    // Get the destiantion path.
    QString destination_path;
    emit this->destinationPath(&destination_path);
    if (destination_path.isEmpty())
    {
        this->error("Destination path is empty");

        return;
    }
    QDir destination_directory(destination_path);
    if (!destination_directory.exists())
    {
        this->error("The destination directory does not exist: " + destination_directory.path());

        return;
    }
    this->debug("Destination path: " + destination_path);

    // Get the input files.
    QStringList input_files;
    emit this->inputFiles(&input_files);
    if (input_files.size() == 0)
    {
        this->error("The source path contains no input files");

        return;
    }

    // Get the overwrite flag.
    bool overwrite_output_files = false;
    emit this->overwriteOutputFiles(&overwrite_output_files);
    this->debug("Overwrite output files: " + QString(overwrite_output_files ? "true" : "false"));

    // Calculate the total size of the input files.
    unsigned long long input_files_total_size = 0;
    for (int i = 0; i < input_files.size(); i++)
    {
        QFileInfo input_file_info(source_directory.filePath(input_files.at(i)));
        input_files_total_size += input_file_info.size();
    }
    this->debug("Input files total size: " + QString::number(input_files_total_size) + "B [" + Utils::bytesToString(input_files_total_size) + "]");

    // Encrypt the files.
    this->setPause(false);
    this->setStop(false);
    this->setProcessedBytes(0);
    this->setBytesToProcess(input_files_total_size);
    this->setProgress(0);
    this->setErrors(0);
    this->setWarnings(0);
    this->setSkipped(0);
    this->setOverwritten(0);
    this->setProcessed(0);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);
    this->setState(Enums::ProcessState_Working);
    for (int i = 0; i < input_files.size(); i++)
    {
        // Check whether the process should be paused, resumed or stopped.
        if (!this->processStateCheckpoint())
        {
            return;
        }

        // Get the input file.
        QFileInfo input_file_info(source_directory.filePath(input_files.at(i)));
        QString input_file = input_file_info.absoluteFilePath();
        this->debug("Working on file " + input_file + " [" + QString::number(i + 1) + "]...");
        this->setCurrentInputFile(input_file_info.fileName());

        // Check whether the input file exists.
        if (!input_file_info.exists())
        {
            this->error("The input file \"" + input_file + "\" does not exist");

            this->setProcessedBytes(m_processedBytes + input_file_info.size());
            this->setErrors(m_errors + 1);

            continue;
        }

        // Check whether the output file can be overwritten.
        QFileInfo output_file_info(destination_directory.filePath(input_file_info.completeBaseName()).left(MAX_PATH - m_OUTPUT_FILE_EXTENSION.length() - 1) + m_OUTPUT_FILE_EXTENSION);
        bool output_file_exists = output_file_info.exists();
        if (!overwrite_output_files)
        {
            // The output file cannot be overwritten, check whether the file already exists.
            if (output_file_exists)
            {
                this->debug("The output file \"" + output_file_info.fileName() + "\" already exists, skipping...");

                this->setProcessedBytes(m_processedBytes + input_file_info.size());
                this->setSkipped(m_skipped + 1);

                continue;
            }
        }
        QString output_file = output_file_info.absoluteFilePath();

        // Encrypt the file.
        QTime encryption_time(0, 0, 0, 0);
        EncryptionState ret_val = this->encryptFileWithAes(input_file, input_file_info.size(), output_file, encryption_time);
        switch (ret_val)
        {
        case EncryptionState_Success:
            break;
        case EncryptionState_Warning:
            this->setWarnings(m_warnings + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                return;
            }

            continue;
        case EncryptionState_Error:
            this->setErrors(m_errors + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                return;
            }

            continue;
        default:
            this->error("Unknown encryption state");
            return;
        }
        this->debug("File encrypted to: " + output_file + " [" + Utils::bytesToString(output_file_info.size()) + "]");
        this->debug("Encryption time: " + encryption_time.toString("HH:mm:ss:zzz"));
        this->setProcessed(m_processed + 1);
    }
    this->setProgress(1);
    this->setState(Enums::ProcessState_Completed);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);

    this->debug("Files encrypted");
}
