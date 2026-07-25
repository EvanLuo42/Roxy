#pragma once

#include <slang-rhi.h>

#include <GLFW/glfw3.h>

#include <string>

namespace Roxy
{

class Window
{
public:
    enum class WindowMode
    {
        Normal,
        Minimized,
        Fullscreen,
    };

    struct Desc
    {
        std::string Title = "Roxy";
        int Width = 1280;
        int Height = 720;

        bool Resizable = true;
    };

    class ICallbacks
    {
    public:
        virtual ~ICallbacks() = default;
        virtual void OnResize(uint32_t width, uint32_t height) = 0;
        virtual void OnClose() = 0;
    };

    explicit Window(Desc desc);

    Window();

    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void OnUpdate();

    [[nodiscard]] bool ShouldClose() const;
    void Close();

    void SetTitle(const std::string &title);

    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;

    [[nodiscard]] rhi::WindowHandle CreateWindowHandle() const;

    [[nodiscard]] GLFWwindow *GetNativeWindow() const;

private:
    void Init();
    void Shutdown();

    GLFWwindow *_window = nullptr;
    Desc _desc;
};

} // namespace Roxy
