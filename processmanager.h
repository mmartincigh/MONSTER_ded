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

private:
    bool m_isEnabled;
    Enums::ProcessType m_type;
    QString m_typeDescription;
    Enums::ProcessState m_state;
    QString m_stateDescription;

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

private:
    void setIsEnabled(bool isEnabled);
    void setType(Enums::ProcessType type);
    void setTypeDescription(const QString &typeDescription);
    void setState(Enums::ProcessState state);
    void setStateDescription(const QString &stateDescription);

public slots:
    void onEncryptionStateChanged(Enums::ProcessState encryptionState);
    void onDecryptionStateChanged(Enums::ProcessState decryptionState);
    void onPause();
    void onResume();
    void onStop();

private slots:
    void onTypeChanged(Enums::ProcessType type);
    void onStateChanged(Enums::ProcessState state);

signals:
    void isEnabledChanged(bool isEnabled);
    void typeChanged(Enums::ProcessType type);
    void typeDescriptionChanged(const QString &typeDescription);
    void stateChanged(Enums::ProcessState state);
    void stateDescriptionChanged(const QString &stateDescription);
    void pauseEncryption();
    void pauseDecryption();
    void resumeEncryption();
    void resumeDecryption();
    void stopEncryption();
    void stopDecryption();
};

#endif // PROCESSMANAGER_H
