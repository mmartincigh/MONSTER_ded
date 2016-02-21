// Qt
#include <QFileInfo>
#include <QDir>

// Local
#include "applicationmanager.h"
#include "utils.h"

ApplicationManager::ApplicationManager(const QString &applicationDirPath, const QStringList &arguments, QObject *parent) :
    Base("AM", parent),
    m_sourcePathManager(this),
    m_securePathManager(this),
    m_destinationPathManager(this),
    m_sourceFileManager(QStringList("*"), this),
    m_secureFileManager(QStringList(Utils::MEF_NAME_FILTER), this),
    m_encryptionManager(applicationDirPath, this),
    m_decryptionManager(applicationDirPath, this),
    m_processManager(this),
    m_statsManager(this),
    m_settingsManager(this),
    m_windowManager(this),
    m_arguments(arguments)
{
    QObject::connect(&m_sourcePathManager, SIGNAL(pathModel(QStringList*)), &m_settingsManager, SLOT(onSourcePathModel(QStringList*)));
    QObject::connect(&m_sourcePathManager, SIGNAL(pathModelChanged(QStringList)), &m_settingsManager, SLOT(onSourcePathModelChanged(QStringList)));
    QObject::connect(&m_sourcePathManager, SIGNAL(pathChanged(QString)), &m_sourceFileManager, SLOT(onPathChanged(QString)));
    QObject::connect(&m_sourcePathManager, SIGNAL(isPathUrlValidChanged(bool)), &m_encryptionManager, SLOT(onIsSourcePathUrlValidChanged(bool)));

    QObject::connect(&m_securePathManager, SIGNAL(pathModel(QStringList*)), &m_settingsManager, SLOT(onSecurePathModel(QStringList*)));
    QObject::connect(&m_securePathManager, SIGNAL(pathModelChanged(QStringList)), &m_settingsManager, SLOT(onSecurePathModelChanged(QStringList)));
    QObject::connect(&m_securePathManager, SIGNAL(pathChanged(QString)), &m_secureFileManager, SLOT(onPathChanged(QString)));
    QObject::connect(&m_securePathManager, SIGNAL(isPathUrlValidChanged(bool)), &m_encryptionManager, SLOT(onIsSecurePathUrlValidChanged(bool)));
    QObject::connect(&m_securePathManager, SIGNAL(isPathUrlValidChanged(bool)), &m_decryptionManager, SLOT(onIsSecurePathUrlValidChanged(bool)));

    QObject::connect(&m_destinationPathManager, SIGNAL(pathModel(QStringList*)), &m_settingsManager, SLOT(onDestinationPathModel(QStringList*)));
    QObject::connect(&m_destinationPathManager, SIGNAL(pathModelChanged(QStringList)), &m_settingsManager, SLOT(onDestinationPathModelChanged(QStringList)));
    QObject::connect(&m_destinationPathManager, SIGNAL(isPathUrlValidChanged(bool)), &m_decryptionManager, SLOT(onIsDestinationPathUrlValidChanged(bool)));

    QObject::connect(&m_sourceFileManager, SIGNAL(path(QString*)), &m_sourcePathManager, SLOT(onPath(QString*)));

    QObject::connect(&m_secureFileManager, SIGNAL(path(QString*)), &m_securePathManager, SLOT(onPath(QString*)));

    QObject::connect(&m_encryptionManager, SIGNAL(stateChanged(Enums::ProcessState)), &m_processManager, SLOT(onEncryptionStateChanged(Enums::ProcessState)));
    QObject::connect(&m_encryptionManager, SIGNAL(progressChanged(float)), &m_processManager, SLOT(onEncryptionProgressChanged(float)));
    QObject::connect(&m_encryptionManager, SIGNAL(processedBytesChanged(unsigned long long)), &m_statsManager, SLOT(onProcessedBytesChanged(unsigned long long)));
    QObject::connect(&m_encryptionManager, SIGNAL(bytesToProcessChanged(unsigned long long)), &m_statsManager, SLOT(onBytesToProcessChanged(unsigned long long)));
    QObject::connect(&m_encryptionManager, SIGNAL(errorsChanged(int)), &m_statsManager, SLOT(onErrorsChanged(int)));
    QObject::connect(&m_encryptionManager, SIGNAL(warningsChanged(int)), &m_statsManager, SLOT(onWarningsChanged(int)));
    QObject::connect(&m_encryptionManager, SIGNAL(skippedChanged(int)), &m_statsManager, SLOT(onSkippedChanged(int)));
    QObject::connect(&m_encryptionManager, SIGNAL(overwrittenChanged(int)), &m_statsManager, SLOT(onOverwrittenChanged(int)));
    QObject::connect(&m_encryptionManager, SIGNAL(processedChanged(int)), &m_statsManager, SLOT(onProcessedChanged(int)));
    QObject::connect(&m_encryptionManager, SIGNAL(currentInputFileChanged(QString)), &m_statsManager, SLOT(onCurrentInputFileChanged(QString)));
    QObject::connect(&m_encryptionManager, SIGNAL(isSourcePathUrlValid(bool*)), &m_sourcePathManager, SLOT(onIsPathUrlValid(bool*)));
    QObject::connect(&m_encryptionManager, SIGNAL(isSecurePathUrlValid(bool*)), &m_securePathManager, SLOT(onIsPathUrlValid(bool*)));
    QObject::connect(&m_encryptionManager, SIGNAL(sourcePath(QString*)), &m_sourcePathManager, SLOT(onPath(QString*)));
    QObject::connect(&m_encryptionManager, SIGNAL(securePath(QString*)), &m_securePathManager, SLOT(onPath(QString*)));
    QObject::connect(&m_encryptionManager, SIGNAL(inputFiles(QStringList*)), &m_sourceFileManager, SLOT(onInputFiles(QStringList*)));
    QObject::connect(&m_encryptionManager, SIGNAL(overwriteOutputFiles(bool*)), &m_sourceFileManager, SLOT(onOverwriteOutputFiles(bool*)));

    QObject::connect(&m_decryptionManager, SIGNAL(stateChanged(Enums::ProcessState)), &m_processManager, SLOT(onDecryptionStateChanged(Enums::ProcessState)));
    QObject::connect(&m_decryptionManager, SIGNAL(progressChanged(float)), &m_processManager, SLOT(onDecryptionProgressChanged(float)));
    QObject::connect(&m_decryptionManager, SIGNAL(processedBytesChanged(unsigned long long)), &m_statsManager, SLOT(onProcessedBytesChanged(unsigned long long)));
    QObject::connect(&m_decryptionManager, SIGNAL(bytesToProcessChanged(unsigned long long)), &m_statsManager, SLOT(onBytesToProcessChanged(unsigned long long)));
    QObject::connect(&m_decryptionManager, SIGNAL(errorsChanged(int)), &m_statsManager, SLOT(onErrorsChanged(int)));
    QObject::connect(&m_decryptionManager, SIGNAL(warningsChanged(int)), &m_statsManager, SLOT(onWarningsChanged(int)));
    QObject::connect(&m_decryptionManager, SIGNAL(skippedChanged(int)), &m_statsManager, SLOT(onSkippedChanged(int)));
    QObject::connect(&m_decryptionManager, SIGNAL(overwrittenChanged(int)), &m_statsManager, SLOT(onOverwrittenChanged(int)));
    QObject::connect(&m_decryptionManager, SIGNAL(processedChanged(int)), &m_statsManager, SLOT(onProcessedChanged(int)));
    QObject::connect(&m_decryptionManager, SIGNAL(currentInputFileChanged(QString)), &m_statsManager, SLOT(onCurrentInputFileChanged(QString)));
    QObject::connect(&m_decryptionManager, SIGNAL(isSecurePathUrlValid(bool*)), &m_securePathManager, SLOT(onIsPathUrlValid(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(isDestinationPathUrlValid(bool*)), &m_destinationPathManager, SLOT(onIsPathUrlValid(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(securePath(QString*)), &m_securePathManager, SLOT(onPath(QString*)));
    QObject::connect(&m_decryptionManager, SIGNAL(destinationPath(QString*)), &m_destinationPathManager, SLOT(onPath(QString*)));
    QObject::connect(&m_decryptionManager, SIGNAL(inputFiles(QStringList*)), &m_secureFileManager, SLOT(onInputFiles(QStringList*)));
    QObject::connect(&m_decryptionManager, SIGNAL(overwriteOutputFiles(bool*)), &m_secureFileManager, SLOT(onOverwriteOutputFiles(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(openFile(QString)), &m_processManager, SLOT(onOpenFile(QString)));

    QObject::connect(&m_processManager, SIGNAL(openFile(bool*)), &m_settingsManager, SLOT(onOpenFile(bool*)));
    QObject::connect(&m_processManager, SIGNAL(openFileChanged(bool)), &m_settingsManager, SLOT(onOpenFileChanged(bool)));
    QObject::connect(&m_processManager, SIGNAL(pauseEncryption()), &m_encryptionManager, SLOT(onPause()));
    QObject::connect(&m_processManager, SIGNAL(pauseDecryption()), &m_decryptionManager, SLOT(onPause()));
    QObject::connect(&m_processManager, SIGNAL(resumeEncryption()), &m_encryptionManager, SLOT(onResume()));
    QObject::connect(&m_processManager, SIGNAL(resumeDecryption()), &m_decryptionManager, SLOT(onResume()));
    QObject::connect(&m_processManager, SIGNAL(stopEncryption()), &m_encryptionManager, SLOT(onStop()));
    QObject::connect(&m_processManager, SIGNAL(stopDecryption()), &m_decryptionManager, SLOT(onStop()));

    this->debug("Application manager created");
}

ApplicationManager::~ApplicationManager()
{
    this->debug("Application manager disposed of");
}

void ApplicationManager::initialize()
{
    m_sourcePathManager.initialize();
    m_securePathManager.initialize();
    m_destinationPathManager.initialize();
    m_encryptionManager.initialize();
    m_decryptionManager.initialize();
    m_processManager.initialize();

    this->debug("Initialized");
}

SourcePathManager *ApplicationManager::sourcePathManager()
{
    return &m_sourcePathManager;
}

SecurePathManager *ApplicationManager::securePathManager()
{
    return &m_securePathManager;
}

DestinationPathManager *ApplicationManager::destinationPathManager()
{
    return &m_destinationPathManager;
}

SourceFileManager *ApplicationManager::sourceFileManager()
{
    return &m_sourceFileManager;
}

SecureFileManager *ApplicationManager::secureFileManager()
{
    return &m_secureFileManager;
}

EncryptionManager *ApplicationManager::encryptionManager()
{
    return &m_encryptionManager;
}

DecryptionManager *ApplicationManager::decryptionManager()
{
    return &m_decryptionManager;
}

ProcessManager *ApplicationManager::processManager()
{
    return &m_processManager;
}

StatsManager *ApplicationManager::statsManager()
{
    return &m_statsManager;
}

WindowManager *ApplicationManager::windowManager()
{
    return &m_windowManager;
}

void ApplicationManager::parseArguments()
{
    this->debug("Arguments: " + m_arguments.join('|'));

    // Check if there are extra arguments.
    if (m_arguments.length() == 1)
    {
        // No extra arguments.

        return;
    }

    // Consider only the second argument.
    QString argument(m_arguments.at(1));

    // Check if the argument is a valid file name.
    QFileInfo argument_file_info(argument);
    if (!argument_file_info.exists())
    {
        // Not a valid file name.

        return;
    }

    // Check whether the argument is an encrypted file or a regular file.
    QString argument_extension(argument_file_info.suffix());
    if (argument_extension == Utils::MEF_EXTENSION)
    {
        // Encrypted file.
        this->debug("Encrypted file passed as an argument: " + argument);

        // Decrypt the file.
        QMetaObject::invokeMethod(&m_decryptionManager, "onProcess", Qt::QueuedConnection, Q_ARG(QString, argument));
    }
    else
    {
        // Regular file.
        this->debug("Regular file passed as an argument: " + argument);

        // Encrypt the file.
        QMetaObject::invokeMethod(&m_encryptionManager, "onProcess", Qt::QueuedConnection, Q_ARG(QString, argument));
    }
}

void ApplicationManager::onGuiCompleted()
{
    this->debug("GUI Completed");

    this->parseArguments();
}

void ApplicationManager::onMessageReceived(const QString &message)
{
    this->debug("Message received: " + message);

    // Regardless the contents of the received message, bring the application's window to front.
    m_windowManager.bringToFront();

    // Check if the argument is a valid file name.
    QFileInfo argument_file_info(message);
    if (!argument_file_info.exists())
    {
        // Not a valid file name.

        return;
    }

    // Check whether the argument is an encrypted file or a regular file.
    QString argument_extension(argument_file_info.suffix());
    if (argument_extension == Utils::MEF_EXTENSION)
    {
        // Encrypted file.
        this->debug("Encrypted file passed from another instance: " + message);

        // Decrypt the file.
        QMetaObject::invokeMethod(&m_decryptionManager, "onProcess", Qt::QueuedConnection, Q_ARG(QString, message));
    }
    else
    {
        // Regular file.
        this->debug("Regular file passed from another instance: " + message);

        // Encrypt the file.
        QMetaObject::invokeMethod(&m_encryptionManager, "onProcess", Qt::QueuedConnection, Q_ARG(QString, message));
    }
}
