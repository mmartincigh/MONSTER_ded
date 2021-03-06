#ifndef FILEMANAGERBASE_H
#define FILEMANAGERBASE_H

// Qt
#include <QUrl>

// Local
#include "ifilemanager.h"

class FileManagerBase : public IFileManager
{
    Q_OBJECT
    Q_PROPERTY(int inputFilesCount READ inputFilesCount NOTIFY inputFilesCountChanged)
    Q_PROPERTY(bool overwriteOutputFiles READ overwriteOutputFiles WRITE setOverwriteOutputFiles NOTIFY overwriteOutputFilesChanged)

private:
    QStringList m_nameFilters;
    QStringList m_inputFiles;
    int m_inputFilesCount;
    bool m_overwriteOutputFiles;

public:
    explicit FileManagerBase(const QStringList &nameFilters, const QString &logTag, QObject *parent = NULL);
    ~FileManagerBase();

public:
    virtual QStringList inputFiles() const;
    virtual int inputFilesCount() const;
    virtual bool overwriteOutputFiles() const;
    virtual void setOverwriteOutputFiles(bool overwriteOutputFiles);

private:
    void setInputFiles(const QStringList &inputFiles);
    void setInputFilesCount(int inputFilesCount);

public slots:
    void onPathChanged(const QString &path);
    void onInputFiles(QStringList *inputFiles);
    void onOverwriteOutputFiles(bool *overwriteOutputFiles);
    void onReloadInputFiles();
    void onReloadInputFiles(const QString &sourcePath);
    void onReloadInputFiles(const QUrl &sourcePathUrl);

signals:
    void inputFilesChanged(const QStringList &inputfiles);
    void inputFilesCountChanged(bool inputFilesCount);
    void overwriteOutputFilesChanged(bool overwriteOutputFiles);
    void path(QString *path);
};

#endif // FILEMANAGERBASE_H
