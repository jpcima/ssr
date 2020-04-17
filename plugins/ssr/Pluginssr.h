#pragma once
#include "DistrhoPlugin.hpp"
#include <memory>
#include <atomic>
namespace sfz { namespace fx { class ResonantArray; } }

class SsrState;

class Pluginssr : public Plugin {
public:
    enum Parameters { pidSustainAmount, paramCount };
    enum { stateCount = 1 };

    Pluginssr();

protected:
    // -------------------------------------------------------------------
    // Information

    const char *getLabel() const noexcept override
    {
        return "ssr";
    }

    const char *getDescription() const override
    {
        return "A sympathetic string resonator";
    }

    const char *getMaker() const noexcept override
    {
        return "JPC";
    }

    const char *getHomePage() const override
    {
        return "http://jpcima.sdf1.org/plugins/ssr";
    }

    const char *getLicense() const noexcept override
    {
        return "https://spdx.org/licenses/BSD-2-Clause";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(0, 1, 0);
    }

    // Go to:
    //
    // http://service.steinberg.de/databases/plugin.nsf/plugIn
    //
    // Get a proper plugin UID and fill it in here!
    int64_t getUniqueId() const noexcept override
    {
        return d_cconst(' ', 's', 's', 'r');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter &parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    void initState(uint32_t index, String &stateKey, String &defaultStateValue) override;
    void setState(const char *key, const char *value) override;
    String getState(const char *key) const override;

    // -------------------------------------------------------------------
    // Optional

    // Optional callback to inform the plugin about a sample rate change.
    void sampleRateChanged(double newSampleRate) override;

    // -------------------------------------------------------------------
    // Process

    void activate() override;

    void run(const float **inputs, float **outputs, uint32_t frames) override;


    // -------------------------------------------------------------------

private:
    void runWithinBufferLimits(const float **inputs, float **outputs, uint32_t numFrames);

    void updateStringsSetup();

private:
    float fParams[paramCount];
    double fSampleRate;

    std::unique_ptr<sfz::fx::ResonantArray> fStringsArray;
    std::unique_ptr<SsrState> fState;
    std::unique_ptr<float[]> fFeedbacks;

    std::unique_ptr<float[]> fTemp1;
    std::unique_ptr<float[]> fTemp2;

    std::atomic<int> fStringsNeedSetup {0};

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pluginssr)
};

struct Preset {
    const char *name;
    float params[Pluginssr::paramCount];
};
