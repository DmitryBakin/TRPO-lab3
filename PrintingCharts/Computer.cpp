#include "Computer.h"
#include <QDebug>
#include <QString>

Computer::Computer(std::shared_ptr<IProcessor> processor)
    : m_processor(processor)
{}

void Computer::ShowInfo() {
    qDebug() << QStringLiteral("Конфигурация компьютера:");
    qDebug() << QStringLiteral("  ") + QString::fromStdString(m_processor->GetProcessorInfo());
}

void Computer::RunBenchmark() {
    if (m_processor->GetSpeed() >= 3.0) {
        qDebug() << QStringLiteral("Режим высокой производительности - ") + QString::fromStdString(m_processor->GetProcessorInfo());
    } else {
        qDebug() << QStringLiteral("Обычный режим - ") + QString::fromStdString(m_processor->GetProcessorInfo());
    }
}
