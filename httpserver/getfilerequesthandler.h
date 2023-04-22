#ifndef GETFILEREQUESTHANDLER_H
#define GETFILEREQUESTHANDLER_H

#include "httprequesthandler.h"
#include "ipfsjsonrpc/ipfsjsonrpc.h"
#include <QObject>

using namespace stefanfrings;

class GetFileRequestHandler : public HttpRequestHandler
{
public:
    GetFileRequestHandler(QObject * parent);
    void service(HttpRequest &request, HttpResponse &response) override;
};

#endif // GETFILEREQUESTHANDLER_H
