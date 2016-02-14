#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

// Qt
#include <QThread>
#include <QSharedPointer>

// Local
#include "base.h"
#include "encryptionmanagerimpl.h"

class EncryptionManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(bool isEnabled READ isEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(Enums::ProcessState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString stateDescription READ stateDescription NOTIFY stateDescriptionChanged)
    Q_PROPERTY(unsigned long long encryptedBytes READ encryptedBytes NOTIFY encryptedBytesChanged)
    Q_PROPERTY(QString encryptedBytesString READ encryptedBytesString NOTIFY encryptedBytesStringChanged)
    Q_PROPERTY(unsigned long long bytesToEncrypt READ bytesToEncrypt NOTIFY bytesToEncryptChanged)
    Q_PROPERTY(QString bytesToEncryptString READ bytesToEncryptString NOTIFY bytesToEncryptStringChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString progressString READ progressString NOTIFY progressStringChanged)
    Q_PROPERTY(int errors READ errors NOTIFY errorsChanged)
    Q_PROPERTY(int warnings READ warnings NOTIFY warningsChanged)
    Q_PROPERTY(int skipped READ skipped NOTIFY skippedChanged)
    Q_PROPERTY(int overwritten READ overwritten NOTIFY overwrittenChanged)
    Q_PROPERTY(int processed READ processed NOTIFY processedChanged)
    Q_PROPERTY(QString currentInputFile READ currentInputFile NOTIFY currentInputFileChanged)

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QThread m_encryptionManagerImplThread;
    QSharedPointer<EncryptionManagerImpl> m_encryptionManagerImplSptr;

public:
    explicit EncryptionManager(QObject *parent = NULL);
    ~EncryptionManager();

public:
    void initialize();
    void uninitialize();
    bool isEnabled() const;
    Enums::ProcessState state() const;
    QString stateDescription() const;
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

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    void onEncryptFiles();
    void onPause();
    void onResume();
    void onStop();

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
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
};

#endif // ENCRYPTIONMANAGER_H
