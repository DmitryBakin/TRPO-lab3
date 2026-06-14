#ifndef SQLITELOADERADAPTER_H
#define SQLITELOADERADAPTER_H

#include <QPointF>
#include "IDataLoader.h"

class SQLiteLoaderAdapter : public IDataLoader
{
public:
    QVector<QPointF> load(const QString& sourcePath, const QString& tableName) override;
};
#endif // SQLITELOADERADAPTER_H
