#include "SQLiteLoaderAdapter.h"
#include <QtSql>
#include <QDateTime>
#include <QDebug>

QVector<QPointF> SQLiteLoaderAdapter::load(const QString& sourcePath, const QString& tableName)
{
    QVector<QPointF> points;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(sourcePath);

    if (!db.open()) {
        qDebug() << "Ошибка открытия БД:" << db.lastError().text();
        return points;
    }


    QString queryStr = QString("SELECT \"Time\", \"Value\" FROM \"%1\" ORDER BY \"Time\"").arg(tableName);

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Ошибка запроса:" << query.lastError().text();
        db.close();
        return points;
    }

    while (query.next()) {
        QString timeStr = query.value(0).toString();
        double value = query.value(1).toDouble();
        QDateTime dt = QDateTime::fromString(timeStr, "dd.MM.yyyy hh:mm");

        if (dt.isValid()) {
            points.append(QPointF(dt.toMSecsSinceEpoch(), value));
            qDebug() << "Загружено:" << dt.toString("yyyy-MM-dd hh:mm") << "->" << value;
        } else {
            qDebug() << "Не удалось распознать дату:" << timeStr;
        }
    }

    db.close();
    qDebug() << "Загружено точек:" << points.size();
    return points;
}
