#ifndef GRAYSCALESTYLE_H
#define GRAYSCALESTYLE_H

#include "IStyleStrategy.h"

class GrayscaleStyle : public IStyleStrategy {
public:
    void apply(QWidget* widget) override;
    QColor getBackgroundColor() const override { return QColor(255, 255, 255); }
    QColor getGridColor() const override { return QColor(200, 200, 200); }
    QColor getTextColor() const override { return QColor(2, 2, 2); }
    QColor getSeriesColor() const override { return QColor(200, 200, 200); }
    QColor getAxisColor() const override { return QColor(0, 0, 0); }
};

#endif // GRAYSCALESTYLE_H
