#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

// Local
#include "base.h"
#include "enums.h"

class ProcessManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(bool isEnabled READ isEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(Enums::ProcessType type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString typeDescription READ typeDescription NOTIFY typeDescriptionChanged)
    Q_PROPERTY(Enums::ProcessState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString stateDescription READ stateDescription NOTIFY stateDescriptionChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString progressString READ progressString NOTIFY progressStringChanged)
    Q_PROPERTY(bool openFile READ openFile WRITE setOpenFile NOTIFY openFileChanged)

private:
    bool m_isEnabled;
    Enums::ProcessType m_type;
    QString m_typeDescription;
    Enums::ProcessState m_state;
    QString m_stateDescription;
    float m_progress;
    QString m_progressString;
    bool m_openFile;

public:
    explicit ProcessManager(QObject *parent = NULL);
    ~ProcessManager();

public:
    void initialize();
    bool isEnabled() const;
    Enums::ProcessType type() const;
    QString typeDescription() const;
    Enums::ProcessState state() const;
    QString stateDescription() const;
    float progress() const;
    QString progressString() const;
    bool openFile() const;
    void setOpenFile(bool openFile);

private:
    void setIsEnabled(bool isEnabled);
    void setType(Enums::ProcessType type);
    void setTypeDescription(const QString &typeDescription);
    void setState(Enums::ProcessState state);
    void setStateDescription(const QString &stateDescription);
    void setProgress(float progress);
    void setProgressString(const QString &progressString);

public slots:
    void onEncryptionStateChanged(Enums::ProcessState encryptionState);
    void onDecryptionStateChanged(Enums::ProcessState decryptionState);
    void onEncryptionProgressChanged(float encryptionProgress);
    void onDecryptionProgressChanged(float decryptionProgress);
    void onPause();
    void onResume();
    void onStop();
    void onOpenFile(const QString &file);

private slots:
    void onTypeChanged(Enums::ProcessType type);
    void onStateChanged(Enums::ProcessState state);
    void onProgressChanged(float progress);

signals:
    void isEnabledChanged(bool isEnabled);
    void typeChanged(Enums::ProcessType type);
    void typeDescriptionChanged(const QString &typeDescription);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
    void progressChanged(float progress);
    void progressStringChanged(const QString &progressString);
    void openFile(bool *openfile);
    void openFileChanged(bool openfile);
    void pauseEncryption();
    void pauseDecryption();
    void resumeEncryption();
    void resumeDecryption();
    void stopEncryption();
    void stopDecryption();
};

#endif // PROCESSMANAGER_H
