/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderManager
{
public:
	void BuildShaders(char* vs_filename, char* fs_filename);
	void SetFloat(GLchar* name, float value);
	void SetInt(GLchar* name, int value);
	void Use();
	void SetMat4(GLchar* name, const glm::mat4& mat);
	void DeleteShader();

private:
	unsigned int shader_program;
};
