#ifndef ENCRYPTIONMANAGERIMPL_H
#define ENCRYPTIONMANAGERIMPL_H

// Qt
#include <QImage>
#include <QUrl>
#include <QWaitCondition>

// Crypto++
#include <osrng.h>

// Local
#include "base.h"
#include "enums.h"

class EncryptionManagerImpl : public Base
{
    Q_OBJECT

private:
    static const QString m_CURRENT_INPUT_FILE_NONE;
    static const QString m_OUTPUT_FILE_EXTENSION;
    static const QString m_PASSPHRASE;
    static const QString m_AES_KEY_FILE_NAME;
    static const QString m_AES_IV_FILE_NAME;
    static const unsigned long long m_ENCRYPTION_THRESHOLD_SIZE;
    static const unsigned long long m_ENCRYPTION_CHUNK_SIZE;
    bool m_isEnabled;
    Enums::State m_state;
    QString m_stateDescription;
    bool m_pause;
    bool m_stop;
    unsigned long long m_encryptedBytes;
    QString m_encryptedBytesString;
    unsigned long long m_bytesToEncrypt;
    QString m_bytesToEncryptString;
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
    explicit EncryptionManagerImpl(QMutex *mutex, QWaitCondition *waitCondition, QObject *parent = NULL);
    ~EncryptionManagerImpl();

public:
    void initialize();
    bool isEnabled() const;
    Enums::State state() const;
    QString stateDescription() const;
    void setPause(bool pause);
    void setStop(bool stop);
    unsigned long long encryptedBytes() const;
    QString encryptedBytesString() const;
    unsigned long long bytesToEncrypt() const;
    QString bytesToEncryptString() const;
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
    void setState(Enums::State state);
    void setStateDescription(const QString &stateDescription);
    void setEncryptedBytes(unsigned long long encryptedBytes);
    void setEncryptedBytesString(const QString &encryptedBytesString);
    void setBytesToEncrypt(unsigned long long bytesToEncrypt);
    void setBytesToEncryptString(const QString& bytesToEncryptString);
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
    bool encryptFileWithMac(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime);
    bool encryptFileWithAes(const QString &inputFile, unsigned long inputFileSize, const QString &outputFile, QTime &encryptionTime);

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    void onEncryptFiles();

private slots:
    void onUpdateState(Enums::State state);
    void onStateChanged(Enums::State state);
    void onProgressChanged(float progress);
    void onBytesEncryptedChanged(unsigned long long bytesEncrypted);
    void onBytesToEncryptChanged(unsigned long long bytesToEncrypt);

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::State state);
    void stateDescriptionChanged(const QString &stateDescription);
    void pauseChanged(bool pause);
    void stopChanged(bool stop);
    void encryptedBytesChanged(unsigned long long encryptedBytes);
    void encryptedBytesStringChanged(const QString &encryptedBytesString);
    void bytesToEncryptChanged(unsigned long long bytesToEncrypt);
    void bytesToEncryptStringChanged(const QString &bytesToEncryptString);
    void progressChanged(float progress);
    void progressStringChanged(const QString &progressString);
    void errorsChanged(int errors);
    void warningsChanged(int warnings);
    void skippedChanged(int skipped);
    void overwrittenChanged(int overwritten);
    void processedChanged(int processed);
    void currentInputFileChanged(const QString &currentInputFile);
    void isSourcePathUrlValid(bool *isSourcePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void sourcePath(QString *sourcePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
    void working();
    void paused();
    void stopped();
};

#endif // ENCRYPTIONMANAGERIMPL_H
