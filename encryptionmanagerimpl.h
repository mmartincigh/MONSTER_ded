#ifndef ENCRYPTIONMANAGERIMPL_H
#define ENCRYPTIONMANAGERIMPL_H

// Local
#include "processimplbase.h"

class EncryptionManagerImpl : public ProcessImplBase
{
    Q_OBJECT

private:
    enum EncryptionState
    {
        EncryptionState_Success,
        EncryptionState_Warning,
        EncryptionState_Error
    };

private:
    static const QString m_OUTPUT_FILE_EXTENSION;
    static const unsigned long long m_ENCRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_ENCRYPTION_CHUNK_SIZE;

public:
    explicit EncryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~EncryptionManagerImpl();

private:
    bool checkIfEnabled();
    EncryptionState encryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime);
    EncryptionState encryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime);

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    virtual void onProcess();

signals:
    void isSourcePathUrlValid(bool *isSourcePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void sourcePath(QString *sourcePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
};

#endif // ENCRYPTIONMANAGERIMPL_H
