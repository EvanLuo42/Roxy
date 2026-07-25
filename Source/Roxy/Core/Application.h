#pragma once

#include "Window.h"

#include <optional>

namespace Roxy
{

class Application : public Window::ICallbacks
{
public:
    explicit Application(Window::Desc windowDesc = {});
    ~Application() override = default;

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Run();

    [[nodiscard]] Window *GetWindow() { return _window.has_value() ? &_window.value() : nullptr; }

    virtual void OnInit() {}
    virtual void OnUpdate(float deltaTimeSeconds) {}
    virtual void OnRender() {}
    virtual void OnShutdown() {}

    void OnResize(uint32_t width, uint32_t height) override {}
    void OnClose() override {}

protected:
    std::optional<Window> _window;

private:
    Window::Desc _windowDesc;
};

} // namespace Roxy
