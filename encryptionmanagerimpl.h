#ifndef ENCRYPTIONMANAGERIMPL_H
#define ENCRYPTIONMANAGERIMPL_H

#include <QImage>
#include <QUrl>
#include <QWaitCondition>

#include "base.h"
#include "enums.h"

#include "fileencryptor.h"

class EncryptionManagerImpl : public Base
{
    Q_OBJECT

private:
    static const QString m_CURRENT_INPUT_FILE_NONE;
    static const QString m_OUTPUT_FILE_EXTENSION;
    static const QString m_PASSPHRASE;
    bool m_isEnabled;
    Enums::State m_state;
    QString m_stateDescription;
    bool m_pause;
    bool m_stop;
    float m_progress;
    QString m_progressString;
    int m_errors;
    int m_warnings;
    int m_skipped;
    int m_overwritten;
    int m_processed;
    QString m_currentInputFile;
    QMutex *m_mutex;
    QWaitCondition *m_waitCondition;

    FileEncryptor *m_fileEncryptor;

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

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    void onEncryptFiles();

private slots:
    void onUpdateState(Enums::State state);
    void onStateChanged(Enums::State state);
    void onProgressChanged(float progress);

signals:
    void isEnabledChanged(bool isEnabled);
    void stateChanged(Enums::State state);
    void stateDescriptionChanged(const QString &stateDescription);
    void pauseChanged(bool pause);
    void stopChanged(bool stop);
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
    void videoFiles(QStringList *videoFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
    void working();
    void paused();
    void stopped();
};

#endif // ENCRYPTIONMANAGERIMPL_H
