#ifndef SYSTEMTRAYICONMANAGER_H
#define SYSTEMTRAYICONMANAGER_H

// Qt
#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

// Local
#include "base.h"

class SystemTrayIconManager : public Base
{
    Q_OBJECT

private:
    static const QString m_MENU_NAME;
    QPixmap m_pixmap;
    QSystemTrayIcon m_icon;
    QMenu m_menu;

public:
    explicit SystemTrayIconManager(QObject *parent = NULL);
    ~SystemTrayIconManager();

public:
    void initialize();
    void show();

public slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void onMessageClicked();
};

#endif // SYSTEMTRAYICONMANAGER_H
