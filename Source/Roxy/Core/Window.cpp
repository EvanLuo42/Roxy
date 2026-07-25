#include "Window.h"

#include <stdexcept>
#include <utility>

namespace Roxy
{
Window::Window(Desc desc) : _desc(std::move(desc)) { Init(); }

Window::Window() { Init(); }

Window::~Window() { Shutdown(); }

void Window::SetCallbacks(ICallbacks *callbacks) { _callbacks = callbacks; }

void Window::OnUpdate() { glfwPollEvents(); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(_window); }

void Window::Close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }

void Window::SetTitle(const std::string &title)
{
    _desc.Title = title;
    glfwSetWindowTitle(_window, title.c_str());
}

int Window::GetWidth() const { return _width; }

int Window::GetHeight() const { return _height; }

rhi::WindowHandle Window::CreateWindowHandle() const { return rhi::getWindowHandleFromGLFW(_window); }

GLFWwindow *Window::GetNativeWindow() const { return _window; }

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

    glfwGetFramebufferSize(_window, &_width, &_height);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, &Window::HandleFramebufferSize);
    glfwSetWindowCloseCallback(_window, &Window::HandleWindowClose);
}

void Window::Shutdown()
{
    if (_window)
    {
        glfwDestroyWindow(_window);
    }

    glfwTerminate();
}

void Window::HandleFramebufferSize(GLFWwindow *window, int width, int height)
{
    auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

    self->_width = width;
    self->_height = height;

    if (self->_callbacks)
    {
        self->_callbacks->OnResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }
}

void Window::HandleWindowClose(GLFWwindow *window)
{

    if (auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window)); self->_callbacks)
    {
        self->_callbacks->OnClose();
    }
}

} // namespace Roxy
