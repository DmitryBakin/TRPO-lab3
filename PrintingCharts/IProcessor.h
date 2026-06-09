#pragma once
#include <string>

enum class ProcessorType { x86, x64 };

class IProcessor {
public:
    virtual ~IProcessor() {}
    virtual std::string GetProcessorInfo() = 0;
    virtual double GetSpeed() const = 0;
    virtual ProcessorType GetType() const = 0;
    virtual std::string GetVersion() const = 0;
};
