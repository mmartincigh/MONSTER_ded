#if defined(Q_OS_WIN)
#include <Windows.h>
#else
#define MAX_PATH 260
#endif

// Qt
#include <QDir>
#include <QTemporaryDir>
#include <QThread>
#include <QProcess>
#include <QTime>
#include <QImageReader>
#include <QtMath>

// Crypto++
#include <default.h>
#include <files.h>
#include <osrng.h>

// Local
#include "encryptionmanagerimpl.h"
#include "utils.h"

const QString EncryptionManagerImpl::m_CURRENT_INPUT_FILE_NONE("none");
const QString EncryptionManagerImpl::m_OUTPUT_FILE_EXTENSION(".mef");
const QString EncryptionManagerImpl::m_PASSPHRASE("Let's pretend that this is a clever passphrase");
const unsigned long long EncryptionManagerImpl::m_ENCRYPTION_THRESHOLD_SIZE(1048576);
const unsigned long long EncryptionManagerImpl::m_ENCRYPTION_CHUNK_SIZE(1048576);

EncryptionManagerImpl::EncryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    Base("EMI", parent),
    m_isEnabled(false),
    m_state(Enums::Idle),
    m_stateDescription(Utils::thumbnailGenerationStateToString(m_state)),
    m_pause(false),
    m_stop(false),
    m_encryptedBytes(0),
    m_encryptedBytesString(Utils::bytesToString(m_encryptedBytes)),
    m_bytesToEncrypt(0),
    m_bytesToEncryptString(Utils::bytesToString(m_bytesToEncrypt)),
    m_progress(0),
    m_progressString(Utils::progressToString(m_progress)),
    m_errors(0),
    m_warnings(0),
    m_skipped(0),
    m_overwritten(0),
    m_processed(0),
    m_currentInputFile(m_CURRENT_INPUT_FILE_NONE),
    m_mutex(mutex),
    m_waitCondition(waitCondition)
{
    QObject::connect(this, SIGNAL(stateChanged(Enums::State)), this, SLOT(onStateChanged(Enums::State)));
    QObject::connect(this, SIGNAL(encryptedBytesChanged(unsigned long long)), this, SLOT(onBytesEncryptedChanged(unsigned long long)));
    QObject::connect(this, SIGNAL(bytesToEncryptChanged(unsigned long long)), this, SLOT(onBytesToEncryptChanged(unsigned long long)));
    QObject::connect(this, SIGNAL(progressChanged(float)), this, SLOT(onProgressChanged(float)));

    this->debug("Thumbnail generator implementation created");
}

EncryptionManagerImpl::~EncryptionManagerImpl()
{
    this->debug("Thumbnail generator implementation disposed of");
}

void EncryptionManagerImpl::initialize()
{
    this->debug("Initialized");
}

bool EncryptionManagerImpl::isEnabled() const
{
    return m_isEnabled;
}

Enums::State EncryptionManagerImpl::state() const
{
    return m_state;
}

QString EncryptionManagerImpl::stateDescription() const
{
    return m_stateDescription;
}

void EncryptionManagerImpl::setPause(bool pause)
{
    if (m_pause == pause)
    {
        return;
    }

    m_pause = pause;

    this->debug("Pause changed: " + QString(m_pause ? "true" : "false"));

    emit this->pauseChanged(m_pause);
}

void EncryptionManagerImpl::setStop(bool stop)
{
    if (m_stop == stop)
    {
        return;
    }

    m_stop = stop;

    this->debug("Stop changed: " + QString(m_stop ? "true" : "false"));

    emit this->stopChanged(m_stop);
}

unsigned long long EncryptionManagerImpl::encryptedBytes() const
{
    return m_encryptedBytes;
}

QString EncryptionManagerImpl::encryptedBytesString() const
{
    return m_encryptedBytesString;
}

unsigned long long EncryptionManagerImpl::bytesToEncrypt() const
{
    return m_bytesToEncrypt;
}

QString EncryptionManagerImpl::bytesToEncryptString() const
{
    return m_bytesToEncryptString;
}

float EncryptionManagerImpl::progress() const
{
    return m_progress;
}

QString EncryptionManagerImpl::progressString() const
{
    return m_progressString;
}

int EncryptionManagerImpl::errors() const
{
    return m_errors;
}

int EncryptionManagerImpl::warnings() const
{
    return m_warnings;
}

int EncryptionManagerImpl::skipped() const
{
    return m_skipped;
}

int EncryptionManagerImpl::overwritten() const
{
    return m_overwritten;
}

int EncryptionManagerImpl::processed() const
{
    return m_processed;
}

QString EncryptionManagerImpl::currentInputFile() const
{
    return m_currentInputFile;
}

void EncryptionManagerImpl::setIsEnabled(bool isEnabled)
{
    if (m_isEnabled == isEnabled)
    {
        return;
    }

    m_isEnabled = isEnabled;

    this->debug("Is enabled changed: " + QString(m_isEnabled ? "true" : "false"));

    emit this->isEnabledChanged(m_isEnabled);
}

void EncryptionManagerImpl::setState(Enums::State state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;

    this->debug("State changed: " + QString::number(m_state));

    emit this->stateChanged(m_state);
}

void EncryptionManagerImpl::setStateDescription(const QString &stateDescription)
{
    if (m_stateDescription == stateDescription)
    {
        return;
    }

    m_stateDescription = stateDescription;

    this->debug("State description changed: " + m_stateDescription);

    emit this->stateDescriptionChanged(m_stateDescription);
}

void EncryptionManagerImpl::setEncryptedBytes(unsigned long long encryptedBytes)
{
    if (m_encryptedBytes == encryptedBytes)
    {
        return;
    }

    m_encryptedBytes = encryptedBytes;

    this->debug("Encrypted bytes changed: " + QString::number(m_encryptedBytes));

    emit this->encryptedBytesChanged(m_encryptedBytes);
}

void EncryptionManagerImpl::setEncryptedBytesString(const QString &encryptedBytesString)
{
    if (m_encryptedBytesString == encryptedBytesString)
    {
        return;
    }

    m_encryptedBytesString = encryptedBytesString;

    this->debug("Encrypted bytes string changed: " + m_encryptedBytesString);

    emit this->encryptedBytesStringChanged(m_encryptedBytesString);
}

void EncryptionManagerImpl::setBytesToEncrypt(unsigned long long bytesToEncrypt)
{
    if (m_bytesToEncrypt == bytesToEncrypt)
    {
        return;
    }

    m_bytesToEncrypt = bytesToEncrypt;

    this->debug("Bytes to encrypt changed: " + QString::number(m_bytesToEncrypt));

    emit this->bytesToEncryptChanged(m_bytesToEncrypt);
}

void EncryptionManagerImpl::setBytesToEncryptString(const QString &bytesToEncryptString)
{
    if (m_bytesToEncryptString == bytesToEncryptString)
    {
        return;
    }

    m_bytesToEncryptString = bytesToEncryptString;

    this->debug("Bytes to encrypt string changed: " + m_bytesToEncryptString);

    emit this->bytesToEncryptStringChanged(m_bytesToEncryptString);
}

void EncryptionManagerImpl::setProgress(float progress)
{
    if (m_progress == progress)
    {
        return;
    }

    m_progress = progress;

    this->debug("Progress changed: " + QString::number(m_progress));

    emit this->progressChanged(m_progress);
}

void EncryptionManagerImpl::setProgressString(const QString &progressString)
{
    if (m_progressString == progressString)
    {
        return;
    }

    m_progressString = progressString;

    this->debug("Progress string changed: " + m_progressString);

    emit this->progressStringChanged(m_progressString);
}

void EncryptionManagerImpl::setErrors(int errors)
{
    if (m_errors == errors)
    {
        return;
    }

    m_errors = errors;

    this->debug("Errors changed: " + m_errors);

    emit this->errorsChanged(m_errors);
}

void EncryptionManagerImpl::setWarnings(int warnings)
{
    if (m_warnings == warnings)
    {
        return;
    }

    m_warnings = warnings;

    this->debug("Warnings changed: " + m_warnings);

    emit this->warningsChanged(m_warnings);
}

void EncryptionManagerImpl::setSkipped(int skipped)
{
    if (m_skipped == skipped)
    {
        return;
    }

    m_skipped = skipped;

    this->debug("Skipped changed: " + QString::number(m_skipped));

    emit this->skippedChanged(m_skipped);
}

void EncryptionManagerImpl::setOverwritten(int overwritten)
{
    if (m_overwritten == overwritten)
    {
        return;
    }

    m_overwritten = overwritten;

    this->debug("Overwritten changed: " + QString::number(m_overwritten));

    emit this->overwrittenChanged(m_overwritten);
}

void EncryptionManagerImpl::setProcessed(int processed)
{
    if (m_processed == processed)
    {
        return;
    }

    m_processed = processed;

    this->debug("Processed changed: " + QString::number(m_processed));

    emit this->processedChanged(m_processed);
}

void EncryptionManagerImpl::setCurrentInputFile(const QString &currentInputFile)
{
    if (m_currentInputFile == currentInputFile)
    {
        return;
    }

    m_currentInputFile = currentInputFile;

    this->debug("Current input file changed: " + m_currentInputFile);

    emit this->currentInputFileChanged(m_currentInputFile);
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

bool EncryptionManagerImpl::processStateCheckpoint()
{
    QMutexLocker mutex_locker(m_mutex);
    Q_UNUSED(mutex_locker)

    // Check whether the process should be paused, resumed or stopped.
    if (m_pause)
    {
        // The process should be paused.
        this->setState(Enums::Paused);
        emit this->paused();
        m_waitCondition->wait(m_mutex);

        if (!m_stop)
        {
            // The process has been resumed.
            this->setState(Enums::Working);
            emit this->working();
        }
    }

    if (m_stop)
    {
        // The process should be stopped.
        this->setState(Enums::Stopped);
        emit this->stopped();
        this->setStop(false);

        this->setProgress(0);

        return false;
    }

    return true;
}

bool EncryptionManagerImpl::encryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime)
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

            this->setEncryptedBytes(m_encryptedBytes + inputFileSize);
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

                    return true;
                }

                long long encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                this->setEncryptedBytes(m_encryptedBytes + encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot encrypt file \"" + inputFile + "\" with MAC. " + e.what());

        return false;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return true;
}

bool EncryptionManagerImpl::encryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime)
{
    this->debug("Encrypting file with AES: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Generate a random key and iv.
        CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH);
        CryptoPP::AutoSeededRandomPool random_generator;
        random_generator.GenerateBlock(key, key.size());
        unsigned char iv[CryptoPP::AES::BLOCKSIZE];
        random_generator.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);

        // Construct the encryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption aes_encryptor(key, key.size(), iv);
        CryptoPP::StreamTransformationFilter *stream_transformation_filter_sptr = new CryptoPP::StreamTransformationFilter(aes_encryptor, file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, stream_transformation_filter_sptr);

        // Encrypt the file.
        if (inputFileSize <= m_ENCRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setEncryptedBytes(m_encryptedBytes + inputFileSize);
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

                    return true;
                }

                long long encrypted_bytes = file_source.Pump(m_ENCRYPTION_CHUNK_SIZE);

                this->setEncryptedBytes(m_encryptedBytes + encrypted_bytes);

                total_encrypted_bytes += encrypted_bytes;
            }
            file_source.PumpAll();
        }
    }
    catch (const std::exception &e)
    {
        this->error("Cannot encrypt file \"" + inputFile + "\" with AES. " + e.what());

        return false;
    }

    int time_elapsed = time.elapsed();
    QTime encryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
    encryptionTime.setHMS(encryption_time.hour(), encryption_time.minute(), encryption_time.second(), encryption_time.msec());

    return true;
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

void EncryptionManagerImpl::onEncryptFiles()
{
    this->debug("Encrypting files...");

    if (!m_isEnabled)
    {
        this->error("File encryption not enabled");

        return;
    }

    // Get the source path.
    QString source_path;
    emit this->sourcePath(&source_path);
    this->debug("Source path: " + source_path);
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

    // Get the destiantion path.
    QString destination_path;
    emit this->destinationPath(&destination_path);
    this->debug("Destination path: " + destination_path);
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
    this->setEncryptedBytes(0);
    this->setBytesToEncrypt(input_files_total_size);
    this->setProgress(0);
    this->setErrors(0);
    this->setWarnings(0);
    this->setSkipped(0);
    this->setOverwritten(0);
    this->setProcessed(0);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);
    this->setState(Enums::Working);
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

            this->setEncryptedBytes(m_encryptedBytes + input_file_info.size());
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

                this->setEncryptedBytes(m_encryptedBytes + input_file_info.size());
                this->setSkipped(m_skipped + 1);

                continue;
            }
        }
        QString output_file = output_file_info.absoluteFilePath();

        // Encrypt the file.
        QTime encryption_time(0, 0, 0, 0);
        //bool ret_val = this->encryptFileWithMac(input_file, input_file_info.size(), output_file, encryption_time);
        bool ret_val = this->encryptFileWithAes(input_file, input_file_info.size(), output_file, encryption_time);
        if (!ret_val)
        {
            emit this->error("Cannot encrypt file: " + input_file);

            this->setErrors(m_errors + 1);

            continue;
        }
        this->debug("File encrypted to: " + output_file + " [" + Utils::bytesToString(output_file_info.size()) + "]");
        this->debug("Encryption time: " + encryption_time.toString("HH:mm:ss:zzz"));
        this->setProcessed(m_processed + 1);
    }
    this->setProgress(1);
    this->setState(Enums::Completed);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);

    this->debug("Files encrypted");
}

void EncryptionManagerImpl::onUpdateState(Enums::State state)
{
    this->setState(state);
}

void EncryptionManagerImpl::onStateChanged(Enums::State state)
{
    this->setStateDescription(Utils::thumbnailGenerationStateToString(state));
}

void EncryptionManagerImpl::onProgressChanged(float progress)
{
    this->setProgressString(Utils::progressToString(progress));
}

void EncryptionManagerImpl::onBytesEncryptedChanged(unsigned long long bytesEncrypted)
{
    this->setEncryptedBytesString(Utils::bytesToString(bytesEncrypted));
    this->setProgress(bytesEncrypted / static_cast<float>(m_bytesToEncrypt));
}

void EncryptionManagerImpl::onBytesToEncryptChanged(unsigned long long bytesToEncrypt)
{
    this->setBytesToEncryptString(Utils::bytesToString(bytesToEncrypt));
}
