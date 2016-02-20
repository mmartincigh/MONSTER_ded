// Qt
#include <QPixmap>
#include <QSplashScreen>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>

// Local
#include "singleapplication.h"
#include "applicationmanager.h"
#include "applicationutils.h"

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    setbuf(stdout, NULL);
#endif

    qInstallMessageHandler(LogManager::messageHandler);
    qRegisterMetaType<Enums::ProcessType>("Enums::ProcessType");
    qRegisterMetaType<Enums::ProcessState>("Enums::ProcessState");
    qmlRegisterType<Enums>("MONSTER.MONSTER_ded", 1, 0, "Enums");

    SingleApplication application(argc, argv);
    application.setOrganizationName(ApplicationUtils::COMPANY_NAME);
    application.setOrganizationDomain(ApplicationUtils::COMPANY_WEBSITE);
    application.setApplicationName(ApplicationUtils::APPLICATION_NAME);
    application.setApplicationVersion(ApplicationUtils::APPLICATION_VERSION);
    if (!application.isRunning())
    {
        // Send a message to the running instance.
        QStringList application_arguments(application.arguments());
        QString message(application_arguments.length() > 1 ? application_arguments.at(1) : "bring to front");
        int ret_val = application.onSendMessage(message) ? EXIT_SUCCESS : EXIT_FAILURE;

        return ret_val;
    }

    QPixmap pixmap(":/images/application/splashScreen");
    QSplashScreen splash_screen(pixmap);
    splash_screen.show();
    application.processEvents();

    LogManager::initialize(application.applicationDirPath());

    ApplicationManager application_manager(application.applicationDirPath(), application.arguments(), &application);
    QObject::connect(&application, SIGNAL(messageReceived(QString)), &application_manager, SLOT(onMessageReceived(QString)));
    application_manager.initialize();

    QQmlApplicationEngine engine;
    QObject::connect(&engine, SIGNAL(quit()), &application, SLOT(quit()));

    QQmlComponent component(&engine);
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("application", &application);
    context->setContextProperty("applicationManager", &application_manager);
    context->setContextProperty("sourcePathManager", application_manager.sourcePathManager());
    context->setContextProperty("securePathManager", application_manager.securePathManager());
    context->setContextProperty("destinationPathManager", application_manager.destinationPathManager());
    context->setContextProperty("sourceFileManager", application_manager.sourceFileManager());
    context->setContextProperty("secureFileManager", application_manager.secureFileManager());
    context->setContextProperty("encryptionManager", application_manager.encryptionManager());
    context->setContextProperty("decryptionManager", application_manager.decryptionManager());
    context->setContextProperty("processManager", application_manager.processManager());
    context->setContextProperty("statsManager", application_manager.statsManager());
    context->setContextProperty("windowManager", application_manager.windowManager());
    component.loadUrl(QUrl("qrc:/qml/main.qml"));
    if (!component.isReady())
    {
        qCritical() << "GUI> ERROR:" << qPrintable(component.errorString().trimmed());

        return EXIT_FAILURE;
    }
    QObject *root = component.beginCreate(context);
    QObject::connect(root, SIGNAL(completed()), &splash_screen, SLOT(close()));
    QObject::connect(root, SIGNAL(completed()), &application_manager, SLOT(onGuiCompleted()));
    component.completeCreate();

    QQuickWindow *main_window = qobject_cast<QQuickWindow *>(root);
    application_manager.windowManager()->setMainWindow(main_window);
    application_manager.windowManager()->setApplication(&application);

    return application.exec();
}
