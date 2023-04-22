#include "base32requesthandler.h"

Base32RequestHandler::Base32RequestHandler(QObject* parent) : HttpRequestHandler(parent)
{
}

void Base32RequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "Base32RequestHandler; " << "ThreadID: " << QThread::currentThreadId();

    QString errorMessage;
    const QByteArray cidV1 {IpfsJsonRPC::toCIDv1(request.getParameter("cid"), errorMessage)};

    response.setStatus(200);
    response.write(cidV1.isEmpty() ? errorMessage.toUtf8() : cidV1);
    response.flush();
}
