#pragma once

#include <juce_core/juce_core.h>

namespace mrta
{

struct ParameterInfo
{
    enum Type : uint32_t
    {
        Float = 0,
        Choice,
        Bool
    };

    // Float ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name, const juce::String& _unit,
                  float _def, float _min, float _max,
                  float _inc, float _skw);

    // Choice ctor
    ParameterInfo( const juce::String& _ID, const juce::String& _name, const juce::StringArray& _steps, unsigned int _def);

    // Bool ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name,
                  const juce::String& offStepName, const juce::String& onStepName,
                  bool _def);

    // Generic ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name, const juce::String& _unit, const juce::String& _steps,
                  Type _type,
                  float _def, float _min, float _max,
                  float _inc, float _skw);

    // Copy ctor
    ParameterInfo(const ParameterInfo& other);

    // No move ctor
    ParameterInfo(ParameterInfo&& other);

    // No default ctor
    ParameterInfo() = delete;

    // No copy assign
    const ParameterInfo& operator=(const ParameterInfo&) = delete;

    // No move asign
    const ParameterInfo& operator=(ParameterInfo&) = delete;

    ~ParameterInfo() noexcept { }

    const juce::String ID;
    const juce::String name;
    const juce::String unit;
    const juce::StringArray steps;

    const Type type;

    const float def;
    const float min;
    const float max;
    const float inc;
    const float skw;
};

}
