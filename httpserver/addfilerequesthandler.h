#ifndef ADDFILEREQUESTHANDLER_H
#define ADDFILEREQUESTHANDLER_H

#include "httprequesthandler.h"
#include "ipfsjsonrpc/ipfsjsonrpc.h"
#include <QObject>

using namespace stefanfrings;

class AddFileRequestHandler : public HttpRequestHandler
{
public:
    AddFileRequestHandler(QObject * parent);

    void service(HttpRequest &request, HttpResponse &response) override;
};

#endif // ADDFILEREQUESTHANDLER_H
