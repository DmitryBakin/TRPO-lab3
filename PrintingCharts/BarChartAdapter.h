#ifndef BARCHARTADAPTER_H
#define BARCHARTADAPTER_H

#include "IChartAdapter.h"

class BarChartAdapter : public IChartAdapter {
public:
    QWidget* createChartWidget(const QVector<QPointF>& data) override;
};

#endif // BARCHARTADAPTER_H
