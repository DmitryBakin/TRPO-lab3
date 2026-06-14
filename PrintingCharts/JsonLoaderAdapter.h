#ifndef JSONLOADERADAPTER_H
#define JSONLOADERADAPTER_H

#include <QPointF>

class JsonLoaderAdapter
{
public:
    QVector<QPointF> load(const QString& sourcePath, const QString& tableName);
};

#endif // JSONLOADERADAPTER_H
