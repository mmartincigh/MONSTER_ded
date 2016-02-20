#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

// Qt
#include <QQuickWindow>
#include <QApplication>

// Local
#include "base.h"

class WindowManager : public Base
{
    Q_OBJECT
    Q_PROPERTY(Qt::WindowFlags windowFlags READ windowFlags CONSTANT)

private:
    Qt::WindowFlags m_windowFlags;
    QQuickWindow *m_mainWindow;
    QApplication *m_application;

public:
    explicit WindowManager(QObject *parent = NULL);
    ~WindowManager();

public:
    Qt::WindowFlags windowFlags() const;
    void setMainWindow(QQuickWindow *mainWindow);
    void setApplication(QApplication *mainWindow);
    void bringToFront();

signals:
    void mainWindowChanged(const QQuickWindow *mainWindow);
    void applicationChanged(const QApplication *application);
};

#endif // WINDOWMANAGER_H
