#pragma once
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
//core responsibilitiies   // framebuffer size
//glfw initialization     //Window title
// glfw termination		 //Mouse event handler
//create window		....
//destroy window
//window size
class GLFWWindow
{
public :
	GLFWWindow(uint32_t width, uint32_t height , const std::string& title);
	~GLFWWindow();

	GLFWWindow(const GLFWWindow&) = delete;
	GLFWWindow& operator=(const GLFWWindow&) = delete;


	//initialization 
	bool Initialization();
	void Shutdown();
	// ------------------------------------------------------------
   // Internal initialization
   // ------------------------------------------------------------

	inline void SetResizable(bool flag) { isResized = flag; }

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	int getFrameBufferWidth()  const;
	int getFrameBufferHeight()  const;

	bool wasResized() const;
	void ResetResizeFlag();
	bool ShouldClose() const;
	void Show();
	void Hide();

	void PollEvent();
	double getMouseX()const;
	double getMouseY()const;
	glm::dvec2 getMouse()const;

	bool isKeyPressed(int key)const;
	bool isKeyRelease(int key)const;
	bool isMinimized() const;
	GLFWwindow* getHandler() const;
private:
	bool InitializeGLFW();

	bool CreateWindow();

	void RegisterCallbacks();

	static void GLFWErrorCallback(int error, const char* desciption);
	static void GLFWFramebufferResizedCallback(GLFWwindow* window, int width, int height);

	uint32_t m_width;
	uint32_t m_height;
	std::string m_title;
	
	bool m_initialized = false;
	bool isResized = false;

	int frameBufferWidth=0;
	int frameBufferHeight=0;
	
	//pointer to store the GLFW window
	GLFWwindow* glfw_window = nullptr;

};

