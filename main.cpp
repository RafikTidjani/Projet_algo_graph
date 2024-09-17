#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QIcon logoApp{":assets/Logo_Graphe.png"};
    a.setWindowIcon(logoApp);

    return a.exec();
}
