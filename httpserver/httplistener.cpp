/**
  @file
  @author Stefan Frings
*/

#include "httplistener.h"
#include "httpconnectionhandler.h"
#include "httpconnectionhandlerpool.h"
#include <QCoreApplication>

using namespace stefanfrings;

const quint16 STANDARD_PORT {51234};

HttpListener::HttpListener(const QSettings* settings, HttpRequestHandler* requestHandler, QObject *parent)
    : QTcpServer(parent)
{
    Q_ASSERT(settings != nullptr);
    Q_ASSERT(requestHandler != nullptr);

    pool = nullptr;
    this->settings = settings;
    this->requestHandler = requestHandler;

    // Reqister type of socketDescriptor for signal/slot handling
    qRegisterMetaType<tSocketDescriptor>("tSocketDescriptor");

    // Start listening
    listen();
}

HttpListener::~HttpListener()
{
    close();
    qDebug("HttpListener: destroyed");
}

void HttpListener::listen()
{
    if (pool == nullptr) pool = new HttpConnectionHandlerPool(settings, requestHandler);

    QString host = settings->value("host").toString();
    quint16 port = settings->value("port").toUInt() & 0xFFFF;

    if (port == 0) port = STANDARD_PORT;

    QTcpServer::listen(host.isEmpty() ? QHostAddress::Any : QHostAddress(host), port);

    if (!isListening())
    {
        qCritical("HttpListener: Cannot bind on port %i: %s", port, qPrintable(errorString()));
        return;
    }

    qDebug("HttpListener: Listening on port %i",port);
}


void HttpListener::close()
{
    QTcpServer::close();

    if (pool != nullptr)
    {
        delete pool;
        pool = nullptr;
    }

    qDebug("HttpListener: closed");
}

void HttpListener::incomingConnection(tSocketDescriptor socketDescriptor)
{
#ifdef SUPERVERBOSE
    qDebug("HttpListener: New connection");
#endif

    HttpConnectionHandler * freeHandler {nullptr};
    if (pool) freeHandler = pool->getConnectionHandler();

    // Let the handler process the new connection.
    if (freeHandler)
    {
        // The descriptor is passed via event queue because the handler lives in another thread
        QMetaObject::invokeMethod(freeHandler, "handleConnection", Qt::QueuedConnection, Q_ARG(tSocketDescriptor, socketDescriptor));
    }
    else
    {
        // Reject the connection
        qDebug("HttpListener: Too many incoming connections");
        QTcpSocket * const socket {new QTcpSocket(this)};
        socket->setSocketDescriptor(socketDescriptor);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater, Qt::UniqueConnection);
        socket->write("HTTP/1.1 503 too many connections\r\nConnection: close\r\n\r\nToo many connections\r\n");
        socket->disconnectFromHost();
    }
}
