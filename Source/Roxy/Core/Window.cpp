#include "Window.h"

#include <utility>

#include <slang-rhi/glfw.h>

namespace Roxy
{
Window::Window(Desc desc) : _desc(std::move(desc))
{
    Init();
}

Window::Window()
{
    Init();
}

Window::~Window()
{
    Shutdown();
}

void Window::OnUpdate()
{
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}

void Window::Close()
{
}

void Window::SetTitle(const std::string &title)
{
    _desc.Title = title;
    glfwSetWindowTitle(_window, title.c_str());
}

int Window::GetWidth() const
{
    return _desc.Width;
}

int Window::GetHeight() const
{
    return _desc.Height;
}

rhi::WindowHandle Window::CreateWindowHandle() const
{
    return rhi::getWindowHandleFromGLFW(_window);
}

GLFWwindow *Window::GetNativeWindow() const
{
    return _window;
}

void Window::Init()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, _desc.Resizable);

    _window = glfwCreateWindow(_desc.Width, _desc.Height, _desc.Title.c_str(), nullptr, nullptr);

    if (!_window)
    {
        glfwTerminate();

        throw std::runtime_error("Failed creating GLFW window");
    }

    glfwSetWindowUserPointer(_window, this);
}

void Window::Shutdown()
{
    if (_window)
    {
        glfwDestroyWindow(_window);
    }

    glfwTerminate();
}

} // namespace Roxy