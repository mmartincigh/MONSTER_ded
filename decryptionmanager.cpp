// Local
#include "decryptionmanager.h"

DecryptionManager::DecryptionManager(const QString &applicationDirPath, QObject *parent) :
    IProcess("DM", parent),
    m_mutex(),
    m_waitCondition(),
    m_decryptionManagerImplThread(this),
    m_decryptionManagerImplSptr(new DecryptionManagerImpl(applicationDirPath, &m_mutex, &m_waitCondition))
{
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(isEnabledChanged(bool)), this, SIGNAL(isEnabledChanged(bool)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(stateChanged(Enums::ProcessState)), this, SIGNAL(stateChanged(Enums::ProcessState)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(stateDescriptionChanged(QString)), this, SIGNAL(stateDescriptionChanged(QString)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(progressChanged(float)), this, SIGNAL(progressChanged(float)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(progressStringChanged(QString)), this, SIGNAL(progressStringChanged(QString)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(processedBytesChanged(unsigned long long)), this, SIGNAL(processedBytesChanged(unsigned long long)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(processedBytesStringChanged(QString)), this, SIGNAL(processedBytesStringChanged(QString)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(bytesToProcessChanged(unsigned long long)), this, SIGNAL(bytesToProcessChanged(unsigned long long)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(bytesToProcessStringChanged(QString)), this, SIGNAL(bytesToProcessStringChanged(QString)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(errorsChanged(int)), this, SIGNAL(errorsChanged(int)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(warningsChanged(int)), this, SIGNAL(warningsChanged(int)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(skippedChanged(int)), this, SIGNAL(skippedChanged(int)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(overwrittenChanged(int)), this, SIGNAL(overwrittenChanged(int)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(processedChanged(int)), this, SIGNAL(processedChanged(int)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(currentInputFileChanged(QString)), this, SIGNAL(currentInputFileChanged(QString)));
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(isSecurePathUrlValid(bool*)), this, SIGNAL(isSecurePathUrlValid(bool*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(isDestinationPathUrlValid(bool*)), this, SIGNAL(isDestinationPathUrlValid(bool*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(securePath(QString*)), this, SIGNAL(securePath(QString*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(destinationPath(QString*)), this, SIGNAL(destinationPath(QString*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(inputFiles(QStringList*)), this, SIGNAL(inputFiles(QStringList*)), Qt::BlockingQueuedConnection);
    QObject::connect(m_decryptionManagerImplSptr.data(), SIGNAL(overwriteOutputFiles(bool*)), this, SIGNAL(overwriteOutputFiles(bool*)), Qt::BlockingQueuedConnection);

    this->debug("Decryption manager created");
}

DecryptionManager::~DecryptionManager()
{
    this->uninitialize();
    this->onStop();

    m_decryptionManagerImplThread.quit();
    m_decryptionManagerImplThread.wait(3000);

    this->debug("Decryption manager disposed of");
}

void DecryptionManager::initialize()
{
    m_decryptionManagerImplSptr.data()->moveToThread(&m_decryptionManagerImplThread);

    m_decryptionManagerImplThread.start();

    this->debug("Initialized");
}

void DecryptionManager::uninitialize()
{
    // Disconnect all the signals of the decryption manager implementation to avoid deadlocks during shut down.
    QObject::disconnect(m_decryptionManagerImplSptr.data(), NULL, NULL, NULL);

    this->debug("Uninitialized");
}

bool DecryptionManager::isEnabled() const
{
    return m_decryptionManagerImplSptr.data()->isEnabled();
}

Enums::ProcessState DecryptionManager::state() const
{
    return m_decryptionManagerImplSptr.data()->state();
}

QString DecryptionManager::stateDescription() const
{
    return m_decryptionManagerImplSptr.data()->stateDescription();
}

float DecryptionManager::progress() const
{
    return m_decryptionManagerImplSptr.data()->progress();
}

QString DecryptionManager::progressString() const
{
    return m_decryptionManagerImplSptr.data()->progressString();
}

unsigned long long DecryptionManager::processedBytes() const
{
    return m_decryptionManagerImplSptr.data()->processedBytes();
}

QString DecryptionManager::processedBytesString() const
{
    return m_decryptionManagerImplSptr.data()->processedBytesString();
}

unsigned long long DecryptionManager::bytesToProcess() const
{
    return m_decryptionManagerImplSptr.data()->bytesToProcess();
}

QString DecryptionManager::bytesToProcessString() const
{
    return m_decryptionManagerImplSptr.data()->bytesToProcessString();
}

int DecryptionManager::errors() const
{
    return m_decryptionManagerImplSptr.data()->errors();
}

int DecryptionManager::warnings() const
{
    return m_decryptionManagerImplSptr.data()->warnings();
}

int DecryptionManager::skipped() const
{
    return m_decryptionManagerImplSptr.data()->skipped();
}

int DecryptionManager::overwritten() const
{
    return m_decryptionManagerImplSptr.data()->overwritten();
}

int DecryptionManager::processed() const
{
    return m_decryptionManagerImplSptr.data()->processed();
}

QString DecryptionManager::currentInputFile() const
{
    return m_decryptionManagerImplSptr.data()->currentInputFile();
}

void DecryptionManager::onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid)
{
    QMetaObject::invokeMethod(m_decryptionManagerImplSptr.data(), "onIsSecurePathUrlValidChanged", Qt::QueuedConnection, Q_ARG(bool, isSecurePathUrlValid));
}

void DecryptionManager::onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid)
{
    QMetaObject::invokeMethod(m_decryptionManagerImplSptr.data(), "onIsDestinationPathUrlValidChanged", Qt::QueuedConnection, Q_ARG(bool, isDestinationPathUrlValid));
}

void DecryptionManager::onProcess()
{
    QMetaObject::invokeMethod(m_decryptionManagerImplSptr.data(), "onProcess", Qt::QueuedConnection);
}

void DecryptionManager::onPause()
{
    m_mutex.lock();
    m_decryptionManagerImplSptr.data()->setPause(true);
    m_mutex.unlock();

    this->debug("Pause event set");
}

void DecryptionManager::onResume()
{
    m_mutex.lock();
    m_decryptionManagerImplSptr.data()->setPause(false);
    m_waitCondition.wakeAll();
    m_mutex.unlock();

    this->debug("Pause event reset");
}

void DecryptionManager::onStop()
{
    m_mutex.lock();
    m_decryptionManagerImplSptr.data()->setPause(false);
    m_decryptionManagerImplSptr.data()->setStop(true);
    m_waitCondition.wakeAll();
    m_mutex.unlock();

    this->debug("Stop event set");
}

void DecryptionManager::onDecryptFile(const QString &fileToDecrypt)
{
    QMetaObject::invokeMethod(m_decryptionManagerImplSptr.data(), "onDecryptFile", Qt::QueuedConnection, Q_ARG(QString, fileToDecrypt));
}
