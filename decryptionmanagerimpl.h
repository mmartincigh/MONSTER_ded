#ifndef DECRYPTIONMANAGERIMPL_H
#define DECRYPTIONMANAGERIMPL_H

// Local
#include "processimplbase.h"

class DecryptionManagerImpl : public ProcessImplBase
{
    Q_OBJECT

private:
    enum DecryptionState
    {
        DecryptionState_Success,
        DecryptionState_Warning,
        DecryptionState_Error
    };

private:
    static const unsigned long long m_DECRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_DECRYPTION_CHUNK_SIZE;

public:
    explicit DecryptionManagerImpl(const QString &applicationDirPath, QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~DecryptionManagerImpl();

private:
    bool checkIfEnabled();
    DecryptionState decryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime);
    DecryptionState decryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, const CryptoPP::SecByteBlock key, const unsigned char *iv, QTime &decryptionTime);

public slots:
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    virtual void onProcess();
    virtual void onProcess(const QString &inputFile);
    virtual void onProcess(const QStringList &inputFiles);

signals:
    void isSecurePathUrlValid(bool *isSecurePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void securePath(QString *securePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
    void openFile(const QString &file);
};

#endif // DECRYPTIONMANAGERIMPL_H
