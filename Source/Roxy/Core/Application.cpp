#include "Application.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <memory>
#include <string_view>
#include <utility>

namespace Roxy
{

namespace
{
class UpperLevelFormatter final : public spdlog::custom_flag_formatter
{
public:
    void format(const spdlog::details::log_msg &msg, const std::tm &, spdlog::memory_buf_t &dest) override
    {
        static constexpr std::string_view LEVEL_NAMES[] = {"TRACE", "DEBUG",    "INFO", "WARN",
                                                           "ERROR", "CRITICAL", "OFF"};
        const auto levelName = LEVEL_NAMES[static_cast<size_t>(msg.level)];
        dest.append(levelName.data(), levelName.data() + levelName.size());
    }

    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override
    {
        return spdlog::details::make_unique<UpperLevelFormatter>();
    }
};
} // namespace

Application::Application(Window::Desc windowDesc) : _windowDesc(std::move(windowDesc))
{
    spdlog::set_default_logger(spdlog::stdout_color_mt("Roxy"));
    spdlog::stdout_color_mt("RHI");

    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<UpperLevelFormatter>('L');
    formatter->set_pattern("[%H:%M:%S.%e] [%^%L%$] [%n] %v");
    spdlog::set_formatter(std::move(formatter));
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    _window.emplace(_windowDesc);
    _window->SetCallbacks(this);
}

void Application::Run()
{
    OnInit();

    auto lastFrameTime = std::chrono::steady_clock::now();

    while (!_window->ShouldClose())
    {
        const auto now = std::chrono::steady_clock::now();
        const float deltaTimeSeconds = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        _window->OnUpdate();

        OnUpdate(deltaTimeSeconds);
        OnRender();
    }

    OnShutdown();
}

} // namespace Roxy
