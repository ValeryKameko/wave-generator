#include "pulse_generator.h"

#include <cmath>

#include "constant_generator.h"
#include "integrator.h"

namespace wave_generator::synthesizer {

PulseGenerator::PulseGenerator(double duty_cycle, double base_amplitude, double base_frequency,
                               std::unique_ptr<SignalGenerator> amplitude_signal,
                               std::unique_ptr<SignalGenerator> frequency_signal)
    : AmplitudeFrequencyGenerator(base_amplitude, base_frequency, std::move(amplitude_signal),
                                  std::move(frequency_signal)),
      duty_cycle_{duty_cycle} {}

auto PulseGenerator::Evaluate(double phase) -> double {
    return fmod(phase, 1.0f) / (1.0f) > duty_cycle_ ? -1.0 : 1.0;
}

}  // namespace wave_generator::synthesizer
