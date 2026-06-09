#pragma once
#include "IProcessor.h"

class IntelProcessor : public IProcessor {
private:
    std::string m_version;
    ProcessorType m_type;
    double m_speed;

public:
    IntelProcessor(double speed, ProcessorType type, const std::string& version);
    std::string GetProcessorInfo() override;
    double GetSpeed() const override;
    ProcessorType GetType() const override;
    std::string GetVersion() const override;
};
