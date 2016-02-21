// Qt
#include <QDir>

// Local
#include "filemanagerbase.h"
#include "utils.h"

FileManagerBase::FileManagerBase(const QStringList &nameFilters, const QString &logTag, QObject *parent) :
    IFileManager(logTag, parent),
    m_nameFilters(nameFilters),
    m_inputFiles(),
    m_inputFilesCount(0),
    m_overwriteOutputFiles(false)
{
}

FileManagerBase::~FileManagerBase()
{
}

QStringList FileManagerBase::inputFiles() const
{
    return m_inputFiles;
}

int FileManagerBase::inputFilesCount() const
{
    return m_inputFilesCount;
}

bool FileManagerBase::overwriteOutputFiles() const
{
    return m_overwriteOutputFiles;
}

void FileManagerBase::setOverwriteOutputFiles(bool overwriteOutputFiles)
{
    if (m_overwriteOutputFiles == overwriteOutputFiles)
    {
        return;
    }

    m_overwriteOutputFiles = overwriteOutputFiles;

    this->debug("Overwrite output files changed: " + QString(m_overwriteOutputFiles ? "true" : "false"));

    emit this->overwriteOutputFilesChanged(m_overwriteOutputFiles);
}

void FileManagerBase::setInputFiles(const QStringList &inputFiles)
{
    if (m_inputFiles == inputFiles)
    {
        return;
    }

    m_inputFiles = inputFiles;

    this->debug("Input files changed");

    emit this->inputFilesChanged(m_inputFiles);
}

void FileManagerBase::setInputFilesCount(int inputFilesCount)
{
    if (m_inputFilesCount == inputFilesCount)
    {
        return;
    }

    m_inputFilesCount = inputFilesCount;

    this->debug("Input files count changed: " + QString::number(m_inputFilesCount));

    emit this->inputFilesCountChanged(m_inputFilesCount);
}

void FileManagerBase::onPathChanged(const QString &path)
{
    this->debug("Path: " + path);
    if (path.isEmpty())
    {
        this->error("Path is empty");

        return;
    }

    this->onReloadInputFiles(path);
}

void FileManagerBase::onInputFiles(QStringList *inputFiles)
{
    if (inputFiles == NULL)
    {
        return;
    }

    *inputFiles = m_inputFiles;
}

void FileManagerBase::onOverwriteOutputFiles(bool *overwriteOutputFiles)
{
    if (overwriteOutputFiles == NULL)
    {
        return;
    }

    *overwriteOutputFiles = m_overwriteOutputFiles;
}

void FileManagerBase::onReloadInputFiles()
{
    QString path;
    emit this->path(&path);
    this->debug("Path: " + path);
    if (path.isEmpty())
    {
        this->error("Path is empty");

        return;
    }

    this->onReloadInputFiles(path);
}
void FileManagerBase::onReloadInputFiles(const QString &path)
{
    if (path.isEmpty())
    {
        this->error("Path is empty");

        return;
    }

    // Enumerate the contents of the path.
    this->debug("Path: " + path);
    QDir directory(path);
    directory.setNameFilters(m_nameFilters);
    directory.setFilter(QDir::Files);
    directory.setSorting(QDir::Name);
    this->setInputFiles(directory.entryList());
    this->setInputFilesCount(m_inputFiles.size());

    if (m_inputFilesCount == 0)
    {
        this->warning("The path contains no input files");

        return;
    }
}
void FileManagerBase::onReloadInputFiles(const QUrl &pathUrl)
{
    QString path = Utils::urlToString(pathUrl);
    this->onReloadInputFiles(path);
}
