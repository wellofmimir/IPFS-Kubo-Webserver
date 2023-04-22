#include "addfilerequesthandler.h"

AddFileRequestHandler::AddFileRequestHandler(QObject* parent) : HttpRequestHandler(parent)
{
}

void AddFileRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "AddFileRequestHandler; " << "ThreadID: " << QThread::currentThreadId();

    if (request.isUploadedFilesEmpty())
    {
        response.setStatus(200);
        response.write("No file passed!");
        response.flush();
        return;
    }

    QString errorMessage;
    const QByteArray content {IpfsJsonRPC::addFile(request.getUploadedFile("myfile")->readAll(), errorMessage)};

    response.setStatus(200);
    response.write(content.isEmpty() ? errorMessage.toUtf8() : content);
    response.flush();
}
