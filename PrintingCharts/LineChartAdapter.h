#ifndef LINECHARTADAPTER_H
#define LINECHARTADAPTER_H

#include "IChartAdapter.h"

class LineChartAdapter : public IChartAdapter {
public:
    QWidget* createChartWidget(const QVector<QPointF>& data) override;
};

#endif // LINECHARTADAPTER_H
