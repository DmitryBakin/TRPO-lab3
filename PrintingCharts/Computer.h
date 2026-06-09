#pragma once
#include "IProcessor.h"
#include <memory>

class Computer {
private:
    std::shared_ptr<IProcessor> m_processor;

public:
    Computer(std::shared_ptr<IProcessor> processor);

    void ShowInfo();
    void RunBenchmark();
};
