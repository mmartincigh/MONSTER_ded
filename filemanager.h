#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QUrl>

#include "base.h"

class FileManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(int inputFilesCount READ inputFilesCount NOTIFY inputFilesCountChanged)
    Q_PROPERTY(bool overwriteOutputFiles READ overwriteOutputFiles WRITE setOverwriteOutputFiles NOTIFY overwriteOutputFilesChanged)

private:
    static const QStringList m_NAME_FILTERS;
    QStringList m_inputFiles;
    int m_inputFilesCount;
    bool m_overwriteOutputFiles;

public:
    explicit FileManager(QObject *parent = NULL);
    ~FileManager();

public:
    void initialize();
    QStringList inputFiles() const;
    int inputFilesCount() const;
    bool overwriteOutputFiles() const;
    void setOverwriteOutputFiles(bool overwriteOutputFiles);

private:
    void setInputFiles(const QStringList &inputFiles);
    void setInputFilesCount(int inputFilesCount);

public slots:
    void onSourcePathChanged(const QString &sourcePath);
    void onInputFiles(QStringList *inputFiles);
    void onOverwriteOutputFiles(bool *overwriteOutputFiles);
    void onReloadInputFiles();
    void onReloadInputFiles(const QString &sourcePath);
    void onReloadInputFiles(const QUrl &sourcePathUrl);

signals:
    void inputFilesChanged(const QStringList &inputfiles);
    void inputFilesCountChanged(bool inputFilesCount);
    void overwriteOutputFilesChanged(bool overwriteOutputFiles);
    void sourcePath(QString *sourcePath);
};

#endif // FILEMANAGER_H
