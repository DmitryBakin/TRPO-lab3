#include "IOC_Contaner.h"
#include "Computer.h"
#include "IntelProcessor.h"
#include "AMDProcessor.h"
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <vector>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Настройка для русского языка в Qt
    qputenv("QT_MESSAGE_PATTERN", "%{message}");

    // Создаем IoC контейнер
    IOCContainer container;

    // ----- Пример 1: Регистрация синглтона процессора -----
    qDebug() << QStringLiteral("=== Пример 1: Синглтон процессора ===");

    // Создаем конкретный процессор Intel
    auto intelProc = std::make_shared<IntelProcessor>(3.5, ProcessorType::x64, "Core i7-12700K");

    // Регистрируем его в контейнере как IProcessor
    container.RegisterInstance<IProcessor>(intelProc);

    // Регистрируем фабрику для создания компьютера с внедрением зависимости
    container.RegisterFactory<Computer, Computer, IProcessor>();

    // Получаем компьютер из контейнера (зависимость IProcessor внедряется автоматически)
    auto computer1 = container.GetObject<Computer>();
    computer1->ShowInfo();

    // ----- Пример 2: Замена на AMD процессор -----
    qDebug() << QStringLiteral("");
    qDebug() << QStringLiteral("=== Пример 2: Замена на AMD процессор (без изменения кода Computer!) ===");

    // Меняем реализацию - теперь AMD
    auto amdProc = std::make_shared<AMDProcessor>(4.0, ProcessorType::x64, "Ryzen 9 7950X");
    container.RegisterInstance<IProcessor>(amdProc);

    auto computer2 = container.GetObject<Computer>();
    computer2->ShowInfo();

    // ----- Пример 3: Несколько компьютеров с разными процессорами -----
    qDebug() << QStringLiteral("");
    qDebug() << QStringLiteral("=== Пример 3: Несколько компьютеров с разными процессорами ===");

    // Создаем компьютеры напрямую с разными процессорами
    auto laptopProc = std::make_shared<IntelProcessor>(2.5, ProcessorType::x64, "Core i5-1235U");
    auto desktopProc = std::make_shared<AMDProcessor>(3.8, ProcessorType::x64, "Ryzen 7 5800X");

    Computer laptop(laptopProc);
    Computer desktop(desktopProc);

    laptop.ShowInfo();
    desktop.ShowInfo();

    // ----- Пример 4: Динамическая смена процессора -----
    qDebug() << QStringLiteral("");
    qDebug() << QStringLiteral("=== Пример 4: Динамическая смена процессора во время выполнения ===");

    // Вектор различных процессоров
    std::vector<std::shared_ptr<IProcessor>> processors = {
        std::make_shared<IntelProcessor>(2.0, ProcessorType::x86, "Pentium"),
        std::make_shared<AMDProcessor>(2.5, ProcessorType::x64, "Athlon"),
        std::make_shared<IntelProcessor>(3.0, ProcessorType::x64, "Core i5"),
        std::make_shared<AMDProcessor>(3.5, ProcessorType::x64, "Ryzen 5")
    };

    // Перебираем разные процессоры и создаем компьютеры через контейнер
    for (size_t i = 0; i < processors.size(); ++i) {
        container.RegisterInstance<IProcessor>(processors[i]);
        auto computer = container.GetObject<Computer>();
        qDebug() << QStringLiteral("Компьютер ") + QString::number(i + 1) + QStringLiteral(":");
        computer->ShowInfo();
    }

    // ----- Пример 5: Завершение работы -----
    qDebug() << QStringLiteral("");
    qDebug() << QStringLiteral("=== Программа успешно завершена ===");

    return 0;
}
