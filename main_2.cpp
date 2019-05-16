/* Entry point for the application, it creates the applications frontend
and its event loop*/

#include <QtGui/QApplication>
#include "nunchuckfrontend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Nunchuckfrontend nf; // A Qwidget extension, represents the applications frontend
    nf.show();
    return a.exec();
}
