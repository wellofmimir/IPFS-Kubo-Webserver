#include <QCoreApplication>
#include "QSettings"

#include "httplistener.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

int main(int argc, char *argv[])
{
    QCoreApplication app {argc, argv};
    new HttpListener {new QSettings{"settings.ini", QSettings::IniFormat, &app}, new HttpRequestHandler{&app}, &app};
    return app.exec();
}
