#include "getfilerequesthandler.h"
#include "errormessagestrings.h"

GetFileRequestHandler::GetFileRequestHandler(QObject * parent) : HttpRequestHandler(parent)
{
}

void GetFileRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "GetFileRequestHandler; " << "ThreadID: " << QThread::currentThreadId();

    QString errorMessage;
    const QByteArray content {IpfsJsonRPC::getFile(request.getParameter("cid"), errorMessage)};

    response.setStatus(200);
    response.write(content.isEmpty() ? errorMessage.append(eMailInformation).toUtf8() : content);
    response.flush();
}
