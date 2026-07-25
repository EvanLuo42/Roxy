#include "Rendering/Renderer.h"
#include "Roxy.h"

#include <format>
#include <spdlog/spdlog.h>
#include <string>

class Viewer : public Roxy::Application
{
public:
    Viewer()
        : Roxy::Application({"Roxy Viewer", 1280, 720, true}),
          _renderer(_window->CreateWindowHandle(), _window->GetWidth(), _window->GetHeight())
    {
        const rhi::DeviceInfo &deviceInfo = _renderer.GetDeviceInfo();

        const std::string title =
            std::format("Roxy Viewer | {} ({})", rhi::getRHI()->getDeviceTypeName(deviceInfo.deviceType),
                        deviceInfo.adapterName ? deviceInfo.adapterName : "Unknown Adapter");
        _window->SetTitle(title);
    }

    void OnInit() override { spdlog::info("OnInit"); }

    void OnUpdate(float deltaTimeSeconds) override {}

    void OnRender() override { _renderer.Render({}); }

    void OnShutdown() override { spdlog::info("OnShutdown"); }

    void OnResize(uint32_t width, uint32_t height) override
    {
        _renderer.OnResize(static_cast<int>(width), static_cast<int>(height));
    }

    void OnClose() override { spdlog::info("OnClose"); }

private:
    Roxy::Renderer _renderer;
};

int main()
{
    Viewer viewer;
    viewer.Run();

    return 0;
}
