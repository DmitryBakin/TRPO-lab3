#include "ChartWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QDateTime>
#include <cmath>
#include <algorithm>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(500, 350);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ChartWidget::setData(const QVector<QPointF>& data)
{
    m_data = data;
    std::sort(m_data.begin(), m_data.end(),
              [](const QPointF& a, const QPointF& b) { return a.x() < b.x(); });
    calculateMinMax();
    update();
}

void ChartWidget::setColors(const QColor& background, const QColor& grid,
                            const QColor& text, const QColor& series, const QColor& axis)
{
    m_backgroundColor = background;
    m_gridColor = grid;
    m_textColor = text;
    m_seriesColor = series;
    m_axisColor = axis;
    update();
}

void ChartWidget::setChartType(const QString& type)
{
    m_chartType = type;
    update();
}

void ChartWidget::calculateMinMax()
{
    if (m_data.isEmpty()) {
        m_minX = 0;
        m_maxX = 1;
        m_minY = 0;
        m_maxY = 1;
        return;
    }

    // Находим минимальные и максимальные значения
    m_minX = m_data[0].x();
    m_maxX = m_data[0].x();
    m_minY = m_data[0].y();
    m_maxY = m_data[0].y();

    for (const QPointF& p : m_data) {
        if (p.x() < m_minX) m_minX = p.x();
        if (p.x() > m_maxX) m_maxX = p.x();
        if (p.y() < m_minY) m_minY = p.y();
        if (p.y() > m_maxY) m_maxY = p.y();
    }

    // Добавляем отступы (10% с каждой стороны)
    double xRange = m_maxX - m_minX;
    double yRange = m_maxY - m_minY;

    if (xRange < 0.001) xRange = 1.0;
    if (yRange < 0.001) yRange = 1.0;

    m_minX -= xRange * 0.05;
    m_maxX += xRange * 0.05;
    m_minY -= yRange * 0.05;
    m_maxY += yRange * 0.05;

    // Если все значения положительные, начинаем ось Y с 0
    if (m_minY > 0) m_minY = 0;
}

QPoint ChartWidget::transformPoint(const QPointF& point, const QRect& rect,
                                   double minX, double maxX, double minY, double maxY)
{
    int x = rect.left() + static_cast<int>((point.x() - minX) / (maxX - minX) * rect.width());
    int y = rect.bottom() - static_cast<int>((point.y() - minY) / (maxY - minY) * rect.height());
    return QPoint(x, y);
}

void ChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Если нет данных, показываем сообщение
    if (m_data.isEmpty()) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "Нет данных для отображения\n\nВыберите файл БД в левой панели");
        return;
    }

    // Заливка фона
    painter.fillRect(rect(), m_backgroundColor);

    // Область для графика
    QRect plotRect(
        m_leftMargin,
        m_topMargin,
        width() - m_leftMargin - m_rightMargin,
        height() - m_topMargin - m_bottomMargin
        );

    // Количество делений на осях
    int xTicks = std::min(10, m_data.size());
    int yTicks = 8;

    // Рисуем сетку
    drawGrid(painter, plotRect, xTicks, yTicks);

    // Рисуем оси и подписи
    drawAxes(painter, plotRect, m_minX, m_maxX, m_minY, m_maxY, xTicks, yTicks);

    // Рисуем график в зависимости от типа
    if (m_chartType == "line") {
        drawLineChart(painter, plotRect, m_minX, m_maxX, m_minY, m_maxY);
    } else if (m_chartType == "bar") {
        drawBarChart(painter, plotRect, m_minX, m_maxX, m_minY, m_maxY);
    }
}

void ChartWidget::drawGrid(QPainter& painter, const QRect& rect, int xTicks, int yTicks)
{
    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));

    // Вертикальные линии (X)
    for (int i = 0; i <= xTicks; ++i) {
        int x = rect.left() + (i * rect.width()) / xTicks;
        painter.drawLine(x, rect.top(), x, rect.bottom());
    }

    // Горизонтальные линии (Y)
    for (int i = 0; i <= yTicks; ++i) {
        int y = rect.top() + (i * rect.height()) / yTicks;
        painter.drawLine(rect.left(), y, rect.right(), y);
    }
}

void ChartWidget::drawAxes(QPainter& painter, const QRect& rect,
                           double minX, double maxX, double minY, double maxY,
                           int xTicks, int yTicks)
{
    // Рисуем оси
    painter.setPen(QPen(m_axisColor, 2));
    painter.drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());  // ось X
    painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());      // ось Y

    // Подписи осей
    painter.setPen(m_textColor);
    painter.setFont(QFont("Arial", 10));

    // Название оси X
    painter.drawText(rect.center().x() - 30, rect.bottom() + 35, 60, 20,
                     Qt::AlignCenter, "Дата");

    // Название оси Y (с поворотом)
    painter.save();
    painter.translate(20, rect.center().y());
    painter.rotate(-90);
    painter.drawText(-30, 0, 60, 20, Qt::AlignCenter, "Значение");
    painter.restore();

    // Подписи на оси X
    painter.setFont(QFont("Arial", 8));
    for (int i = 0; i <= xTicks; ++i) {
        int x = rect.left() + (i * rect.width()) / xTicks;
        double value = minX + (i * (maxX - minX)) / xTicks;

        QDateTime dt;
        dt.setMSecsSinceEpoch(static_cast<qint64>(value));
        QString label = dt.toString("dd.MM.yy");

        painter.drawText(x - 25, rect.bottom() + 5, 50, 20,
                         Qt::AlignCenter, label);
    }

    // Подписи на оси Y
    for (int i = 0; i <= yTicks; ++i) {
        int y = rect.bottom() - (i * rect.height()) / yTicks;
        double value = minY + (i * (maxY - minY)) / yTicks;
        QString label = QString::number(value, 'f', 1);

        painter.drawText(rect.left() - 50, y - 8, 45, 20,
                         Qt::AlignRight, label);
    }
}

void ChartWidget::drawLineChart(QPainter& painter, const QRect& rect,
                                double minX, double maxX, double minY, double maxY)
{
    if (m_data.size() < 2) return;

    // Строим путь для линии
    QPainterPath path;
    QPoint firstPoint = transformPoint(m_data[0], rect, minX, maxX, minY, maxY);
    path.moveTo(firstPoint);

    for (int i = 1; i < m_data.size(); ++i) {
        path.lineTo(transformPoint(m_data[i], rect, minX, maxX, minY, maxY));
    }

    // Рисуем линию
    painter.setPen(QPen(m_seriesColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);

    // Рисуем маркеры точек
    painter.setBrush(QBrush(m_seriesColor));
    painter.setPen(QPen(m_axisColor, 0.5));

    for (const QPointF& p : m_data) {
        QPoint screenPoint = transformPoint(p, rect, minX, maxX, minY, maxY);
        painter.drawEllipse(screenPoint, 2, 2);
    }
}

void ChartWidget::drawBarChart(QPainter& painter, const QRect& rect,
                               double minX, double maxX, double minY, double maxY)
{
    if (m_data.isEmpty()) return;

    double barWidth = static_cast<double>(rect.width()) / m_data.size() * 0.7;

    painter.setBrush(QBrush(m_seriesColor));
    painter.setPen(QPen(m_axisColor, 1));

    for (const QPointF& p : m_data) {
        double xRatio = (p.x() - minX) / (maxX - minX);
        int barX = rect.left() + static_cast<int>(xRatio * rect.width() - barWidth / 2);
        int barHeight = static_cast<int>((p.y() - minY) / (maxY - minY) * rect.height());
        int barY = rect.bottom() - barHeight;

        if (barHeight < 0) {
            barY = rect.bottom();
            barHeight = 0;
        }

        painter.fillRect(barX, barY, static_cast<int>(barWidth), barHeight, m_seriesColor);
        painter.drawRect(barX, barY, static_cast<int>(barWidth), barHeight);

        // Подпись значения над столбцом
        painter.setPen(m_textColor);
        painter.drawText(barX, barY - 5, static_cast<int>(barWidth), 20,
                         Qt::AlignCenter, QString::number(p.y(), 'f', 1));
        painter.setPen(QPen(m_axisColor, 1));
    }
}

void ChartWidget::drawScatterChart(QPainter& painter, const QRect& rect,
                                   double minX, double maxX, double minY, double maxY)
{
    if (m_data.isEmpty()) return;

    painter.setBrush(QBrush(m_seriesColor));
    painter.setPen(QPen(m_seriesColor, 2));

    for (const QPointF& p : m_data) {
        QPoint screenPoint = transformPoint(p, rect, minX, maxX, minY, maxY);
        painter.drawEllipse(screenPoint, 7, 7);
    }

    // Дополнительно можно соединить линией (опционально)
    if (m_data.size() >= 2) {
        painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));
        for (int i = 1; i < m_data.size(); ++i) {
            QPoint p1 = transformPoint(m_data[i-1], rect, minX, maxX, minY, maxY);
            QPoint p2 = transformPoint(m_data[i], rect, minX, maxX, minY, maxY);
            painter.drawLine(p1, p2);
        }
    }
}

void ChartWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    update();
}
