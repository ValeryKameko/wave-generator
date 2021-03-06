#include "node_port_view.h"

#include <imgui_internal.h>

#include <utility>

#include "node_view.h"

namespace wave_generator::view::node {

static const ImColor kInputPortBackgroundColor = {150, 150, 150};
static const ImColor kInputPortBorderColor = {170, 170, 170};
static const ImColor kLinkColor = {150, 150, 150};
static const ImVec2 kLinkBezierOffset = {50, 0};
static const float kLinkThickness = 3;
static const float kPortRadius = 6;

int NodeInputView::counter_id_ = 0;

static void RenderPort(ImDrawList* draw_list, ImVec2 position, ImGuiButtonFlags flags = 0) {
    const auto port_size = ImVec2{kPortRadius, kPortRadius} * 2;
    ImGui::SetCursorScreenPos(position - port_size / 2);

    draw_list->AddCircleFilled(position, kPortRadius, kInputPortBackgroundColor);
    draw_list->AddCircle(position, kPortRadius, kInputPortBorderColor);

    ImGui::InvisibleButton("port_button", port_size, flags);
}

static void RenderLink(ImDrawList* draw_list, ImVec2 from_position, ImVec2 to_position) {
    draw_list->AddBezierCurve(from_position, from_position + kLinkBezierOffset,
                              to_position - kLinkBezierOffset, to_position, kLinkColor,
                              kLinkThickness);
}

NodeInputView::NodeInputView(const NodeView* parent, std::string name, bool has_port)
    : parent_{parent}, id_{GetID()}, name_{std::move(name)}, has_port_{has_port} {}

void NodeInputView::Render(ImDrawList* draw_list) {
    is_topology_changed_ = false;
    ImGui::PushID(id_);

    draw_list->ChannelsSetCurrent(foreground_channel_);
    ImGui::BeginGroup();
    RenderItem(draw_list);
    ImGui::EndGroup();

    if (has_port_) {
        auto item_rect = ImRect{ImGui::GetItemRectMin(), ImGui::GetItemRectMax()};
        port_position_ = {parent_->GetOuterRect().Min.x, item_rect.GetCenter().y};

        draw_list->ChannelsSetCurrent(foreground_channel_);
        RenderPort(draw_list, port_position_, ImGuiButtonFlags_PressedOnClickRelease);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            Disconnect();
        }

        if (connected_output_ != nullptr) {
            RenderLink(draw_list, connected_output_->GetPortPosition(), port_position_);
        }
    }

    ImGui::PopID();
}

void NodeInputView::Connect(NodeOutputView* output) {
    if (connected_output_ == nullptr) {
        connected_output_ = output;
        output->Connect(this);
        is_topology_changed_ = true;
    }
}

auto NodeInputView::GetName() const -> const std::string& { return name_; }

auto NodeInputView::GetID() -> int { return counter_id_++; }

auto NodeInputView::GetPortPosition() const -> ImVec2 { return port_position_; }

void NodeInputView::Disconnect() {
    if (connected_output_ != nullptr) {
        connected_output_->Disconnect(this);
        connected_output_ = nullptr;
        is_topology_changed_ = true;
    }
}

auto NodeInputView::GetPortSize() const -> ImVec2 { return {2 * kPortRadius, 2 * kPortRadius}; }

auto NodeInputView::GetPortRect() const -> ImRect {
    auto rect = ImRect{GetPortPosition(), GetPortPosition()};
    rect.Expand(GetPortSize() / 2);
    return rect;
}

auto NodeInputView::CanConnect(const NodeOutputView* output) const -> bool { return false; }

auto NodeInputView::HasPort() const -> bool { return has_port_; }

auto NodeInputView::GetConnectedNode() const -> const NodeView* {
    if (connected_output_ == nullptr) return nullptr;
    return connected_output_->GetNode();
}

void NodeInputView::SetChannels(int foreground_channel, int background_channel) {
    foreground_channel_ = foreground_channel;
    background_channel_ = background_channel;
}

auto NodeInputView::IsTopologyChanged() const -> bool { return is_topology_changed_; }

NodeOutputView::NodeOutputView(const NodeView* parent, std::string name)
    : parent_{parent}, name_{std::move(name)} {}

void NodeOutputView::Render(ImDrawList* draw_list) {
    is_topology_changed_ = false;
    draw_list->ChannelsSetCurrent(foreground_channel_);

    ImGui::BeginGroup();
    ImGui::Text("%s", name_.c_str());
    ImGui::EndGroup();

    auto item_rect = ImRect{ImGui::GetItemRectMin(), ImGui::GetItemRectMax()};
    position_ = {parent_->GetOuterRect().Max.x, item_rect.GetCenter().y};

    draw_list->ChannelsSetCurrent(foreground_channel_);
    RenderPort(draw_list, position_);

    is_connecting_ = ImGui::IsItemDeactivated();
    if (ImGui::IsItemActive()) {
        RenderLink(draw_list, position_, ImGui::GetIO().MousePos);
    }
}

auto NodeOutputView::GetPortPosition() const -> ImVec2 { return position_; }

auto NodeOutputView::IsConnecting() const -> bool { return is_connecting_; }

auto NodeOutputView::GetPortRect() const -> ImRect {
    auto rect = ImRect{GetPortPosition(), GetPortPosition()};
    rect.Expand(GetPortSize() / 2);
    return rect;
}

auto NodeOutputView::GetPortSize() const -> ImVec2 { return {2 * kPortRadius, 2 * kPortRadius}; }

void NodeOutputView::Connect(NodeInputView* input) {
    if (connected_inputs_.find(input) == std::end(connected_inputs_)) {
        connected_inputs_.insert(input);
        input->Connect(this);
        is_topology_changed_ = true;
    }
}

void NodeOutputView::Disconnect(NodeInputView* input) {
    if (auto it = connected_inputs_.find(input); it != std::end(connected_inputs_)) {
        connected_inputs_.erase(it);
        input->Disconnect();
        is_topology_changed_ = true;
    }
}

void NodeOutputView::DisconnectAll() {
    while (!connected_inputs_.empty()) {
        Disconnect(*std::begin(connected_inputs_));
    }
}

auto NodeOutputView::GetNode() const -> const NodeView* { return parent_; }

void NodeOutputView::SetChannels(int foreground_channel, int background_channel) {
    foreground_channel_ = foreground_channel;
    background_channel_ = background_channel;
}

auto NodeOutputView::IsTopologyChanged() const -> bool { return is_topology_changed_; }

}  // namespace wave_generator::view::node
