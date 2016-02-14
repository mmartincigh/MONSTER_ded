// Qt
#include <QDir>

// Local
#include "secureFileManager.h"
#include "utils.h"

const QStringList SecureFileManager::m_NAME_FILTERS("*.mef");

SecureFileManager::SecureFileManager(QObject *parent) :
    Base("SCRFM", parent),
    m_inputFiles(),
    m_inputFilesCount(0),
    m_overwriteOutputFiles(false)
{
    this->debug("Secure file manager created");
}

SecureFileManager::~SecureFileManager()
{
    this->debug("Secure file manager disposed of");
}

void SecureFileManager::initialize()
{
    this->debug("Initialized");
}

QStringList SecureFileManager::inputFiles() const
{
    return m_inputFiles;
}

int SecureFileManager::inputFilesCount() const
{
    return m_inputFilesCount;
}

bool SecureFileManager::overwriteOutputFiles() const
{
    return m_overwriteOutputFiles;
}

void SecureFileManager::setOverwriteOutputFiles(bool overwriteOutputFiles)
{
    if (m_overwriteOutputFiles == overwriteOutputFiles)
    {
        return;
    }

    m_overwriteOutputFiles = overwriteOutputFiles;

    this->debug("Overwrite output files changed: " + QString(m_overwriteOutputFiles ? "true" : "false"));

    emit this->overwriteOutputFilesChanged(m_overwriteOutputFiles);
}

void SecureFileManager::setInputFiles(const QStringList &inputFiles)
{
    m_inputFiles = inputFiles;

    this->debug("Input files changed");

    emit this->inputFilesChanged(m_inputFiles);
}

void SecureFileManager::setInputFilesCount(int inputFilesCount)
{
    if (m_inputFilesCount == inputFilesCount)
    {
        return;
    }

    m_inputFilesCount = inputFilesCount;

    this->debug("Input files count changed: " + QString::number(m_inputFilesCount));

    emit this->inputFilesCountChanged(m_inputFilesCount);
}

void SecureFileManager::onSecurePathChanged(const QString &securePath)
{
    this->debug("Secure path: " + securePath);
    if (securePath.isEmpty())
    {
        this->error("Secure path is empty");

        return;
    }

    this->onReloadInputFiles(securePath);
}

void SecureFileManager::onInputFiles(QStringList *inputFiles)
{
    if (inputFiles == NULL)
    {
        return;
    }

    *inputFiles = m_inputFiles;
}

void SecureFileManager::onOverwriteOutputFiles(bool *overwriteOutputFiles)
{
    if (overwriteOutputFiles == NULL)
    {
        return;
    }

    *overwriteOutputFiles = m_overwriteOutputFiles;
}

void SecureFileManager::onReloadInputFiles()
{
    QString secure_path;
    emit this->securePath(&secure_path);
    this->debug("Secure path: " + secure_path);
    if (secure_path.isEmpty())
    {
        this->error("Secure path is empty");

        return;
    }

    this->onReloadInputFiles(secure_path);
}
void SecureFileManager::onReloadInputFiles(const QString &securePath)
{
    if (securePath.isEmpty())
    {
        this->error("Secure path is empty");

        return;
    }

    // Enumerate the contents of the secure path.
    this->debug("Secure path: " + securePath);
    QDir secure_directory(securePath);
    secure_directory.setNameFilters(m_NAME_FILTERS);
    secure_directory.setFilter(QDir::Files);
    secure_directory.setSorting(QDir::Name);
    this->setInputFiles(secure_directory.entryList());
    this->setInputFilesCount(m_inputFiles.size());

    if (m_inputFilesCount == 0)
    {
        this->warning("The secure path contains no input files");

        return;
    }
}
void SecureFileManager::onReloadInputFiles(const QUrl &securePathUrl)
{
    QString secure_path = Utils::urlToString(securePathUrl);
    this->onReloadInputFiles(secure_path);
}
