#include "mainwindow.h"
#include <QApplication>

#include <QStyle>
#include <QDesktopWidget>
#include <QObject>

#include "utilidades.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QObject::connect(&w, SIGNAL(quit()), qApp, SLOT(quit()) );

    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            qApp->desktop()->availableGeometry()
        )
    );

    w.show();

    //xxxx revisar destructores para donde sean necesarios.

    return a.exec();
}
