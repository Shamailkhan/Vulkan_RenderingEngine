#include "GLFWWindow.h"
#include <iostream>
GLFWWindow::GLFWWindow(uint32_t width, uint32_t height, const std::string& title):
	m_width(width),
	m_height(height),
	m_title(title)
{

}

GLFWWindow::~GLFWWindow()
{
	Shutdown();
}

bool GLFWWindow::Initialization()
{
	// this is the main setup function
	if (m_initialized)
	{
		std::cout << "the glfw is already initialized";
		return true;
	}

	std::cout << "INITIALIZING GLFW " << std::endl;
	//step 1 initialize glfe
	if (!InitializeGLFW())
	{
		std::cout << "FAILED TO Initializing GLFW " << std::endl;
		return false;
	}
	// create window 

	if (!CreateWindow())
	{
		std::cout << "Failed to create GLFW window"<<std::endl;
		glfwTerminate();
		return false;


	}
	//Register resize , mouse , keyboard etc callbacks
	RegisterCallbacks();

	
	// Get initial framebuffer size
	glfwGetFramebufferSize(
		glfw_window,
		&frameBufferWidth,
		&frameBufferHeight
	);
	m_initialized = true;

	return true;
}

void GLFWWindow::Shutdown()
{
}

bool GLFWWindow::InitializeGLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << " failed to initialize GLFW" << std::endl;
		return false;
	 }

	//we are creating a window for vulkan it is 
	//Sets the specified window hint to the desired value
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//ALLow the window to be resize
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);



	return true;
}
bool GLFWWindow::CreateWindow()
{
	glfw_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

	if (!glfw_window)
	{
		std::cout << "Failed to craete a window" << std::endl;
		return false;
	}


	return true;

}

void GLFWWindow::RegisterCallbacks()
{

	if (!glfw_window)
		return;

	//store glfw window pointer inside the GLFW
	glfwSetWindowUserPointer(glfw_window, this);


	//frame buffer resizecallback
	glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow* window, int width, int height)
		{
			GLFWWindow* glfwWindow =
				static_cast<GLFWWindow*>(
					glfwGetWindowUserPointer(window)
					);

			if (!glfwWindow)
				return;

			glfwWindow->frameBufferWidth =width;

			glfwWindow->frameBufferHeight =height;

			glfwWindow->isResized = true;

		});


}

GLFWwindow* GLFWWindow::getHandler() const
{
	return glfw_window;
}


uint32_t GLFWWindow::getWidth() const
{
	return m_width;
}
uint32_t GLFWWindow::getHeight() const
{
	return m_height;
}



int GLFWWindow::getFrameBufferWidth()  const
{
	return frameBufferWidth;
}
int GLFWWindow::getFrameBufferHeight()  const
{
	return frameBufferHeight;
}
bool GLFWWindow::ShouldClose() const
{
	if (!glfw_window)
		return true;

	return (glfwWindowShouldClose(glfw_window) == GLFW_TRUE);
}
bool GLFWWindow::wasResized() const
{
	return isResized;

 }


void GLFWWindow::ResetResizeFlag()
{
	isResized = false;
}

void GLFWWindow::Show()
{
	if(!glfw_window)
		return;

	glfwShowWindow(glfw_window);
}

void GLFWWindow::Hide()
{
	if (!glfw_window)
		return;

	glfwHideWindow(glfw_window);
}