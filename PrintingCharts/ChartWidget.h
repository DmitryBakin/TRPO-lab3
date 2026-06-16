#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);

    void setData(const QVector<QPointF>& data);
    void setColors(const QColor& background, const QColor& grid,
                   const QColor& text, const QColor& series, const QColor& axis);
    void setChartType(const QString& type);

    QString chartType() const { return m_chartType; }
    int dataSize() const { return m_data.size(); }

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawGrid(QPainter& painter, const QRect& rect, int xTicks, int yTicks);
    void drawAxes(QPainter& painter, const QRect& rect, double minX, double maxX,
                  double minY, double maxY, int xTicks, int yTicks);
    void drawLineChart(QPainter& painter, const QRect& rect,
                       double minX, double maxX, double minY, double maxY);
    void drawBarChart(QPainter& painter, const QRect& rect,
                      double minX, double maxX, double minY, double maxY);

    void calculateMinMax();
    QPoint transformPoint(const QPointF& point, const QRect& rect,
                          double minX, double maxX, double minY, double maxY);

    QVector<QPointF> m_data;

    QColor m_backgroundColor = QColor(255, 255, 255);
    QColor m_gridColor = QColor(200, 200, 200);
    QColor m_textColor = QColor(0, 0, 0);
    QColor m_seriesColor = QColor(100, 150, 200);
    QColor m_axisColor = QColor(0, 0, 0);

    QString m_chartType = "line";

    double m_minX = 0;
    double m_maxX = 1;
    double m_minY = 0;
    double m_maxY = 1;

    int m_leftMargin = 70;
    int m_rightMargin = 50;
    int m_topMargin = 40;
    int m_bottomMargin = 60;
};

#endif // CHARTWIDGET_H
