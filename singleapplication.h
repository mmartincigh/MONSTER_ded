#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

// Qt
#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>

// Local
#include "applicationutils.h"

class SingleApplication : public QApplication
{
    Q_OBJECT

private:
    static const int m_TIMEOUT;
    bool m_isRunning;
    QSharedMemory m_sharedMemory;
    QLocalServer m_localServer;

public:
    explicit SingleApplication(int argc, char *argv[]);
    ~SingleApplication();

public:
    bool isRunning() const;

private:
    void setIsRunning(bool isRunning);

public slots:
    bool onSendMessage(const QString &message);

private slots:
    void onMessageReceived();

signals:
    void isRunningChanged(bool isRunning);
    void messageReceived(const QString &message);
};

#endif // SINGLEAPPLICATION_H
