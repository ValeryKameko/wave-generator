#include "demo_view.h"

#include <glad/gl.h>

namespace wave_generator::view {

DemoView::DemoView() = default;

void DemoView::Render() {
    if (show_demo_window_) {
        ImGui::ShowDemoWindow(&show_demo_window_);
    }

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");  // Create a window called "Hello,
                                        // world!" and append into it.

        ImGui::Text("This is some useful text.");  // Display some text (you can
                                                   // use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &show_demo_window_);  // Edit bools storing our window
                                              // open/close state
        ImGui::Checkbox("Another Window", &show_another_window_);

        ImGui::SliderFloat(
            "float", &f, 0.0f,
            1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3(
            "clear color",
            (float*)&clear_color_);  // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button"))  // Buttons return true when clicked (most widgets
                            // return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (show_another_window_) {
        ImGui::Begin(
            "Another Window",
            &show_another_window_);  // Pass a pointer to our bool variable (the
                                     // window will have a closing button that
                                     // will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) show_another_window_ = false;
        ImGui::End();
    }
    glClearColor(clear_color_.x, clear_color_.y, clear_color_.z,
                 clear_color_.w);
}

}  // namespace wave_generator::view