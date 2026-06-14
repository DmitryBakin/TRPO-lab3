#include "MainWindow.h"

#include <QApplication>
#include "JsonLoaderAdapter.h".h"
#include <qDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
*/


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    // Теперь загружаем его
    JsonLoaderAdapter adapter;
    QVector<QPointF> data = adapter.load("C:\\GItHub\\TRPO-lab3\\New document 1.json", "");
    qDebug() << "Загружено точек:" << data.size();

    return 0;
}
