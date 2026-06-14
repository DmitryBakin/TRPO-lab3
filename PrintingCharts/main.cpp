#include "MainWindow.h"

#include <QApplication>
#include "SQLiteLoaderAdapter.h"
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

    QString dbPath = "C:\\GItHub\\TRPO-lab3\\HUMIDITY_MOSCOW.sqlite";

    SQLiteLoaderAdapter adapter;
    QVector<QPointF> data = adapter.load(dbPath, "HUMIDITY_MOSCOW");

    qDebug() << "Всего точек:" << data.size();

    return 0;
}
