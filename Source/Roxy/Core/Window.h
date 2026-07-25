#pragma once

#include <slang-rhi.h>

#if SLANG_WINDOWS_FAMILY
#define GLFW_EXPOSE_NATIVE_WIN32
#elif SLANG_LINUX_FAMILY
#define GLFW_EXPOSE_NATIVE_X11
#elif SLANG_APPLE_FAMILY
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <slang-rhi/glfw.h>

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
        virtual void OnResize(uint32_t width, uint32_t height) {}
        virtual void OnClose() {}
    };

    explicit Window(Desc desc);

    Window();

    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void SetCallbacks(ICallbacks *callbacks);

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

    static void HandleFramebufferSize(GLFWwindow *window, int width, int height);
    static void HandleWindowClose(GLFWwindow *window);

    GLFWwindow *_window = nullptr;
    ICallbacks *_callbacks = nullptr;
    Desc _desc;
    int _width = 0;
    int _height = 0;
};

} // namespace Roxy
