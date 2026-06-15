#ifndef ISTYLESTRATEGY_H
#define ISTYLESTRATEGY_H

#include <QWidget>

class IStyleStrategy {
public:
    virtual ~IStyleStrategy() = default;
    virtual void apply(QWidget* widget) = 0;
    virtual QColor getBackgroundColor() const = 0;
    virtual QColor getGridColor() const = 0;
    virtual QColor getTextColor() const = 0;
    virtual QColor getSeriesColor() const = 0;
    virtual QColor getAxisColor() const = 0;
};

#endif // ISTYLESTRATEGY_H
