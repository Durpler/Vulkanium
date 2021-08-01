#include "FirstApp.h"

void FirstApp::run()
{
	while (!vkaniumWindow.ShouldClose())
	{
		glfwPollEvents();
	}
}