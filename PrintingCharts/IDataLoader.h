#ifndef IDATALOADER_H
#define IDATALOADER_H

#include <QPointF>

class IDataLoader {
public:
    virtual ~IDataLoader() = default;
    virtual QVector<QPointF> load(const QString& sourcePath, const QString& tableName) = 0;
};

#endif // IDATALOADER_H
