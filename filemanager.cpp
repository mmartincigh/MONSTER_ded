// Qt
#include <QDir>

// Local
#include "filemanager.h"
#include "utils.h"

const QStringList FileManager::m_NAME_FILTERS("*");

FileManager::FileManager(QObject *parent) :
    Base("FM", parent),
    m_inputFiles(),
    m_inputFilesCount(0),
    m_overwriteOutputFiles(false)
{
    this->debug("File manager created");
}

FileManager::~FileManager()
{
    this->debug("File manager disposed of");
}

void FileManager::initialize()
{
    this->debug("Initialized");
}

QStringList FileManager::inputFiles() const
{
    return m_inputFiles;
}

int FileManager::inputFilesCount() const
{
    return m_inputFilesCount;
}

bool FileManager::overwriteOutputFiles() const
{
    return m_overwriteOutputFiles;
}

void FileManager::setOverwriteOutputFiles(bool overwriteOutputFiles)
{
    if (m_overwriteOutputFiles == overwriteOutputFiles)
    {
        return;
    }

    m_overwriteOutputFiles = overwriteOutputFiles;

    this->debug("Overwrite output files changed: " + QString(m_overwriteOutputFiles ? "true" : "false"));

    emit this->overwriteOutputFilesChanged(m_overwriteOutputFiles);
}

void FileManager::setInputFiles(const QStringList &inputFiles)
{
    m_inputFiles = inputFiles;

    this->debug("Input files changed");

    emit this->inputFilesChanged(m_inputFiles);
}

void FileManager::setInputFilesCount(int inputFilesCount)
{
    if (m_inputFilesCount == inputFilesCount)
    {
        return;
    }

    m_inputFilesCount = inputFilesCount;

    this->debug("Input files count changed: " + QString::number(m_inputFilesCount));

    emit this->inputFilesCountChanged(m_inputFilesCount);
}

void FileManager::onSourcePathChanged(const QString &sourcePath)
{
    this->debug("Source path: " + sourcePath);
    if (sourcePath.isEmpty())
    {
        this->error("Source path is empty");

        return;
    }

    this->onReloadInputFiles(sourcePath);
}

void FileManager::onInputFiles(QStringList *inputFiles)
{
    if (inputFiles == NULL)
    {
        return;
    }

    *inputFiles = m_inputFiles;
}

void FileManager::onOverwriteOutputFiles(bool *overwriteOutputFiles)
{
    if (overwriteOutputFiles == NULL)
    {
        return;
    }

    *overwriteOutputFiles = m_overwriteOutputFiles;
}

void FileManager::onReloadInputFiles()
{
    QString source_path;
    emit this->sourcePath(&source_path);
    this->debug("Source path: " + source_path);
    if (source_path.isEmpty())
    {
        this->error("Source path is empty");

        return;
    }

    this->onReloadInputFiles(source_path);
}

void FileManager::onReloadInputFiles(const QString &sourcePath)
{
    if (sourcePath.isEmpty())
    {
        this->error("Source path is empty");

        return;
    }

    // Enumerate the contents of the source path.
    this->debug("Source path: " + sourcePath);
    QDir source_directory(sourcePath);
    source_directory.setNameFilters(m_NAME_FILTERS);
    source_directory.setFilter(QDir::Files);
    source_directory.setSorting(QDir::Name);
    this->setInputFiles(source_directory.entryList());
    this->setInputFilesCount(m_inputFiles.size());

    if (m_inputFilesCount == 0)
    {
        this->warning("The source path contains no input files");

        return;
    }
}

void FileManager::onReloadInputFiles(const QUrl &sourcePathUrl)
{
    if (sourcePathUrl.isEmpty())
    {
        this->error("Source path URL is empty");

        return;
    }

    // Enumerate the contents of the source path.
    QString source_path = Utils::urlToString(sourcePathUrl);
    this->debug("Source path: " + source_path);
    QDir source_directory(source_path);
    source_directory.setNameFilters(m_NAME_FILTERS);
    source_directory.setFilter(QDir::Files);
    source_directory.setSorting(QDir::Name);
    this->setInputFiles(source_directory.entryList());
    this->setInputFilesCount(m_inputFiles.size());

    if (m_inputFilesCount == 0)
    {
        this->warning("The source path contains no input files");

        return;
    }
}
