#include "stdafx.h"
#include "Downloader.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Downloader window;
    window.show();
    return app.exec();
}