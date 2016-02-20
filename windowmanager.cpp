// Qt
#include <QMainWindow>

// Local
#include "windowmanager.h"
#include "utils.h"

WindowManager::WindowManager(QObject *parent) :
    Base("WM", parent),
    #if defined(Q_OS_WIN)
    m_windowFlags(Qt::Window),
    m_mainWindow(NULL),
    m_application(NULL)
    #else
    m_windowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
    #endif
{
    this->debug("Window manager manager created");
}

WindowManager::~WindowManager()
{
    this->debug("Window manager manager created");
}

Qt::WindowFlags WindowManager::windowFlags() const
{
    return m_windowFlags;
}

void WindowManager::setMainWindow(QQuickWindow *mainWindow)
{
    if (m_mainWindow == mainWindow)
    {
        return;
    }

    m_mainWindow = mainWindow;

    this->debug("Main window changed: " + Utils::pointerToString(m_mainWindow));

    emit this->mainWindowChanged(m_mainWindow);
}

void WindowManager::setApplication(QApplication *application)
{
    if (m_application == application)
    {
        return;
    }

    m_application = application;

    this->debug("Application changed: " + Utils::pointerToString(m_application));

    emit this->applicationChanged(m_application);
}

void WindowManager::bringToFront()
{
    if (m_mainWindow == NULL
            || m_application == NULL)
    {
        return;
    }

    this->debug("Bringing application's window to front...");

    m_mainWindow->raise();
    m_mainWindow->requestActivate();
    m_mainWindow->show();
    m_application->setActiveWindow(qobject_cast<QMainWindow *>(m_mainWindow));

    this->debug("Active window set");
}
