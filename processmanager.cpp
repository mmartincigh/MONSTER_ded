// Local
#include "processmanager.h"
#include "utils.h"

ProcessManager::ProcessManager(QObject *parent) :
    Base("PM", parent),
    m_isEnabled(false),
    m_type(Enums::ProcessType_None),
    m_typeDescription(Utils::processTypeToString(m_type)),
    m_state(Enums::ProcessState_Idle),
    m_stateDescription(Utils::processStateToString(m_state)),
    m_progress(0),
    m_progressString(Utils::progressToString(m_progress))
{
    QObject::connect(this, SIGNAL(typeChanged(Enums::ProcessType)), this, SLOT(onTypeChanged(Enums::ProcessType)));
    QObject::connect(this, SIGNAL(stateChanged(Enums::ProcessState)), this, SLOT(onStateChanged(Enums::ProcessState)));
    QObject::connect(this, SIGNAL(progressChanged(float)), this, SLOT(onProgressChanged(float)));

    this->debug("Process manager created");
}

ProcessManager::~ProcessManager()
{
    this->debug("Process manager disposed of");
}

bool ProcessManager::isEnabled() const
{
    return m_isEnabled;
}

Enums::ProcessType ProcessManager::type() const
{
    return m_type;
}

QString ProcessManager::typeDescription() const
{
    return m_typeDescription;
}

Enums::ProcessState ProcessManager::state() const
{
    return m_state;
}

QString ProcessManager::stateDescription() const
{
    return m_stateDescription;
}

float ProcessManager::progress() const
{
    return m_progress;
}

QString ProcessManager::progressString() const
{
    return m_progressString;
}

void ProcessManager::setIsEnabled(bool isEnabled)
{
    if (m_isEnabled == isEnabled)
    {
        return;
    }

    m_isEnabled = isEnabled;

    this->debug("Is enabled changed: " + QString(m_isEnabled ? "true" : "false"));

    emit this->isEnabledChanged(m_isEnabled);
}

void ProcessManager::setType(Enums::ProcessType type)
{
    if (m_type == type)
    {
        return;
    }

    m_type = type;

    this->debug("Type changed: " + QString::number(m_type));

    emit this->typeChanged(m_type);
}

void ProcessManager::setTypeDescription(const QString &typeDescription)
{
    if (m_typeDescription == typeDescription)
    {
        return;
    }

    m_typeDescription = typeDescription;

    this->debug("Type description changed: " + m_typeDescription);

    emit this->typeDescriptionChanged(m_typeDescription);
}

void ProcessManager::setState(Enums::ProcessState state)
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;

    this->debug("State changed: " + QString::number(m_state));

    emit this->stateChanged(m_state);
}

void ProcessManager::setStateDescription(const QString &stateDescription)
{
    if (m_stateDescription == stateDescription)
    {
        return;
    }

    m_stateDescription = stateDescription;

    this->debug("State description changed: " + m_stateDescription);

    emit this->stateDescriptionChanged(m_stateDescription);
}

void ProcessManager::setProgress(float progress)
{
    if (m_progress == progress)
    {
        return;
    }

    m_progress = progress;

    //this->debug("Progress changed: " + QString::number(m_progress));

    emit this->progressChanged(m_progress);
}

void ProcessManager::setProgressString(const QString &progressString)
{
    if (m_progressString == progressString)
    {
        return;
    }

    m_progressString = progressString;

    //this->debug("Progress string changed: " + m_progressString);

    emit this->progressStringChanged(m_progressString);
}

void ProcessManager::onEncryptionStateChanged(Enums::ProcessState encryptionState)
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->setType(Enums::ProcessType_Encryption);
        break;
    case Enums::ProcessType_Encryption:
        // Already an encryption process.
        break;
    case Enums::ProcessType_Decryption:
        this->error("Conflicting process types");
        return;
    default:
        this->error("Unknown process type");
        return;
    }

    this->setState(encryptionState);
}

void ProcessManager::onDecryptionStateChanged(Enums::ProcessState decryptionState)
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->setType(Enums::ProcessType_Decryption);
        break;
    case Enums::ProcessType_Encryption:
        this->error("Conflicting process types");
        return;
    case Enums::ProcessType_Decryption:
        // Already an decryption process.
        break;
    default:
        this->error("Unknown process type");
        return;
    }

    this->setState(decryptionState);
}

void ProcessManager::onEncryptionProgressChanged(float encryptionProgress)
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->setType(Enums::ProcessType_Encryption);
        break;
    case Enums::ProcessType_Encryption:
        // Already an encryption process.
        break;
    case Enums::ProcessType_Decryption:
        this->error("Conflicting process types");
        return;
    default:
        this->error("Unknown process type");
        return;
    }

    this->setProgress(encryptionProgress);
}

void ProcessManager::onDecryptionProgressChanged(float decryptionProgress)
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->setType(Enums::ProcessType_Decryption);
        break;
    case Enums::ProcessType_Encryption:
        this->error("Conflicting process types");
        return;
    case Enums::ProcessType_Decryption:
        // Already an decryption process.
        break;
    default:
        this->error("Unknown process type");
        return;
    }

    this->setProgress(decryptionProgress);
}

void ProcessManager::onPause()
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->warning("Pause requested on no process");
        return;
    case Enums::ProcessType_Encryption:
        // Pause the encryption process.
        emit this->pauseEncryption();
        break;
    case Enums::ProcessType_Decryption:
        // Pause the decryption process.
        emit this->pauseDecryption();
        break;
    default:
        this->error("Unkown process type");
        return;
    }
}

void ProcessManager::onResume()
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->warning("Resume requested on no process");
        return;
    case Enums::ProcessType_Encryption:
        // Resume the encryption process.
        emit this->resumeEncryption();
        break;
    case Enums::ProcessType_Decryption:
        // Resume the decryption process.
        emit this->resumeDecryption();
        break;
    default:
        this->error("Unkown process type");
        return;
    }
}

void ProcessManager::onStop()
{
    switch (m_type)
    {
    case Enums::ProcessType_None:
        this->warning("Stop requested on no process");
        return;
    case Enums::ProcessType_Encryption:
        // Stop the encryption process.
        emit this->stopEncryption();
        break;
    case Enums::ProcessType_Decryption:
        // Stop the decryption process.
        emit this->stopDecryption();
        break;
    default:
        this->error("Unkown process type");
        return;
    }
}

void ProcessManager::onTypeChanged(Enums::ProcessType type)
{
    this->setTypeDescription(Utils::processTypeToString(type));

    switch (type)
    {
    case Enums::ProcessType_None:
        this->setIsEnabled(false);
        break;
    case Enums::ProcessType_Encryption:
    case Enums::ProcessType_Decryption:
        this->setIsEnabled(true);
        break;
    default:
        this->error("Unkown process type");
        break;
    }
}

void ProcessManager::onStateChanged(Enums::ProcessState state)
{
    this->setStateDescription(Utils::processStateToString(state));
}

void ProcessManager::onProgressChanged(float progress)
{
    this->setProgressString(Utils::progressToString(progress));
}
