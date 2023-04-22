#ifndef BASE32REQUESTHANDLER_H
#define BASE32REQUESTHANDLER_H

#include "httprequesthandler.h"
#include "ipfsjsonrpc/ipfsjsonrpc.h"
#include <QObject>

using namespace stefanfrings;

class Base32RequestHandler : public HttpRequestHandler
{
public:
    Base32RequestHandler(QObject * parent);

    void service(HttpRequest &request, HttpResponse &response) override;
};

#endif // BASE32REQUESTHANDLER_H
