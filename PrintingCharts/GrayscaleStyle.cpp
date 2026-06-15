#include "GrayscaleStyle.h"
#include "ChartWidget.h"

void GrayscaleStyle::apply(QWidget* widget)
{
    ChartWidget* chartWidget = qobject_cast<ChartWidget*>(widget);
    if (chartWidget) {
        chartWidget->setColors(
            getBackgroundColor(),
            getGridColor(),
            getTextColor(),
            getSeriesColor(),
            getAxisColor()
            );
    }
}
