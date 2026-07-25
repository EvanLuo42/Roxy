#include "Renderer.h"

namespace Roxy
{
Renderer::Renderer(const rhi::WindowHandle &windowHandle, const int width, const int height)
    : _windowHandle(windowHandle), _width(width), _height(height)
{
    if (SLANG_FAILED(Init()))
    {
        throw std::runtime_error("Failed to init Renderer");
    }
}
void Renderer::Render(const RenderView &renderView) {}
void Renderer::SetRenderGraph(RenderGraph renderGraph) {}
void Renderer::OnResize(const int width, const int height)
{
    if (width == _width && height == _height)
    {
        return;
    }

    _width = width;
    _height = height;

    if (width == 0 || height == 0 || !_surface)
    {
        return;
    }

    rhi::SurfaceConfig surfaceConfig;
    surfaceConfig.width = width;
    surfaceConfig.height = height;
    if (SLANG_FAILED(_surface->configure(surfaceConfig)))
    {
        spdlog::error("Failed to reconfigure surface on resize to {}x{}", width, height);
    }
}
void Renderer::OnUI() {}

static std::vector<rhi::Feature> GetRequiredFeatures(const rhi::DeviceType deviceType)
{
    switch (deviceType)
    {
    case rhi::DeviceType::Vulkan:
    case rhi::DeviceType::D3D12:
        return {
            rhi::Feature::Surface,        rhi::Feature::RayQuery, rhi::Feature::AccelerationStructure,
            rhi::Feature::ParameterBlock, rhi::Feature::Bindless, rhi::Feature::PipelineCache,
            rhi::Feature::Rasterization,
        };
    case rhi::DeviceType::Metal:
        return {
            rhi::Feature::Surface,        rhi::Feature::RayQuery,      rhi::Feature::AccelerationStructure,
            rhi::Feature::ParameterBlock, rhi::Feature::Rasterization, rhi::Feature::ArgumentBufferTier2
        };
    default:
        return {};
    }
}

rhi::Result Renderer::Init()
{
#ifndef NDEBUG
    rhi::DebugLayerOptions debugLayerOptions;
    debugLayerOptions.coreValidation = true;
    if (SLANG_FAILED(rhi::getRHI()->setDebugLayerOptions(debugLayerOptions)))
    {
        spdlog::warn("Failed to enable RHI debug layers");
    }
#endif

#if SLANG_APPLE_FAMILY
    const std::vector deviceTypes = {rhi::DeviceType::Metal};
#else
    const std::vector deviceTypes = {rhi::DeviceType::Vulkan, rhi::DeviceType::D3D12};
#endif

    for (const auto deviceType : deviceTypes)
    {
        if (!rhi::getRHI()->isDeviceTypeSupported(deviceType))
        {
            continue;
        }

        const std::vector requiredFeatures = GetRequiredFeatures(deviceType);
        std::vector<std::pair<std::string, std::string>> macros;

        if (SLANG_FAILED(InitDevice(deviceType, requiredFeatures, macros)))
        {
            continue;
        }

        if (!macros.empty())
        {
            _device.setNull();
            if (SLANG_FAILED(InitDevice(deviceType, requiredFeatures, macros)))
            {
                continue;
            }
        }

        if (SLANG_FAILED(InitSurface(rhi::Format::Undefined)))
        {
            continue;
        }

        const rhi::DeviceInfo &deviceInfo = _device->getInfo();
        spdlog::info("Created {} device on {}", rhi::getRHI()->getDeviceTypeName(deviceInfo.deviceType),
                     deviceInfo.adapterName ? deviceInfo.adapterName : "Unknown Adapter");

        return SLANG_OK;
    }

    spdlog::error("Failed to create a device supporting all required features");
    return SLANG_FAIL;
}

static const char *GetFeatureName(const rhi::Feature feature)
{
    switch (feature)
    {
#define ROXY_FEATURE_NAME(name, string)                                                                                \
    case rhi::Feature::name:                                                                                           \
        return string;
        SLANG_RHI_FEATURES(ROXY_FEATURE_NAME)
#undef ROXY_FEATURE_NAME
    default:
        return "unknown";
    }
}

rhi::Result Renderer::InitDevice(rhi::DeviceType deviceType, const std::vector<rhi::Feature> &requiredFeatures,
                                 const std::vector<std::pair<std::string, std::string>> &preprocessorMacros)
{
    rhi::DeviceDesc deviceDesc = {};
    deviceDesc.deviceType = deviceType;
    deviceDesc.debugCallback = &_debugCallback;
#ifndef NDEBUG
    deviceDesc.enableValidation = true;
#endif
    const char *searchPaths[] = {ROXY_SHADER_DIR};
    deviceDesc.slang.searchPaths = searchPaths;
    deviceDesc.slang.searchPathCount = SLANG_COUNT_OF(searchPaths);

    std::vector<slang::PreprocessorMacroDesc> preprocessorMacrosDescs;
    for (const auto &[name, value] : preprocessorMacros)
    {
        slang::PreprocessorMacroDesc desc{};
        desc.name = name.c_str();
        desc.value = value.c_str();
        preprocessorMacrosDescs.push_back(desc);
    }
    deviceDesc.slang.preprocessorMacros = preprocessorMacrosDescs.data();
    deviceDesc.slang.preprocessorMacroCount = preprocessorMacrosDescs.size();

    if (SLANG_FAILED(rhi::getRHI()->createDevice(deviceDesc, _device.writeRef())))
    {
        spdlog::debug("createDevice failed for device type {}", rhi::getRHI()->getDeviceTypeName(deviceType));
        return SLANG_FAIL;
    }

    for (const auto &feature : requiredFeatures)
    {
        if (!_device->hasFeature(feature))
        {
            spdlog::warn("{} device does not support required feature \"{}\"",
                         rhi::getRHI()->getDeviceTypeName(deviceType), GetFeatureName(feature));
            return SLANG_E_NOT_AVAILABLE;
        }
    }

    return SLANG_OK;
}

rhi::Result Renderer::InitSurface(rhi::Format format)
{
    if (SLANG_FAILED(_device->createSurface(_windowHandle, _surface.writeRef())))
    {
        return SLANG_FAIL;
    }

    rhi::SurfaceConfig surfaceConfig;
    surfaceConfig.width = _width;
    surfaceConfig.height = _height;
    surfaceConfig.format = format;
    if (SLANG_FAILED(_surface->configure(surfaceConfig)))
    {
        return SLANG_FAIL;
    }

    return SLANG_OK;
}
} // namespace Roxy