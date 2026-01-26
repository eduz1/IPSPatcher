#include "stdafx.h"
#include "IPSPatcherUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    IPSPatcherUI window;
    window.show();
    return app.exec();
}
