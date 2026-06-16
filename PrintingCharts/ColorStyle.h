#ifndef COLORSTYLE_H
#define COLORSTYLE_H

#include "IStyleStrategy.h"

class ColorStyle : public IStyleStrategy {
public:
    void apply(QWidget* widget) override;
    QColor getBackgroundColor() const override { return QColor(255, 255, 255); }
    QColor getGridColor() const override { return QColor(200, 200, 200); }
    QColor getTextColor() const override { return QColor(0, 0, 0); }
    QColor getSeriesColor() const override { return QColor(100, 150, 200); }
    QColor getAxisColor() const override { return QColor(0, 0, 0); }
};

#endif // COLORSTYLE_H
