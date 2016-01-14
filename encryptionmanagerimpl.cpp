#if defined(Q_OS_WIN)
#include <Windows.h>
#else
#define MAX_PATH 260
#endif

#include <QDir>
#include <QTemporaryDir>
#include <QThread>
#include <QProcess>
#include <QTime>
#include <QImageReader>
#include <QtMath>

#include "encryptionmanagerimpl.h"
#include "utils.h"

const QString EncryptionManagerImpl::m_CURRENT_INPUT_FILE_NONE("none");
const QString EncryptionManagerImpl::m_OUTPUT_FILE_EXTENSION(".mef");
const QString EncryptionManagerImpl::m_PASSPHRASE("Let's pretend that this is a clever passphrase");

EncryptionManagerImpl::EncryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent) :
    Base("EMI", parent),
    m_isEnabled(false),
    m_state(Enums::Idle),
    m_stateDescription(Utils::thumbnailGenerationStateToString(m_state)),
    m_pause(false),
    m_stop(false),
    m_progress(0),
    m_progressString(Utils::progressToString(m_progress)),
    m_errors(0),
    m_warnings(0),
    m_skipped(0),
    m_overwritten(0),
    m_processed(0),
    m_currentInputFile(m_CURRENT_INPUT_FILE_NONE),
    m_mutex(mutex),
    m_waitCondition(waitCondition),
    m_fileEncryptor(new FileEncryptor(this))
{
    QObject::connect(this, SIGNAL(stateChanged(Enums::State)), this, SLOT(onStateChanged(Enums::State)));
    QObject::connect(this, SIGNAL(progressChanged(float)), this, SLOT(onProgressChanged(float)));

    QObject::connect(m_fileEncryptor, SIGNAL(stateChanged(Enums::State)), this, SLOT(onUpdateState(Enums::State)));
    //QObject::connect(m_fileEncryptor, SIGNAL(bytesEncryptedChanged(unsigned long long)), this, SLOT(onBytesEncryptedChanged(unsigned long long)));

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

    QStringList video_files;
    emit this->videoFiles(&video_files);
    if (video_files.size() == 0)
    {
        this->error("The source path contains no video files");

        return;
    }

    bool overwrite_output_files = false;
    emit this->overwriteOutputFiles(&overwrite_output_files);
    this->debug("Overwrite output files: " + QString(overwrite_output_files ? "true" : "false"));

    // Encrypt the files.
    float total_progress = video_files.size();
    int current_progress = 0;
    this->setProgress(0);
    this->setErrors(0);
    this->setWarnings(0);
    this->setSkipped(0);
    this->setOverwritten(0);
    this->setProcessed(0);
    this->setCurrentInputFile(m_CURRENT_INPUT_FILE_NONE);
    this->setState(Enums::Working);
    for (int i = 0; i < video_files.size(); i++)
    {
        // Check whether the process should be paused, resumed or stopped.
        if (!this->processStateCheckpoint())
        {
            return;
        }

        // Get the input file.
        QFileInfo input_file_info(source_directory.filePath(video_files.at(i)));
        QString input_file = input_file_info.absoluteFilePath();
        this->debug("Working on file " + input_file + " [" + QString::number(i + 1) + "]...");
        this->setCurrentInputFile(input_file_info.fileName());

        // Check whether the input file exists.
        if (!input_file_info.exists())
        {
            this->error("The input file \"" + input_file + "\" does not exist");

            this->setProgress(++current_progress / total_progress);
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

                this->setProgress(++current_progress / total_progress);
                this->setSkipped(m_skipped + 1);

                continue;
            }
        }
        QString output_file = output_file_info.absoluteFilePath();

        // Encrypt the file.
        QTime encryption_time(0, 0, 0, 0);
        int fe_ret_val = m_fileEncryptor->encryptFile(input_file, output_file, m_PASSPHRASE, encryption_time, &m_pause, &m_stop, m_mutex, m_waitCondition);
        if (fe_ret_val < 0)
        {
            emit this->error("Cannot encrypt file: " + input_file);

            this->setProgress(++current_progress / total_progress);
            this->setErrors(m_errors + 1);

            continue;
        }

        int encryption_time_milliseconds = encryption_time.hour() * 3600000 + encryption_time.minute() * 60000 + encryption_time.second() * 1000 + encryption_time.msec();

        emit this->debug("encryption_time_milliseconds: " + QString::number(encryption_time_milliseconds));

        // Check whether the process should be paused, resumed or stopped.
        /*if (!this->processStateCheckpoint())
        {
            return;
        }*/

        break;
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