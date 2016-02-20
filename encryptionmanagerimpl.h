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
    static const unsigned long long m_ENCRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_ENCRYPTION_CHUNK_SIZE;

public:
    explicit EncryptionManagerImpl(const QString &applicationDirPath, QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~EncryptionManagerImpl();

private:
    bool checkIfEnabled();
    EncryptionState encryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime);
    EncryptionState encryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, const CryptoPP::SecByteBlock key, const unsigned char *iv, QTime &encryptionTime);

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    virtual void onProcess();
    virtual void onProcess(const QString &inputFile);

signals:
    void isSourcePathUrlValid(bool *isSourcePathUrlValid);
    void isSecurePathUrlValid(bool *isSecurePathUrlValid);
    void sourcePath(QString *sourcePath);
    void securePath(QString *securePath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
};

#endif // ENCRYPTIONMANAGERIMPL_H
