#pragma once

#include "RenderGraph/RenderGraph.h"

#include <memory>
#include <slang-rhi.h>
#include <slang.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace Roxy
{
struct RenderView
{
};

class Renderer
{
public:
    Renderer(const rhi::WindowHandle &windowHandle, int width, int height);

    void Render(const RenderView &renderView);

    void SetRenderGraph(RenderGraph renderGraph);

    void OnResize(int width, int height);
    void OnUI();

    [[nodiscard]] const rhi::DeviceInfo &GetDeviceInfo() const { return _device->getInfo(); }

private:
    class DebugCallback : public rhi::IDebugCallback
    {
    public:
        DebugCallback()
        {
            _logger = spdlog::get("RHI");
            if (!_logger)
            {
                _logger = spdlog::default_logger();
            }
        }

        SLANG_NO_THROW void SLANG_MCALL handleMessage(rhi::DebugMessageType type, rhi::DebugMessageSource source,
                                                      const char *message) override
        {
            const char *sourceName = "Unknown";
            switch (source)
            {
            case rhi::DebugMessageSource::Layer:
                sourceName = "Layer";
                break;
            case rhi::DebugMessageSource::Driver:
                sourceName = "Driver";
                break;
            case rhi::DebugMessageSource::Slang:
                sourceName = "Slang";
                break;
            }

            switch (type)
            {
            case rhi::DebugMessageType::Info:
                _logger->info("{}: {}", sourceName, message);
                break;
            case rhi::DebugMessageType::Warning:
                _logger->warn("{}: {}", sourceName, message);
                break;
            case rhi::DebugMessageType::Error:
                _logger->error("{}: {}", sourceName, message);
                break;
            }
        }

    private:
        std::shared_ptr<spdlog::logger> _logger;
    };

    rhi::Result Init();
    rhi::Result InitDevice(rhi::DeviceType deviceType, const std::vector<rhi::Feature> &requiredFeatures,
                           const std::vector<std::pair<std::string, std::string>> &preprocessorMacros);
    rhi::Result InitSurface(rhi::Format format);

    RenderGraph _renderGraph;
    rhi::WindowHandle _windowHandle;

    int _width;
    int _height;

    DebugCallback _debugCallback;
    rhi::ComPtr<rhi::IDevice> _device;
    rhi::ComPtr<rhi::ISurface> _surface;
};

} // namespace Roxy
