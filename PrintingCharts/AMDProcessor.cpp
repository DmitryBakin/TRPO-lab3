#include "AMDProcessor.h"
#include <string>

AMDProcessor::AMDProcessor(double speed, ProcessorType type, const std::string& version)
    : m_version(version)
    , m_type(type)
    , m_speed(speed)
{}

std::string AMDProcessor::GetProcessorInfo() {
    std::string typeStr;
    if (m_type == ProcessorType::x86) {
        typeStr = "x86";
    } else {
        typeStr = "x64";
    }

    return "AMD Processor: " + m_version + ", " + std::to_string(m_speed) + " GHz, " + typeStr;
}

double AMDProcessor::GetSpeed() const { return m_speed; }
ProcessorType AMDProcessor::GetType() const { return m_type; }
std::string AMDProcessor::GetVersion() const { return m_version; }
