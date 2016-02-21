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
#include "decryptionmanagerimpl.h"
#include "utils.h"

const unsigned long long DecryptionManagerImpl::m_DECRYPTION_THRESHOLD_SIZE(10485760);
const unsigned long long DecryptionManagerImpl::m_DECRYPTION_CHUNK_SIZE(1048576);

DecryptionManagerImpl::DecryptionManagerImpl(const QString &applicationDirPath, QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    ProcessImplBase(applicationDirPath, mutex, waitCondition, "DMI", parent)
{
    this->debug("Decryption manager implementation created");
}

DecryptionManagerImpl::~DecryptionManagerImpl()
{
    this->debug("Decryption manager implementation disposed of");
}

bool DecryptionManagerImpl::checkIfEnabled()
{
    bool is_secure_url_path_valid = false;
    emit this->isSecurePathUrlValid(&is_secure_url_path_valid);
    if (!is_secure_url_path_valid)
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

DecryptionManagerImpl::DecryptionState DecryptionManagerImpl::decryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime)
{
    this->debug("Decrypting file with MAC: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Construct the decryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::DefaultDecryptorWithMAC *decryptor = new CryptoPP::DefaultDecryptorWithMAC(m_PASSPHRASE.toUtf8().constData(), file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, decryptor);

        // Decrypt the file.
        if (inputFileSize <= m_DECRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setProcessedBytes(m_processedBytes + inputFileSize);
        }
        else
        {
            this->debug("File \"" + inputFile + "\" is bigger than " + Utils::bytesToString(m_DECRYPTION_THRESHOLD_SIZE) + ", decrypting by pumping chunks of " + Utils::bytesToString(m_DECRYPTION_CHUNK_SIZE) + "...");

            unsigned long long total_decrypted_bytes = 0;
            while (total_decrypted_bytes < inputFileSize)
            {
                // Check whether the decryption should be paused, resumed or stopped.
                if (!this->processStateCheckpoint())
                {
                    this->warning("The decryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file might be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

                    return DecryptionState_Warning;
                }

                long long decrypted_bytes = file_source.Pump(m_DECRYPTION_CHUNK_SIZE);

                this->setProcessedBytes(m_processedBytes + decrypted_bytes);

                total_decrypted_bytes += decrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot decrypt file \"" + inputFile + "\" with MAC. " + e.what());

        return DecryptionState_Error;
    }

    int time_elapsed = time.elapsed();
    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

    return DecryptionState_Success;
}

DecryptionManagerImpl::DecryptionState DecryptionManagerImpl::decryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, const CryptoPP::SecByteBlock key, const unsigned char *iv, QTime &decryptionTime)
{
    this->debug("Decrypting file with AES: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Construct the decryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption aes_decryptor(key, key.size(), iv);
        CryptoPP::StreamTransformationFilter *stream_transformation_filter_sptr = new CryptoPP::StreamTransformationFilter(aes_decryptor, file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, stream_transformation_filter_sptr);

        // Decrypt the file.
        if (inputFileSize <= m_DECRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setProcessedBytes(m_processedBytes + inputFileSize);
        }
        else
        {
            this->debug("File \"" + inputFile + "\" is bigger than " + Utils::bytesToString(m_DECRYPTION_THRESHOLD_SIZE) + ", decrypting by pumping chunks of " + Utils::bytesToString(m_DECRYPTION_CHUNK_SIZE) + "...");

            unsigned long long total_decrypted_bytes = 0;
            while (total_decrypted_bytes < inputFileSize)
            {
                // Check whether the decryption should be paused, resumed or stopped.
                if (!this->processStateCheckpoint())
                {
                    this->warning("The decryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file might be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

                    return DecryptionState_Warning;
                }

                long long decrypted_bytes = file_source.Pump(m_DECRYPTION_CHUNK_SIZE);

                this->setProcessedBytes(m_processedBytes + decrypted_bytes);

                total_decrypted_bytes += decrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot decrypt file \"" + inputFile + "\" with AES. " + e.what());

        return DecryptionState_Error;
    }

    int time_elapsed = time.elapsed();
    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

    return DecryptionState_Success;
}

void DecryptionManagerImpl::onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid)
{
    this->debug("Is secure path URL vaild: " + QString(isSecurePathUrlValid ? "true" : "false"));

    if (!isSecurePathUrlValid)
    {
        this->setIsEnabled(false);

        return;
    }

    this->setIsEnabled(this->checkIfEnabled());
}

void DecryptionManagerImpl::onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid)
{
    this->debug("Is destination path URL vaild: " + QString(isDestinationPathUrlValid ? "true" : "false"));

    if (!isDestinationPathUrlValid)
    {
        this->setIsEnabled(false);

        return;
    }

    this->setIsEnabled(this->checkIfEnabled());
}

void DecryptionManagerImpl::onProcess()
{
    this->debug("Decrypting files...");

    if (!m_isEnabled)
    {
        this->error("File Decryption not enabled");

        return;
    }

    // Get the secure path.
    QString secure_path;
    emit this->securePath(&secure_path);
    if (secure_path.isEmpty())
    {
        this->error("Secure path is empty");

        return;
    }
    QDir secure_directory(secure_path);
    if (!secure_directory.exists())
    {
        this->error("The secure directory does not exist: " + secure_directory.path());

        return;
    }
    this->debug("Secure path: " + secure_path);

    // Get the destination path.
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
        this->error("The secure path contains no input files");

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
        QFileInfo input_file_info(secure_directory.filePath(input_files.at(i)));
        input_files_total_size += input_file_info.size();
    }
    this->debug("Input files total size: " + QString::number(input_files_total_size) + "B [" + Utils::bytesToString(input_files_total_size) + "]");

    // Get the decryption key from file.
    CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH);
    bool ret_val = this->readKeyFromFile(key);
    if (!ret_val)
    {
        this->error("Cannot get the decryption key from file");

        return;
    }
    this->debug("Decryption key read from file");

    // Get the initialization vector from file.
    unsigned char iv[CryptoPP::AES::BLOCKSIZE];
    size_t iv_size = sizeof(iv);
    ret_val = this->readIvFromFile(iv, iv_size);
    if (!ret_val)
    {
        this->error("Cannot get the initialization vector from file");

        this->clearKey(key);

        return;
    }
    this->debug("Initialization vector read from file");

    // Decrypt the files.
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
            this->clearKey(key);
            this->clearIv(iv, iv_size);

            return;
        }

        // Get the input file.
        QFileInfo input_file_info(secure_directory.filePath(input_files.at(i)));
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

        // Check whether the output file name is valid.
        QString output_file = destination_directory.filePath(input_file_info.completeBaseName());
        if (output_file.length() > MAX_PATH)
        {
            this->error("The output file name \"" + output_file + "\" is too long");

            this->setProcessedBytes(m_processedBytes + input_file_info.size());
            this->setErrors(m_errors + 1);

            continue;
        }

        // Check whether the output file can be overwritten.
        QFileInfo output_file_info(output_file);
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

        // Decrypt the file.
        QTime decryption_time(0, 0, 0, 0);
        DecryptionState ret_val = this->decryptFileWithAes(input_file, input_file_info.size(), output_file, key, iv, decryption_time);
        switch (ret_val)
        {
        case DecryptionState_Success:
            break;
        case DecryptionState_Warning:
            this->setWarnings(m_warnings + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                this->clearKey(key);
                this->clearIv(iv, iv_size);

                return;
            }

            continue;
        case DecryptionState_Error:
            this->setErrors(m_errors + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                this->clearKey(key);
                this->clearIv(iv, iv_size);

                return;
            }

            continue;
        default:
            this->error("Unknown decryption state");

            this->clearKey(key);
            this->clearIv(iv, iv_size);

            return;
        }
        this->debug("File decrypted to: " + output_file + " [" + Utils::bytesToString(output_file_info.size()) + "]");
        this->debug("Decryption time: " + decryption_time.toString("HH:mm:ss:zzz"));
        this->setProcessed(m_processed + 1);
    }
    this->setProgress(1);
    this->setState(Enums::ProcessState_Completed);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);
    this->clearKey(key);
    this->clearIv(iv, iv_size);

    this->debug("Files decrypted");
}

void DecryptionManagerImpl::onProcess(const QString &inputFile)
{
    this->debug("Decrypting file: " + inputFile);

    // Check whether the input file exists.
    QFileInfo input_file_info(inputFile);
    if (!input_file_info.exists())
    {
        this->error("The input file \"" + inputFile + "\" does not exist");

        return;
    }

    // Save the decrypted file in the same path of the input file.
    QString output_path(input_file_info.absolutePath());
    if (output_path.isEmpty())
    {
        this->error("Output path is empty");

        return;
    }
    QDir output_directory(output_path);
    if (!output_directory.exists())
    {
        this->error("The output directory does not exist: " + output_directory.path());

        return;
    }
    this->debug("Output path: " + output_path);

    // Get the size of the input file.
    unsigned long long input_file_size = input_file_info.size();
    this->debug("Input file size: " + QString::number(input_file_size) + "B [" + Utils::bytesToString(input_file_size) + "]");

    // Get the decryption key from file.
    CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH);
    bool ret_val = this->readKeyFromFile(key);
    if (!ret_val)
    {
        this->error("Cannot get the decryption key from file");

        return;
    }
    this->debug("Decryption key read from file");

    // Get the initialization vector from file.
    unsigned char iv[CryptoPP::AES::BLOCKSIZE];
    size_t iv_size = sizeof(iv);
    ret_val = this->readIvFromFile(iv, iv_size);
    if (!ret_val)
    {
        this->error("Cannot get the initialization vector from file");

        this->clearKey(key);

        return;
    }
    this->debug("Initialization vector read from file");

    // Decrypt the file.
    this->setProcessedBytes(0);
    this->setBytesToProcess(input_file_size);
    this->setProgress(0);
    this->setErrors(0);
    this->setWarnings(0);
    this->setSkipped(0);
    this->setOverwritten(0);
    this->setProcessed(0);
    this->setCurrentInputFile(input_file_info.fileName());
    this->setState(Enums::ProcessState_Working);

    // Check whether the output file name is valid.
    QString output_file = output_directory.filePath(input_file_info.completeBaseName());
    if (output_file.length() > MAX_PATH)
    {
        this->error("The output file name \"" + output_file + "\" is too long");

        this->setProcessedBytes(input_file_info.size());
        this->setErrors(1);
        this->clearKey(key);
        this->clearIv(iv, iv_size);

        return;
    }

    // Decrypt the file.
    QTime decryption_time(0, 0, 0, 0);
    DecryptionState decryption_ret_val = this->decryptFileWithAes(inputFile, input_file_info.size(), output_file, key, iv, decryption_time);
    switch (decryption_ret_val)
    {
    case DecryptionState_Success:
        break;
    case DecryptionState_Warning:
        this->setProcessedBytes(input_file_info.size());
        this->setWarnings(1);
        this->clearKey(key);
        this->clearIv(iv, iv_size);

        return;
    case DecryptionState_Error:
        this->setProcessedBytes(input_file_info.size());
        this->setErrors(1);
        this->clearKey(key);
        this->clearIv(iv, iv_size);

        return;
    default:
        this->error("Unknown decryption state");

        this->setProcessedBytes(input_file_info.size());
        this->setErrors(1);
        this->clearKey(key);
        this->clearIv(iv, iv_size);

        return;
    }

    // File decrypted.
    QFileInfo output_file_info(output_file);
    this->debug("File decrypted to: " + output_file + " [" + Utils::bytesToString(output_file_info.size()) + "]");
    this->debug("Decryption time: " + decryption_time.toString("HH:mm:ss:zzz"));
    this->setProcessed(1);
    this->setProgress(1);
    this->setState(Enums::ProcessState_Completed);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);
    this->clearKey(key);
    this->clearIv(iv, iv_size);

    this->debug("File decrypted");

    // Try to open the file.
    emit this->openFile(output_file);
}
