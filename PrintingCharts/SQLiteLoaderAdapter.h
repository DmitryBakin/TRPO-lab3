#ifndef SQLITELOADERADAPTER_H
#define SQLITELOADERADAPTER_H

#include <QPointF>

class SQLiteLoaderAdapter
{
public:
    QVector<QPointF> load(const QString& sourcePath, const QString& tableName);
};
#endif // SQLITELOADERADAPTER_H
