#include "BarChartAdapter.h"
#include "ChartWidget.h"

QWidget* BarChartAdapter::createChartWidget(const QVector<QPointF>& data)
{
    ChartWidget* widget = new ChartWidget();
    widget->setChartType("bar");
    widget->setData(data);
    widget->setMinimumSize(500, 350);
    return widget;
}
