#ifndef SECUREFILEMANAGER_H
#define SECUREFILEMANAGER_H

// Qt
#include <QUrl>

// Local
#include "base.h"

class SecureFileManager : public Base
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
    explicit SecureFileManager(QObject *parent = NULL);
    ~SecureFileManager();

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
    void onSecurePathChanged(const QString &securePath);
    void onInputFiles(QStringList *inputFiles);
    void onOverwriteOutputFiles(bool *overwriteOutputFiles);
    void onReloadInputFiles();
    void onReloadInputFiles(const QString &securePath);
    void onReloadInputFiles(const QUrl &securePathUrl);

signals:
    void inputFilesChanged(const QStringList &inputfiles);
    void inputFilesCountChanged(bool inputFilesCount);
    void overwriteOutputFilesChanged(bool overwriteOutputFiles);
    void securePath(QString *securePath);
};

#endif // SECUREFILEMANAGER_H
