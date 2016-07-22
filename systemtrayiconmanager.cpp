// Local
#include "systemtrayiconmanager.h"
#include "applicationutils.h"
#include "utils.h"

const QString SystemTrayIconManager::m_MENU_NAME(ApplicationUtils::APPLICATION_NAME + " system tray menu");

SystemTrayIconManager::SystemTrayIconManager(QObject *parent) :
    Base("STIM", parent),
    m_pixmap(":/images/application/systemTrayIcon"),
    m_icon(m_pixmap, this),
    m_menu(m_MENU_NAME)
{
    QObject::connect(&m_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(&m_icon, SIGNAL(messageClicked()), this, SLOT(onMessageClicked()));

    this->debug("System tray icon manager created");
}


SystemTrayIconManager::~SystemTrayIconManager()
{
    this->debug("System tray icon manager created");
}

void SystemTrayIconManager::initialize()
{
    m_menu.addAction("do something cool");
    m_menu.addSeparator();
    m_menu.addAction(ApplicationUtils::APPLICATION_NAME + " v" + ApplicationUtils::APPLICATION_VERSION);
    m_icon.setContextMenu(&m_menu);

    this->debug("Initialized");
}

void SystemTrayIconManager::show()
{
    m_icon.show();
}

void SystemTrayIconManager::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    this->debug("System tray icon activated: " + Utils::activationReasonToString(reason));
}

void SystemTrayIconManager::onMessageClicked()
{
    this->debug("System tray icon message clicked");
}
