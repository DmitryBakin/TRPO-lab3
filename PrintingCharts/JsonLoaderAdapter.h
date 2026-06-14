#ifndef JSONLOADERADAPTER_H
#define JSONLOADERADAPTER_H

#include "IDataLoader.h"

class JsonLoaderAdapter : public IDataLoader
{
public:
    QVector<QPointF> load(const QString& sourcePath, const QString& tableName) override;
};

#endif // JSONLOADERADAPTER_H
