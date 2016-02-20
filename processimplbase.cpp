// Qt
#include <QDir>

// Local
#include "processimplbase.h"
#include "utils.h"

const QString ProcessImplBase::m_CURRENT_INPUT_FILE_NONE("none");
const QString ProcessImplBase::m_PASSPHRASE("Let's pretend that this is a clever passphrase");
const QString ProcessImplBase::m_AES_KEY_FILE_NAME("AES_key.bin");
const QString ProcessImplBase::m_AES_IV_FILE_NAME("AES_iv.bin");

ProcessImplBase::ProcessImplBase(const QString &applicationDirPath, QMutex *mutex, QWaitCondition *waitCondition, const QString &logTag, QObject *parent) :
    IProcessImpl(logTag, parent),
    m_isEnabled(false),
    m_processedBytes(0),
    m_errors(0),
    m_warnings(0),
    m_skipped(0),
    m_overwritten(0),
    m_processed(0),
    m_iv(),
    m_mutex(mutex),
    m_state(Enums::ProcessState_Idle),
    m_stateDescription(Utils::processStateToString(m_state)),
    m_progress(0),
    m_progressString(Utils::progressToString(m_progress)),
    m_processedBytesString(Utils::bytesToString(m_processedBytes)),
    m_bytesToProcess(0),
    m_bytesToProcessString(Utils::bytesToString(m_bytesToProcess)),
    m_currentInputFile(m_CURRENT_INPUT_FILE_NONE),
    m_pause(false),
    m_stop(false),
    m_applicationDirPath(applicationDirPath),
    m_key(CryptoPP::AES::MAX_KEYLENGTH),
    m_waitCondition(waitCondition)
{
    QObject::connect(this, SIGNAL(stateChanged(Enums::ProcessState)), this, SLOT(onStateChanged(Enums::ProcessState)));
    QObject::connect(this, SIGNAL(progressChanged(float)), this, SLOT(onProgressChanged(float)));
    QObject::connect(this, SIGNAL(processedBytesChanged(unsigned long long)), this, SLOT(onProcessedBytesChanged(unsigned long long)));
    QObject::connect(this, SIGNAL(bytesToProcessChanged(unsigned long long)), this, SLOT(onBytesToProcessChanged(unsigned long long)));
}

ProcessImplBase::~ProcessImplBase()
{
    this->clearSecrets();
}

bool ProcessImplBase::isEnabled() const
{
    return m_isEnabled;
}

Enums::ProcessState ProcessImplBase::state() const
{
    return m_state;
}

QString ProcessImplBase::stateDescription() const
{
    return m_stateDescription;
}

float ProcessImplBase::progress() const
{
    return m_progress;
}

QString ProcessImplBase::progressString() const
{
    return m_progressString;
}

unsigned long long ProcessImplBase::processedBytes() const
{
    return m_processedBytes;
}

QString ProcessImplBase::processedBytesString() const
{
    return m_processedBytesString;
}

unsigned long long ProcessImplBase::bytesToProcess() const
{
    return m_bytesToProcess;
}

QString ProcessImplBase::bytesToProcessString() const
{
    return m_bytesToProcessString;
}


int ProcessImplBase::errors() const
{
    return m_errors;
}

int ProcessImplBase::warnings() const
{
    return m_warnings;
}

int ProcessImplBase::skipped() const
{
    return m_skipped;
}

int ProcessImplBase::overwritten() const
{
    return m_overwritten;
}

int ProcessImplBase::processed() const
{
    return m_processed;
}

QString ProcessImplBase::currentInputFile() const
{
    return m_currentInputFile;
}

void ProcessImplBase::setPause(bool pause)
{
    if (m_pause == pause)
    {
        return;
    }

    m_pause = pause;

    this->debug("Pause changed: " + QString(m_pause ? "true" : "false"));

    emit this->pauseChanged(m_pause);
}

void ProcessImplBase::setStop(bool stop)
{
    if (m_stop == stop)
    {
        return;
    }

    m_stop = stop;

    this->debug("Stop changed: " + QString(m_stop ? "true" : "false"));

    emit this->stopChanged(m_stop);
}
void ProcessImplBase::setIsEnabled(bool isEnabled)
{
    if (m_isEnabled == isEnabled)
    {
        return;
    }

    m_isEnabled = isEnabled;

    this->debug("Is enabled changed: " + QString(m_isEnabled ? "true" : "false"));

    emit this->isEnabledChanged(m_isEnabled);
}

void ProcessImplBase::setState(Enums::ProcessState state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;

    this->debug("State changed: " + QString::number(m_state));

    emit this->stateChanged(m_state);
}

void ProcessImplBase::setProgress(float progress)
{
    if (m_progress == progress)
    {
        return;
    }

    m_progress = progress;

    //this->debug("Progress changed: " + QString::number(m_progress));

    emit this->progressChanged(m_progress);
}

void ProcessImplBase::setProcessedBytes(unsigned long long processedBytes)
{
    if (m_processedBytes == processedBytes)
    {
        return;
    }

    m_processedBytes = processedBytes;

    //this->debug("Processed bytes changed: " + QString::number(m_processedBytes));

    emit this->processedBytesChanged(m_processedBytes);
}

void ProcessImplBase::setBytesToProcess(unsigned long long bytesToProcess)
{
    if (m_bytesToProcess == bytesToProcess)
    {
        return;
    }

    m_bytesToProcess = bytesToProcess;

    this->debug("Bytes to encrypt changed: " + QString::number(m_bytesToProcess));

    emit this->bytesToProcessChanged(m_bytesToProcess);
}

void ProcessImplBase::setErrors(int errors)
{
    if (m_errors == errors)
    {
        return;
    }

    m_errors = errors;

    this->debug("Errors changed: " + QString::number(m_errors));

    emit this->errorsChanged(m_errors);
}

void ProcessImplBase::setWarnings(int warnings)
{
    if (m_warnings == warnings)
    {
        return;
    }

    m_warnings = warnings;

    this->debug("Warnings changed: " + QString::number(m_warnings));

    emit this->warningsChanged(m_warnings);
}

void ProcessImplBase::setSkipped(int skipped)
{
    if (m_skipped == skipped)
    {
        return;
    }

    m_skipped = skipped;

    this->debug("Skipped changed: " + QString::number(m_skipped));

    emit this->skippedChanged(m_skipped);
}

void ProcessImplBase::setOverwritten(int overwritten)
{
    if (m_overwritten == overwritten)
    {
        return;
    }

    m_overwritten = overwritten;

    this->debug("Overwritten changed: " + QString::number(m_overwritten));

    emit this->overwrittenChanged(m_overwritten);
}

void ProcessImplBase::setProcessed(int processed)
{
    if (m_processed == processed)
    {
        return;
    }

    m_processed = processed;

    this->debug("Processed changed: " + QString::number(m_processed));

    emit this->processedChanged(m_processed);
}

void ProcessImplBase::setCurrentInputFile(const QString &currentInputFile)
{
    if (m_currentInputFile == currentInputFile)
    {
        return;
    }

    m_currentInputFile = currentInputFile;

    this->debug("Current input file changed: " + m_currentInputFile);

    emit this->currentInputFileChanged(m_currentInputFile);
}

bool ProcessImplBase::readKeyFromFile()
{
    QDir application_directory(m_applicationDirPath);
    QString key_file_name = application_directory.filePath(m_AES_KEY_FILE_NAME);
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
        this->error("The encryption key is empty");

        return false;
    }

    return true;
}

bool ProcessImplBase::readIvFromFile()
{
    QDir application_directory(m_applicationDirPath);
    QString iv_file_name = application_directory.filePath(m_AES_IV_FILE_NAME);
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

bool ProcessImplBase::processStateCheckpoint()
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

void ProcessImplBase::clearSecrets()
{
    // Clear the key.
    for (size_t i = 0; i < m_key.size(); i++)
    {
        m_key[i] = '\0';
    }

    // Clear the initialization vector.
    for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
    {
        m_iv[i] = '\0';
    }
}

void ProcessImplBase::setStateDescription(const QString &stateDescription)
{
    if (m_stateDescription == stateDescription)
    {
        return;
    }

    m_stateDescription = stateDescription;

    this->debug("State description changed: " + m_stateDescription);

    emit this->stateDescriptionChanged(m_stateDescription);
}

void ProcessImplBase::setProgressString(const QString &progressString)
{
    if (m_progressString == progressString)
    {
        return;
    }

    m_progressString = progressString;

    //this->debug("Progress string changed: " + m_progressString);

    emit this->progressStringChanged(m_progressString);
}

void ProcessImplBase::setProcessedBytesString(const QString &processedBytesString)
{
    if (m_processedBytesString == processedBytesString)
    {
        return;
    }

    m_processedBytesString = processedBytesString;

    //this->debug("Processed bytes string changed: " + m_processedBytesString);

    emit this->processedBytesStringChanged(m_processedBytesString);
}

void ProcessImplBase::setBytesToProcessString(const QString &bytesToProcessString)
{
    if (m_bytesToProcessString == bytesToProcessString)
    {
        return;
    }

    m_bytesToProcessString = bytesToProcessString;

    this->debug("Bytes to encrypt string changed: " + m_bytesToProcessString);

    emit this->bytesToProcessStringChanged(m_bytesToProcessString);
}

void ProcessImplBase::onStateChanged(Enums::ProcessState state)
{
    this->setStateDescription(Utils::processStateToString(state));
}

void ProcessImplBase::onProgressChanged(float progress)
{
    this->setProgressString(Utils::progressToString(progress));
}

void ProcessImplBase::onProcessedBytesChanged(unsigned long long processedBytes)
{
    this->setProcessedBytesString(Utils::bytesToString(processedBytes));
    this->setProgress(processedBytes / static_cast<float>(m_bytesToProcess));
}

void ProcessImplBase::onBytesToProcessChanged(unsigned long long bytesToProcess)
{
    this->setBytesToProcessString(Utils::bytesToString(bytesToProcess));
}
