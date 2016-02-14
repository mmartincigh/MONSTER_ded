#ifndef DECRYPTIONMANAGERIMPL_H
#define DECRYPTIONMANAGERIMPL_H

// Qt
#include <QWaitCondition>

// Crypto++
#include <osrng.h>

// Local
#include "base.h"
#include "enums.h"

class DecryptionManagerImpl : public Base
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
    static const QString m_CURRENT_INPUT_FILE_NONE;
    static const QString m_OUTPUT_FILE_EXTENSION;
    static const QString m_PASSPHRASE;
    static const QString m_AES_KEY_FILE_NAME;
    static const QString m_AES_IV_FILE_NAME;
    static const unsigned long long m_DECRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_DECRYPTION_CHUNK_SIZE;
    bool m_isEnabled;
    Enums::ProcessState m_state;
    QString m_stateDescription;
    bool m_pause;
    bool m_stop;
    unsigned long long m_decryptedBytes;
    QString m_decryptedBytesString;
    unsigned long long m_bytesToDecrypt;
    QString m_bytesToDecryptString;
    float m_progress;
    QString m_progressString;
    int m_errors;
    int m_warnings;
    int m_skipped;
    int m_overwritten;
    int m_processed;
    QString m_currentInputFile;
    CryptoPP::SecByteBlock m_key;
    unsigned char m_iv[CryptoPP::AES::BLOCKSIZE];
    QMutex *m_mutex;
    QWaitCondition *m_waitCondition;

public:
    explicit DecryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~DecryptionManagerImpl();

public:
    void initialize();
    bool isEnabled() const;
    Enums::ProcessState state() const;
    QString stateDescription() const;
    void setPause(bool pause);
    void setStop(bool stop);
    unsigned long long decryptedBytes() const;
    QString decryptedBytesString() const;
    unsigned long long bytesToDecrypt() const;
    QString bytesToDecryptString() const;
    float progress() const;
    QString progressString() const;
    int errors() const;
    int warnings() const;
    int skipped() const;
    int overwritten() const;
    int processed() const;
    QString currentInputFile() const;

private:
    void setIsEnabled(bool isEnabled);
    void setState(Enums::ProcessState state);
    void setStateDescription(const QString &stateDescription);
    void setDecryptedBytes(unsigned long long decryptedBytes);
    void setDecryptedBytesString(const QString &decryptedBytesString);
    void setBytesToDecrypt(unsigned long long bytesToDecrypt);
    void setBytesToDecryptString(const QString& bytesToDecryptString);
    void setProgress(float progress);
    void setProgressString(const QString &progressString);
    void setErrors(int errors);
    void setWarnings(int warnings);
    void setSkipped(int skipped);
    void setOverwritten(int overwritten);
    void setProcessed(int processed);
    void setCurrentInputFile(const QString &currentInputFile);
    bool checkIfEnabled();
    bool processStateCheckpoint();
    bool readKeyFromFile();
    bool readIvFromFile();
    DecryptionState decryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime);
    DecryptionState decryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &decryptionTime);

public slots:
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    void onDecryptFiles();

private slots:
    void onUpdateState(Enums::ProcessState state);
    void onStateChanged(Enums::ProcessState state);
    void onProgressChanged(float progress);
    void onBytesDecryptedChanged(unsigned long long bytesDecrypted);
    void onBytesToDecryptChanged(unsigned long long bytesToDecrypt);

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
    void pauseChanged(bool pause);
    void stopChanged(bool stop);
    void decryptedBytesChanged(unsigned long long decryptedBytes);
    void decryptedBytesStringChanged(const QString &decryptedBytesString);
    void bytesToDecryptChanged(unsigned long long bytesToDecrypt);
    void bytesToDecryptStringChanged(const QString &bytesToDecryptString);
    void progressChanged(float progress);
    void progressStringChanged(const QString &progressString);
    void errorsChanged(int errors);
    void warningsChanged(int warnings);
    void skippedChanged(int skipped);
    void overwrittenChanged(int overwritten);
    void processedChanged(int processed);
    void currentInputFileChanged(const QString &currentInputFile);
    void isSecurePathUrlValid(bool *isSecurePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void securePath(QString *securePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
    void working();
    void paused();
    void stopped();
};

#endif // DECRYPTIONMANAGERIMPL_H
