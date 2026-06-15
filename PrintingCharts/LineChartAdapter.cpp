#include "LineChartAdapter.h"
#include "ChartWidget.h"

QWidget* LineChartAdapter::createChartWidget(const QVector<QPointF>& data)
{
    ChartWidget* widget = new ChartWidget();
    widget->setChartType("line");
    widget->setData(data);
    widget->setMinimumSize(500, 350);
    return widget;
}
