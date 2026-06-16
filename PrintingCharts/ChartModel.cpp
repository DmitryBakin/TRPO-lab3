#include "ChartModel.h"


ChartModel::ChartModel(QObject *parent) : QObject(parent) {}

void ChartModel::loadData(const QString& filePath, const QString& tableName, IDataLoader* loader)
{
    if (!loader) {
        emit errorOccurred("Загрузчик не указан");
        return;
    }
    m_data = loader->load(filePath, tableName);
    if (m_data.isEmpty())
        emit errorOccurred("Нет данных или файл пуст");
    else {
        emit dataLoaded(m_data.size());
        emit dataChanged();
    }
}

void ChartModel::clearData()
{
    m_data.clear();
    emit dataChanged();
}
