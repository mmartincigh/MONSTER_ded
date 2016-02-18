#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

// Qt
#include <QThread>
#include <QSharedPointer>

// Local
#include "iprocess.h"
#include "encryptionmanagerimpl.h"

class EncryptionManager : public IProcess
{
    Q_OBJECT

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QThread m_encryptionManagerImplThread;
    QSharedPointer<EncryptionManagerImpl> m_encryptionManagerImplSptr;

public:
    explicit EncryptionManager(QObject *parent = NULL);
    ~EncryptionManager();

public:
    virtual void initialize();
    virtual void uninitialize();
    virtual bool isEnabled() const;
    virtual Enums::ProcessState state() const;
    virtual QString stateDescription() const;
    virtual float progress() const;
    virtual QString progressString() const;
    virtual unsigned long long processedBytes() const;
    virtual QString processedBytesString() const;
    virtual unsigned long long bytesToProcess() const;
    virtual QString bytesToProcessString() const;
    virtual int errors() const;
    virtual int warnings() const;
    virtual int skipped() const;
    virtual int overwritten() const;
    virtual int processed() const;
    virtual QString currentInputFile() const;

public slots:
    void onIsSourcePathUrlValidChanged(bool isSourcePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    virtual void onProcess();
    virtual void onPause();
    virtual void onResume();
    virtual void onStop();

signals:
    void isSourcePathUrlValid(bool *isSourcePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void sourcePath(QString *sourcePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
};

#endif // ENCRYPTIONMANAGER_H
