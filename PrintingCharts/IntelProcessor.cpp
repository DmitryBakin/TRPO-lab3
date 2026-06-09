#include "IntelProcessor.h"
#include <string>

IntelProcessor::IntelProcessor(double speed, ProcessorType type, const std::string& version)
    : m_version(version)
    , m_type(type)
    , m_speed(speed)
{}

std::string IntelProcessor::GetProcessorInfo() {
    std::string typeStr;

    if (m_type == ProcessorType::x86) {
        typeStr = "x86";
    } else {
        typeStr = "x64";
    }

    return "Intel Processor: " + m_version + ", " + std::to_string(m_speed) + " GHz, " + typeStr;
}

double IntelProcessor::GetSpeed() const { return m_speed; }
ProcessorType IntelProcessor::GetType() const { return m_type; }
std::string IntelProcessor::GetVersion() const { return m_version; }
