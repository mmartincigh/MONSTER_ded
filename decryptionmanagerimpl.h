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
    explicit DecryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~DecryptionManagerImpl();

private:
    bool checkIfEnabled();
    DecryptionState decryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime);
    DecryptionState decryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime);

public slots:
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    virtual void onProcess();

    void onDecryptFile(const QString &fileToDecrypt);

signals:
    void isSecurePathUrlValid(bool *isSecurePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void securePath(QString *securePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
};

#endif // DECRYPTIONMANAGERIMPL_H
