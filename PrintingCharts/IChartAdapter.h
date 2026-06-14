#ifndef ICHARTADAPTER_H
#define ICHARTADAPTER_H

#include <QVector>
#include <QPointF>
#include <memory>

namespace QtCharts
{
class QChart;
}

class IChartAdapter {
public:
    virtual ~IChartAdapter() = default;
    virtual std::unique_ptr<QtCharts::QChart> createChart(const QVector<QPointF>& data) = 0;
};
#endif // ICHARTADAPTER_H
