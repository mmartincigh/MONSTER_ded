#include "encryptionmanager.h"

EncryptionManager::EncryptionManager(QObject *parent) :
    Base("EM", parent),
    m_mutex(),
    m_waitCondition(),
    m_encryptionManagerImplThread(this),
    m_encryptionManagerImplSptr(new EncryptionManagerImpl(&m_mutex, &m_waitCondition))
{
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(isEnabledChanged(bool)), this, SIGNAL(isEnabledChanged(bool)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(stateChanged(Enums::State)), this, SIGNAL(stateChanged(Enums::State)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(stateDescriptionChanged(QString)), this, SIGNAL(stateDescriptionChanged(QString)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(progressChanged(float)), this, SIGNAL(progressChanged(float)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(progressStringChanged(QString)), this, SIGNAL(progressStringChanged(QString)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(errorsChanged(int)), this, SIGNAL(errorsChanged(int)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(warningsChanged(int)), this, SIGNAL(warningsChanged(int)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(skippedChanged(int)), this, SIGNAL(skippedChanged(int)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(overwrittenChanged(int)), this, SIGNAL(overwrittenChanged(int)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(processedChanged(int)), this, SIGNAL(processedChanged(int)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(currentInputFileChanged(QString)), this, SIGNAL(currentInputFileChanged(QString)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(isSourcePathUrlValid(bool*)), this, SIGNAL(isSourcePathUrlValid(bool*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(isDestinationPathUrlValid(bool*)), this, SIGNAL(isDestinationPathUrlValid(bool*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(sourcePath(QString*)), this, SIGNAL(sourcePath(QString*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(destinationPath(QString*)), this, SIGNAL(destinationPath(QString*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(videoFiles(QStringList*)), this, SIGNAL(videoFiles(QStringList*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_encryptionManagerImplSptr.data(), SIGNAL(overwriteOutputFiles(bool*)), this, SIGNAL(overwriteOutputFiles(bool*)), Qt::BlockingQueuedConnection);

    this->debug("Thumbnail generator created");
}

EncryptionManager::~EncryptionManager()
{
    this->uninitialize();
    this->onStop();

    m_encryptionManagerImplThread.quit();
    m_encryptionManagerImplThread.wait(3000);

    this->debug("Encryption manager disposed of");
}

void EncryptionManager::initialize()
{
    m_encryptionManagerImplSptr.data()->initialize();
    m_encryptionManagerImplSptr.data()->moveToThread(&m_encryptionManagerImplThread);

    m_encryptionManagerImplThread.start();

    this->debug("Initialized");
}

void EncryptionManager::uninitialize()
{
    // Disconnect all the encryption manager implementation's signals to avoid deadlocks during shut down.
    QObject::disconnect(m_encryptionManagerImplSptr.data(), NULL, NULL, NULL);

    this->debug("Uninitialized");
}

bool EncryptionManager::isEnabled() const
{
    return m_encryptionManagerImplSptr.data()->isEnabled();
}

Enums::State EncryptionManager::state() const
{
    return m_encryptionManagerImplSptr.data()->state();
}

QString EncryptionManager::stateDescription() const
{
    return m_encryptionManagerImplSptr.data()->stateDescription();
}

float EncryptionManager::progress() const
{
    return m_encryptionManagerImplSptr.data()->progress();
}

QString EncryptionManager::progressString() const
{
    return m_encryptionManagerImplSptr.data()->progressString();
}

int EncryptionManager::errors() const
{
    return m_encryptionManagerImplSptr.data()->errors();
}

int EncryptionManager::warnings() const
{
    return m_encryptionManagerImplSptr.data()->warnings();
}

int EncryptionManager::skipped() const
{
    return m_encryptionManagerImplSptr.data()->skipped();
}

int EncryptionManager::overwritten() const
{
    return m_encryptionManagerImplSptr.data()->overwritten();
}

int EncryptionManager::processed() const
{
    return m_encryptionManagerImplSptr.data()->processed();
}

QString EncryptionManager::currentInputFile() const
{
    return m_encryptionManagerImplSptr.data()->currentInputFile();
}

void EncryptionManager::onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid)
{
    QMetaObject::invokeMethod(m_encryptionManagerImplSptr.data(), "onIsSourcePathUrlValidChanged", Qt::QueuedConnection, Q_ARG(bool, isSourcePathUrlValid));
}

void EncryptionManager::onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid)
{
    QMetaObject::invokeMethod(m_encryptionManagerImplSptr.data(), "onIsDestinationPathUrlValidChanged", Qt::QueuedConnection, Q_ARG(bool, isDestinationPathUrlValid));
}

void EncryptionManager::onEncryptFiles()
{
    QMetaObject::invokeMethod(m_encryptionManagerImplSptr.data(), "onEncryptFiles", Qt::QueuedConnection);
}

void EncryptionManager::onPause()
{
    m_mutex.lock();
    m_encryptionManagerImplSptr.data()->setPause(true);
    m_mutex.unlock();

    this->debug("Pause event set");
}

void EncryptionManager::onResume()
{
    m_mutex.lock();
    m_encryptionManagerImplSptr.data()->setPause(false);
    m_waitCondition.wakeAll();
    m_mutex.unlock();

    this->debug("Pause event reset");
}

void EncryptionManager::onStop()
{
    m_mutex.lock();
    m_encryptionManagerImplSptr.data()->setPause(false);
    m_encryptionManagerImplSptr.data()->setStop(true);
    m_waitCondition.wakeAll();
    m_mutex.unlock();

    this->debug("Stop event set");
}
