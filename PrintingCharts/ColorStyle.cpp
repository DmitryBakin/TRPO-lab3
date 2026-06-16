#include "ColorStyle.h"
#include "ChartWidget.h"

void ColorStyle::apply(QWidget* widget)
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
