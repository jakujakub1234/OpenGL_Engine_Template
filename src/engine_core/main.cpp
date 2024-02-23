#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../../include/engine_core/GLCore.h"

int main()
{
	GLCore window = GLCore();

	window.RenderLoop();
}