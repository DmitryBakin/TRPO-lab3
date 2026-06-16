#include "JsonLoaderAdapter.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

QVector<QPointF> JsonLoaderAdapter::load(const QString& sourcePath, const QString&)
{
    QVector<QPointF> points;
    QFile file(sourcePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть JSON файл:" << sourcePath;
        return points;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);

    if (doc.isNull()) {
        qWarning() << "Ошибка парсинга JSON. Файл может содержать синтаксические ошибки.";
        return points;
    }

    if (!doc.isArray()) {
        qWarning() << "Корневой элемент JSON не является массивом. Ожидается формат [ { ... }, ... ]";
        return points;
    }

    QJsonArray jsonArray = doc.array();

    for (const QJsonValue& value : jsonArray) {
        if (!value.isObject()) {
            qWarning() << "Элемент массива не является JSON-объектом. Пропускаем.";
            continue;
        }

        QJsonObject obj = value.toObject();

        double val = obj["value"].toDouble();
        QString dateString = obj["date"].toString();

        QDateTime dt = QDateTime::fromString(dateString, Qt::ISODate);
        if (!dt.isValid()) {
            dt = QDateTime::fromString(dateString, "yyyy-MM-dd");
        }

        if (dt.isValid()) {
            qint64 timestamp = dt.toMSecsSinceEpoch();
            points.append(QPointF(static_cast<double>(timestamp), val));
        } else {
            qWarning() << "Не удалось распарсить дату:" << dateString;
        }
    }

    return points;
}
