#pragma once
#include <GLFW/glfw3.h>
#include <String>
#include <vector>
#include <cstdint>
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

	//initialization 
	bool Initialization();
	void Shutdown();
	// ------------------------------------------------------------
   // Internal initialization
   // ------------------------------------------------------------

	

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	int getFrameBufferWidth()  const;
	int getFrameBufferHeight()  const;

	bool wasResized() const;
	void ResetResizeFlag();
	bool ShouldClose() const;
	void Show();
	void Hide();


	double getMouseX()const;
	double getMouseY()const;

	bool isKeyPressed(int key)const;
	bool isKeyRelease(int key)const;

	GLFWwindow* getHandler() const;
private:
	bool InitializeGLFW();

	bool CreateWindow();

	void RegisterCallbacks();

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

