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
#include <osrng.h>

// Local
#include "decryptionmanagerimpl.h"
#include "utils.h"

const QString DecryptionManagerImpl::m_CURRENT_INPUT_FILE_NONE("none");
const QString DecryptionManagerImpl::m_OUTPUT_FILE_EXTENSION(".mef");
const QString DecryptionManagerImpl::m_PASSPHRASE("Let's pretend that this is a clever passphrase");
const QString DecryptionManagerImpl::m_AES_KEY_FILE_NAME("AES_key.bin");
const QString DecryptionManagerImpl::m_AES_IV_FILE_NAME("AES_iv.bin");
const unsigned long long DecryptionManagerImpl::m_DECRYPTION_THRESHOLD_SIZE(10485760);
const unsigned long long DecryptionManagerImpl::m_DECRYPTION_CHUNK_SIZE(1048576);

DecryptionManagerImpl::DecryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    Base("DMI", parent),
    m_isEnabled(false),
    m_state(Enums::ProcessState_Idle),
    m_stateDescription(Utils::processStateToString(m_state)),
    m_pause(false),
    m_stop(false),
    m_decryptedBytes(0),
    m_decryptedBytesString(Utils::bytesToString(m_decryptedBytes)),
    m_bytesToDecrypt(0),
    m_bytesToDecryptString(Utils::bytesToString(m_bytesToDecrypt)),
    m_progress(0),
    m_progressString(Utils::progressToString(m_progress)),
    m_errors(0),
    m_warnings(0),
    m_skipped(0),
    m_overwritten(0),
    m_processed(0),
    m_currentInputFile(m_CURRENT_INPUT_FILE_NONE),
    m_key(CryptoPP::AES::MAX_KEYLENGTH),
    m_iv(),
    m_mutex(mutex),
    m_waitCondition(waitCondition)
{
    QObject::connect(this, SIGNAL(stateChanged(Enums::ProcessState)), this, SLOT(onStateChanged(Enums::ProcessState)));
    QObject::connect(this, SIGNAL(decryptedBytesChanged(unsigned long long)), this, SLOT(onBytesDecryptedChanged(unsigned long long)));
    QObject::connect(this, SIGNAL(bytesToDecryptChanged(unsigned long long)), this, SLOT(onBytesToDecryptChanged(unsigned long long)));
    QObject::connect(this, SIGNAL(progressChanged(float)), this, SLOT(onProgressChanged(float)));

    this->debug("Decryption manager implementation created");
}

DecryptionManagerImpl::~DecryptionManagerImpl()
{
    this->debug("Decryption manager implementation disposed of");
}

void DecryptionManagerImpl::initialize()
{
    this->debug("Initialized");
}

bool DecryptionManagerImpl::isEnabled() const
{
    return m_isEnabled;
}

Enums::ProcessState DecryptionManagerImpl::state() const
{
    return m_state;
}

QString DecryptionManagerImpl::stateDescription() const
{
    return m_stateDescription;
}

void DecryptionManagerImpl::setPause(bool pause)
{
    if (m_pause == pause)
    {
        return;
    }

    m_pause = pause;

    this->debug("Pause changed: " + QString(m_pause ? "true" : "false"));

    emit this->pauseChanged(m_pause);
}

void DecryptionManagerImpl::setStop(bool stop)
{
    if (m_stop == stop)
    {
        return;
    }

    m_stop = stop;

    this->debug("Stop changed: " + QString(m_stop ? "true" : "false"));

    emit this->stopChanged(m_stop);
}

unsigned long long DecryptionManagerImpl::decryptedBytes() const
{
    return m_decryptedBytes;
}

QString DecryptionManagerImpl::decryptedBytesString() const
{
    return m_decryptedBytesString;
}

unsigned long long DecryptionManagerImpl::bytesToDecrypt() const
{
    return m_bytesToDecrypt;
}

QString DecryptionManagerImpl::bytesToDecryptString() const
{
    return m_bytesToDecryptString;
}

float DecryptionManagerImpl::progress() const
{
    return m_progress;
}

QString DecryptionManagerImpl::progressString() const
{
    return m_progressString;
}

int DecryptionManagerImpl::errors() const
{
    return m_errors;
}

int DecryptionManagerImpl::warnings() const
{
    return m_warnings;
}

int DecryptionManagerImpl::skipped() const
{
    return m_skipped;
}

int DecryptionManagerImpl::overwritten() const
{
    return m_overwritten;
}

int DecryptionManagerImpl::processed() const
{
    return m_processed;
}

QString DecryptionManagerImpl::currentInputFile() const
{
    return m_currentInputFile;
}

void DecryptionManagerImpl::setIsEnabled(bool isEnabled)
{
    if (m_isEnabled == isEnabled)
    {
        return;
    }

    m_isEnabled = isEnabled;

    this->debug("Is enabled changed: " + QString(m_isEnabled ? "true" : "false"));

    emit this->isEnabledChanged(m_isEnabled);
}

void DecryptionManagerImpl::setState(Enums::ProcessState state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;

    this->debug("State changed: " + QString::number(m_state));

    emit this->stateChanged(m_state);
}

void DecryptionManagerImpl::setStateDescription(const QString &stateDescription)
{
    if (m_stateDescription == stateDescription)
    {
        return;
    }

    m_stateDescription = stateDescription;

    this->debug("State description changed: " + m_stateDescription);

    emit this->stateDescriptionChanged(m_stateDescription);
}

void DecryptionManagerImpl::setDecryptedBytes(unsigned long long decryptedBytes)
{
    if (m_decryptedBytes == decryptedBytes)
    {
        return;
    }

    m_decryptedBytes = decryptedBytes;

    //this->debug("Decrypted bytes changed: " + QString::number(m_decryptedBytes));

    emit this->decryptedBytesChanged(m_decryptedBytes);
}

void DecryptionManagerImpl::setDecryptedBytesString(const QString &decryptedBytesString)
{
    if (m_decryptedBytesString == decryptedBytesString)
    {
        return;
    }

    m_decryptedBytesString = decryptedBytesString;

    //this->debug("Decrypted bytes string changed: " + m_decryptedBytesString);

    emit this->decryptedBytesStringChanged(m_decryptedBytesString);
}

void DecryptionManagerImpl::setBytesToDecrypt(unsigned long long bytesToDecrypt)
{
    if (m_bytesToDecrypt == bytesToDecrypt)
    {
        return;
    }

    m_bytesToDecrypt = bytesToDecrypt;

    this->debug("Bytes to decrypt changed: " + QString::number(m_bytesToDecrypt));

    emit this->bytesToDecryptChanged(m_bytesToDecrypt);
}

void DecryptionManagerImpl::setBytesToDecryptString(const QString &bytesToDecryptString)
{
    if (m_bytesToDecryptString == bytesToDecryptString)
    {
        return;
    }

    m_bytesToDecryptString = bytesToDecryptString;

    this->debug("Bytes to decrypt string changed: " + m_bytesToDecryptString);

    emit this->bytesToDecryptStringChanged(m_bytesToDecryptString);
}

void DecryptionManagerImpl::setProgress(float progress)
{
    if (m_progress == progress)
    {
        return;
    }

    m_progress = progress;

    //this->debug("Progress changed: " + QString::number(m_progress));

    emit this->progressChanged(m_progress);
}

void DecryptionManagerImpl::setProgressString(const QString &progressString)
{
    if (m_progressString == progressString)
    {
        return;
    }

    m_progressString = progressString;

    //this->debug("Progress string changed: " + m_progressString);

    emit this->progressStringChanged(m_progressString);
}

void DecryptionManagerImpl::setErrors(int errors)
{
    if (m_errors == errors)
    {
        return;
    }

    m_errors = errors;

    this->debug("Errors changed: " + m_errors);

    emit this->errorsChanged(m_errors);
}

void DecryptionManagerImpl::setWarnings(int warnings)
{
    if (m_warnings == warnings)
    {
        return;
    }

    m_warnings = warnings;

    this->debug("Warnings changed: " + m_warnings);

    emit this->warningsChanged(m_warnings);
}

void DecryptionManagerImpl::setSkipped(int skipped)
{
    if (m_skipped == skipped)
    {
        return;
    }

    m_skipped = skipped;

    this->debug("Skipped changed: " + QString::number(m_skipped));

    emit this->skippedChanged(m_skipped);
}

void DecryptionManagerImpl::setOverwritten(int overwritten)
{
    if (m_overwritten == overwritten)
    {
        return;
    }

    m_overwritten = overwritten;

    this->debug("Overwritten changed: " + QString::number(m_overwritten));

    emit this->overwrittenChanged(m_overwritten);
}

void DecryptionManagerImpl::setProcessed(int processed)
{
    if (m_processed == processed)
    {
        return;
    }

    m_processed = processed;

    this->debug("Processed changed: " + QString::number(m_processed));

    emit this->processedChanged(m_processed);
}

void DecryptionManagerImpl::setCurrentInputFile(const QString &currentInputFile)
{
    if (m_currentInputFile == currentInputFile)
    {
        return;
    }

    m_currentInputFile = currentInputFile;

    this->debug("Current input file changed: " + m_currentInputFile);

    emit this->currentInputFileChanged(m_currentInputFile);
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

bool DecryptionManagerImpl::processStateCheckpoint()
{
    QMutexLocker mutex_locker(m_mutex);
    Q_UNUSED(mutex_locker)

    // Check whether the process should be paused, resumed or stopped.
    if (m_pause)
    {
        // The process should be paused.
        this->setState(Enums::ProcessState_Paused);
        emit this->paused();
        m_waitCondition->wait(m_mutex);

        if (!m_stop)
        {
            // The process has been resumed.
            this->setState(Enums::ProcessState_Working);
            emit this->working();
        }
    }

    if (m_stop)
    {
        // The process should be stopped.
        this->setState(Enums::ProcessState_Stopped);
        emit this->stopped();

        this->setProgress(0);

        return false;
    }

    return true;
}

bool DecryptionManagerImpl::readKeyFromFile()
{
    QString key_file_name = QDir::current().filePath(m_AES_KEY_FILE_NAME);
    QFile key_file(key_file_name);
    if (!key_file.exists())
    {
        this->error("The key file \"" + key_file_name + "\" does not exist");

        return false;
    }
    bool ret_val = key_file.open(QIODevice::ReadOnly);
    if (!ret_val)
    {
        this->error("Cannot open the key file: " + key_file_name);

        return false;
    }
    size_t key_size =  m_key.size();
    memset(m_key.data(), 0, key_size);
    long long bytes_read = key_file.read(reinterpret_cast<char *>(m_key.data()), key_size);
    key_file.close();
    if (bytes_read != key_size)
    {
        this->error("Wrong key size: expected " + Utils::bytesToString(key_size) + ", got " + Utils::bytesToString(bytes_read));

        return false;
    }
    if (m_key.empty())
    {
        this->error("The decryption key is empty");

        return false;
    }

    return true;
}

bool DecryptionManagerImpl::readIvFromFile()
{
    QString iv_file_name = QDir::current().filePath(m_AES_IV_FILE_NAME);
    QFile iv_file(iv_file_name);
    if (!iv_file.exists())
    {
        this->error("The IV file \"" + iv_file_name + "\" does not exist");

        return false;
    }
    bool ret_val = iv_file.open(QIODevice::ReadOnly);
    if (!ret_val)
    {
        this->error("Cannot open the IV file: " + iv_file_name);

        return false;
    }
    size_t iv_size = sizeof(m_iv);
    memset(m_iv, 0, iv_size);
    long long bytes_read = iv_file.read(reinterpret_cast<char *>(m_iv), iv_size);
    iv_file.close();
    if (bytes_read != iv_size)
    {
        this->error("Wrong IV size: expected " + Utils::bytesToString(iv_size) + ", got " + Utils::bytesToString(bytes_read));

        return false;
    }
    for (size_t i = 0; i < iv_size; i++)
    {
        if (m_iv[i] != 0)
        {
            return true;
        }
    }

    return false;
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

            this->setDecryptedBytes(m_decryptedBytes + inputFileSize);
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
                    this->warning("The decryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file may be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

                    return DecryptionState_Warning;
                }

                long long decrypted_bytes = file_source.Pump(m_DECRYPTION_CHUNK_SIZE);

                this->setDecryptedBytes(m_decryptedBytes + decrypted_bytes);

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

DecryptionManagerImpl::DecryptionState DecryptionManagerImpl::decryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime)
{
    this->debug("Decrypting file with AES: " + inputFile + " [" + Utils::bytesToString(inputFileSize) + "]");

    QTime time(0, 0, 0, 0);
    time.start();

    try
    {
        // Construct the decryption machinery.
        CryptoPP::FileSink *file_sink = new CryptoPP::FileSink(outputFile.toUtf8().constData());
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption aes_decryptor(m_key, m_key.size(), m_iv);
        CryptoPP::StreamTransformationFilter *stream_transformation_filter_sptr = new CryptoPP::StreamTransformationFilter(aes_decryptor, file_sink);
        CryptoPP::FileSource file_source(inputFile.toUtf8().constData(), false, stream_transformation_filter_sptr);

        // Decrypt the file.
        if (inputFileSize <= m_DECRYPTION_THRESHOLD_SIZE)
        {
            file_source.PumpAll();

            this->setDecryptedBytes(m_decryptedBytes + inputFileSize);
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
                    this->warning("The decryption was stopped in the middle of the file \"" + inputFile + "\", the resulting file may be corrupted");

                    int time_elapsed = time.elapsed();
                    QTime decryption_time = QTime(0, 0, 0, 0).addMSecs(time_elapsed);
                    decryptionTime.setHMS(decryption_time.hour(), decryption_time.minute(), decryption_time.second(), decryption_time.msec());

                    return DecryptionState_Warning;
                }

                long long decrypted_bytes = file_source.Pump(m_DECRYPTION_CHUNK_SIZE);

                this->setDecryptedBytes(m_decryptedBytes + decrypted_bytes);

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

void DecryptionManagerImpl::onDecryptFiles()
{
    this->debug("Decrypting files...");

    return;

    if (!m_isEnabled)
    {
        this->error("File Decryption not enabled");

        return;
    }

    // Get the decryption key from file.
    bool ret_val = this->readKeyFromFile();
    if (!ret_val)
    {
        this->error("Cannot get the decryption key from file");

        return;
    }
    this->debug("Decryption key read from file");

    // Get the initialization vector from file.
    ret_val = this->readIvFromFile();
    if (!ret_val)
    {
        this->error("Cannot get the initialization vector from file");

        return;
    }
    this->debug("Initialization vector read from file");

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

    // Decrypt the files.
    this->setDecryptedBytes(0);
    this->setBytesToDecrypt(input_files_total_size);
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
        QFileInfo input_file_info(secure_directory.filePath(input_files.at(i)));
        QString input_file = input_file_info.absoluteFilePath();
        this->debug("Working on file " + input_file + " [" + QString::number(i + 1) + "]...");
        this->setCurrentInputFile(input_file_info.fileName());

        // Check whether the input file exists.
        if (!input_file_info.exists())
        {
            this->error("The input file \"" + input_file + "\" does not exist");

            this->setDecryptedBytes(m_decryptedBytes + input_file_info.size());
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

                this->setDecryptedBytes(m_decryptedBytes + input_file_info.size());
                this->setSkipped(m_skipped + 1);

                continue;
            }
        }
        QString output_file = output_file_info.absoluteFilePath();

        // Decrypt the file.
        QTime decryption_time(0, 0, 0, 0);
        DecryptionState ret_val = this->decryptFileWithAes(input_file, input_file_info.size(), output_file, decryption_time);
        switch (ret_val)
        {
        case DecryptionState_Success:
            break;
        case DecryptionState_Warning:
            this->setWarnings(m_warnings + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                return;
            }

            continue;
        case DecryptionState_Error:
            this->setErrors(m_errors + 1);

            // Check whether the process should be paused, resumed or stopped.
            if (!this->processStateCheckpoint())
            {
                return;
            }

            continue;
        default:
            this->error("Unknown decryption state");
            return;
        }
        this->debug("File decrypted to: " + output_file + " [" + Utils::bytesToString(output_file_info.size()) + "]");
        this->debug("Decryption time: " + decryption_time.toString("HH:mm:ss:zzz"));
        this->setProcessed(m_processed + 1);
    }
    this->setProgress(1);
    this->setState(Enums::ProcessState_Completed);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);

    this->debug("Files decrypted");
}

void DecryptionManagerImpl::onUpdateState(Enums::ProcessState state)
{
    this->setState(state);
}

void DecryptionManagerImpl::onStateChanged(Enums::ProcessState state)
{
    this->setStateDescription(Utils::processStateToString(state));
}

void DecryptionManagerImpl::onProgressChanged(float progress)
{
    this->setProgressString(Utils::progressToString(progress));
}

void DecryptionManagerImpl::onBytesDecryptedChanged(unsigned long long bytesDecrypted)
{
    this->setDecryptedBytesString(Utils::bytesToString(bytesDecrypted));
    this->setProgress(bytesDecrypted / static_cast<float>(m_bytesToDecrypt));
}

void DecryptionManagerImpl::onBytesToDecryptChanged(unsigned long long bytesToDecrypt)
{
    this->setBytesToDecryptString(Utils::bytesToString(bytesToDecrypt));
}
