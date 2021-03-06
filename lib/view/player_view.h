#ifndef WAVEGENERATOR_PLAYER_VIEW_H
#define WAVEGENERATOR_PLAYER_VIEW_H

#include <sound_device.h>

#include <functional>
#include <memory>
#include <string>

#include "view.h"
#include "wav_recorder_view.h"

namespace wave_generator::view {

class PlayerView : public View {
  public:
    using SignalGeneratorPtr = std::unique_ptr<synthesizer::SignalGenerator>;

    explicit PlayerView(model::SoundDevicePtr sound_device, std::shared_ptr<WavRecorderView> wav_recorder);

    void Render() override;
    auto WindowName() -> const std::string&;

  private:
    void RenderWindow();

    std::shared_ptr<model::SoundDevice> sound_device_;
    std::shared_ptr<WavRecorderView> wav_recorder_;
};

}  // namespace wave_generator::view

#endif  // WAVEGENERATOR_PLAYER_VIEW_H
