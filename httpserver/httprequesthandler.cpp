/**
  @file
  @author Stefan Frings
*/

#include "httprequesthandler.h"
#include "base32requesthandler.h"
#include "getfilerequesthandler.h"
#include "addfilerequesthandler.h"

using namespace stefanfrings;

HttpRequestHandler::HttpRequestHandler(QObject* parent)
    : QObject(parent)
{
}

HttpRequestHandler::~HttpRequestHandler()
{
}

void HttpRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    qDebug("HttpRequestHandler: request=%s %s %s",request.getMethod().data(), request.getPath().data(), request.getVersion().data());
    qDebug() << "ThreadID: " << QThread::currentThreadId();

    if (request.getMethod() == "GET")
    {
        if (request.getPath().isEmpty()) return;

        if (request.getPath() == "/cidV1")
        {
            if (!request.getParameterMap().contains("cid"))
            {
                response.setHeader("OK", 200);
                response.write("Method 'cidV1': Parameter named 'cid' is missing.");
                response.flush();
                return;
            }

            auto requestHandleFunction = [this](HttpRequest &request, HttpResponse &response)
            {
                QScopedPointer<Base32RequestHandler> base32RequestHandler {new Base32RequestHandler{this}};
                base32RequestHandler->service(request, response);
            };

            handleRequestInThread(requestHandleFunction, request, response);
        }

        if (request.getPath() == "/getfile")
        {
            if (!request.getParameterMap().contains("cid"))
            {
                response.setHeader("OK", 200);
                response.write("Method 'getfile': Parameter named 'cid' is missing.");
                response.flush();
                return;
            }

            auto requestHandleFunction = [this](HttpRequest &request, HttpResponse &response)
            {
                QScopedPointer<GetFileRequestHandler> getFileRequestHandler {new GetFileRequestHandler{this}};
                getFileRequestHandler->service(request, response);
            };

            handleRequestInThread(requestHandleFunction, request, response);
        }
    }
    else if (request.getMethod() == "POST")
    {
        if (request.getPath() == "/addfile")
        {
            auto requestHandleFunction = [this](HttpRequest &request, HttpResponse &response)
            {
                QScopedPointer<AddFileRequestHandler> addFileRequestHandler {new AddFileRequestHandler{this}};
                addFileRequestHandler->service(request, response);
            };

            handleRequestInThread(requestHandleFunction, request, response);
        }
    }
}

void HttpRequestHandler::handleRequestInThread(std::function<void(HttpRequest &, HttpResponse &)> function, HttpRequest &request, HttpResponse &response)
{
    QEventLoop eventLoop;
    QFutureWatcher<void> futureWatcher;
    connect(&futureWatcher, &QFutureWatcherBase::finished, &eventLoop, &QEventLoop::quit);
    QFuture<void> future {QtConcurrent::run(function, std::ref(request), std::ref(response))};
    futureWatcher.setFuture(future);
    eventLoop.exec();
}
