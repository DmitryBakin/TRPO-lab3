#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include "IDataLoader.h"

#include <QObject>
#include <QVector>
#include <QPointF>

class ChartModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartModel(QObject *parent = nullptr);
    void loadData(const QString& filePath, const QString& tableName, IDataLoader* loader);
    const QVector<QPointF>& data() const { return m_data; }
    bool isEmpty() const { return m_data.isEmpty(); }
    void clearData();

signals:
    void dataChanged();
    void dataLoaded(int count);
    void errorOccurred(const QString& error);

private:
    QVector<QPointF> m_data;
};

#endif // CHARTMODEL_H
