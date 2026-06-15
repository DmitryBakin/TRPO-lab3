#ifndef ICHARTADAPTER_H
#define ICHARTADAPTER_H

#include <QVector>
#include <QPointF>
#include <QWidget>

class IChartAdapter {
public:
    virtual ~IChartAdapter() = default;
    virtual QWidget* createChartWidget(const QVector<QPointF>& data) = 0;
};

#endif // ICHARTADAPTER_H
