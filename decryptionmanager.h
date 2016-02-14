#ifndef DECRYPTIONMANAGER_H
#define DECRYPTIONMANAGER_H

// Qt
#include <QThread>
#include <QSharedPointer>

// Local
#include "base.h"
#include "decryptionmanagerimpl.h"

class DecryptionManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(bool isEnabled READ isEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(Enums::ProcessState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString stateDescription READ stateDescription NOTIFY stateDescriptionChanged)
    Q_PROPERTY(unsigned long long decryptedBytes READ decryptedBytes NOTIFY decryptedBytesChanged)
    Q_PROPERTY(QString decryptedBytesString READ decryptedBytesString NOTIFY decryptedBytesStringChanged)
    Q_PROPERTY(unsigned long long bytesToDecrypt READ bytesToDecrypt NOTIFY bytesToDecryptChanged)
    Q_PROPERTY(QString bytesToDecryptString READ bytesToDecryptString NOTIFY bytesToDecryptStringChanged)
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
    QThread m_decryptionManagerImplThread;
    QSharedPointer<DecryptionManagerImpl> m_decryptionManagerImplSptr;

public:
    explicit DecryptionManager(QObject *parent = NULL);
    ~DecryptionManager();

public:
    void initialize();
    void uninitialize();
    bool isEnabled() const;
    Enums::ProcessState state() const;
    QString stateDescription() const;
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

public slots:
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    void onDecryptFiles();
    void onPause();
    void onResume();
    void onStop();

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
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
};

#endif // DECRYPTIONMANAGER_H
