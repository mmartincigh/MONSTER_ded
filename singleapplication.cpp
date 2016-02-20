//System
#include <iostream>

// Qt
#include <QLocalSocket>

// Local
#include "singleapplication.h"

const int SingleApplication::m_TIMEOUT(100);

SingleApplication::SingleApplication(int argc, char *argv[]) :
    QApplication(argc, argv),
    m_isRunning(false),
    m_sharedMemory(this),
    m_localServer(this)
{
    m_sharedMemory.setKey(ApplicationUtils::APPLICATION_NAME);
    this->setIsRunning(!m_sharedMemory.attach());
    if (m_isRunning)
    {
        // Create the shared memory.
        if (!m_sharedMemory.create(1))
        {
            std::cerr << "   SA > Unable to create the first program instance: " << m_sharedMemory.errorString().toStdString() << std::endl;

            exit(EXIT_FAILURE);
        }
        // Create a local server and listen to incoming messages from the other program instances.
        QObject::connect(&m_localServer, SIGNAL(newConnection()), this, SLOT(onMessageReceived()));
        m_localServer.listen(ApplicationUtils::APPLICATION_NAME);
    }

    std::cout << "   SA > Single application created" << std::endl;
}

SingleApplication::~SingleApplication()
{
    std::cout << "   SA > Single application disposed of" << std::endl;
}

bool SingleApplication::isRunning() const
{
    return m_isRunning;
}

void SingleApplication::setIsRunning(bool isRunning)
{
    if (m_isRunning == isRunning)
    {
        return;
    }

    m_isRunning = isRunning;

    std::cout << "   SA > Is running changed: " << (m_isRunning ? "true" : "false") << std::endl;

    emit this->isRunningChanged(m_isRunning);
}

bool SingleApplication::onSendMessage(const QString &message)
{
    if (m_isRunning)
    {
        return false;
    }

    std::cout << "   SA > Sending a message to the running instance: " << message.toStdString() << std::endl;

    QLocalSocket localSocket(this);
    localSocket.connectToServer(ApplicationUtils::APPLICATION_NAME, QIODevice::WriteOnly);
    if (!localSocket.waitForConnected(m_TIMEOUT))
    {
        std::cerr << "   SA > ERROR: Cannot wait for a connection to the running program instance: " << localSocket.errorString().toStdString() << std::endl;

        return false;
    }
    int bytes_written = localSocket.write(message.toUtf8());
    if (bytes_written < 0)
    {
        std::cerr << "   SA > ERROR: Cannot send the message to the running program instance socket" << std::endl;

        return false;
    }
    if (bytes_written < message.toUtf8().length())
    {
        std::cerr << "   SA > ERROR: Cannot send all the message to the running program instance socket" << std::endl;

        return false;
    }
    if (!localSocket.waitForBytesWritten(m_TIMEOUT))
    {
        std::cerr << "   SA > ERROR: Cannot wait for bytes written to the running program instance: " << localSocket.errorString().toStdString() << std::endl;

        return false;
    }
    localSocket.disconnectFromServer();

    std::cout << "   SA > Message sent" << std::endl;

    return true;
}

void SingleApplication::onMessageReceived()
{
    std::cout << "   SA > Receiving message..." << std::endl;

    QLocalSocket *local_socket = m_localServer.nextPendingConnection();
    if (!local_socket->waitForReadyRead(m_TIMEOUT))
    {
        std::cout << "   SA > Cannot wait for messages: " << local_socket->errorString().toStdString() << std::endl;

        return;
    }
    QByteArray byte_array = local_socket->readAll();
    QString message = QString::fromUtf8(byte_array.constData());
    local_socket->disconnectFromServer();

    std::cout << "   SA > Message received: " << message.toStdString() << std::endl;

    emit this->messageReceived(message);
}
