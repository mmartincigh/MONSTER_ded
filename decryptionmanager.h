#ifndef DECRYPTIONMANAGER_H
#define DECRYPTIONMANAGER_H

// Qt
#include <QThread>
#include <QSharedPointer>

// Local
#include "iprocess.h"
#include "decryptionmanagerimpl.h"

class DecryptionManager : public IProcess
{
    Q_OBJECT

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QThread m_decryptionManagerImplThread;
    QSharedPointer<DecryptionManagerImpl> m_decryptionManagerImplSptr;

public:
    explicit DecryptionManager(const QString &applicationDirPath, QObject *parent = NULL);
    ~DecryptionManager();

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
    void onIsSecurePathUrlValidChanged(bool isSecurePathUrlValid);
    void onIsDestinationPathUrlValidChanged(bool isDestinationPathUrlValid);
    virtual void onProcess();
    virtual void onProcess(const QString &inputFile);
    virtual void onPause();
    virtual void onResume();
    virtual void onStop();

signals:
    void isSecurePathUrlValid(bool *isSecurePathUrlValid);
    void isDestinationPathUrlValid(bool *isDestinationPathUrlValid);
    void securePath(QString *securePath);
    void destinationPath(QString *destinationPath);
    void inputFiles(QStringList *inputFiles);
    void overwriteOutputFiles(bool *overwriteOutputFiles);
};

#endif // DECRYPTIONMANAGER_H
