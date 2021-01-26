#ifndef SHADER_H
#define SHADER_H

#include "defines.h"
#include "maths.h"
#include <GLES2/gl2.h>
#include <vector>

struct Shader {
	GLuint ID;
	GLuint vertexshaderID;
	GLuint fragshaderID;
};

internal inline
i32 get_uniform_location(Shader shader, const char* name) {
	return glGetUniformLocation(shader.ID, name);
}

internal inline
u32 load_shader_file(const char* path, GLenum type) {
	GLuint shaderID = glCreateShader(type);

	const char* shaderSource = read_file(path);

	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	i32 result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		i32 len;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(shaderID, len, &len, &error[0]);
		glDeleteShader(shaderID);
		LOG(DEBUG, "\nCOULD NOT COMPILE %s SHADER! (ID: %d)\n", path, shaderID);
		LOG(DEBUG, "%s\n", &error[0]);
		return 0;
	}
	else {
		//std::cout << "Compiled " << filename << " shader successfully! " << "(ID: " << shaderID << ")" << std::endl;
		//std::cout << shaderSource << std::endl;
	}

	delete[] shaderSource;
	return shaderID;
}

internal inline
u32 load_shader_string(const char* string, u32 type) {
	i32 shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &string, NULL);
	glCompileShader(shaderID);

	i32 result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		i32 len;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(shaderID, len, &len, &error[0]);
		glDeleteShader(shaderID);
		LOG(MINOR_ERROR, "\nCOULD NOT COMPILE %s SHADER! (ID: %d)\n", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), shaderID);
		LOG(FATAL_ERROR, "%s\n", &error[0]);
		return 0;
	}
	else {
		//std::cout << "Compiled " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader successfully! " << "(ID: " << shaderID << ")" << std::endl;
	}

	return shaderID;
}

internal inline
Shader load_shader(const char* vertexfile, const char* fragmentfile) {
	Shader shader = { 0 };
	shader.vertexshaderID = load_shader_file(vertexfile, GL_VERTEX_SHADER);
	shader.fragshaderID = load_shader_file(fragmentfile, GL_FRAGMENT_SHADER);
	shader.ID = glCreateProgram();

	glAttachShader(shader.ID, shader.vertexshaderID);
	glAttachShader(shader.ID, shader.fragshaderID);
	//glBindFragDataLocation(shader.ID, 0, "outColor");
	glBindAttribLocation(shader.ID, 0, "position");
	glBindAttribLocation(shader.ID, 1, "normal");
	glBindAttribLocation(shader.ID, 2, "uv");
	glLinkProgram(shader.ID);
	glValidateProgram(shader.ID);

	glUseProgram(0);
	return shader;
}

internal inline
Shader load_shader_from_strings(const char* vertexstring, const char* fragmentstring) {
	Shader shader = { 0 };
	shader.vertexshaderID = load_shader_string(vertexstring, GL_VERTEX_SHADER);
	shader.fragshaderID = load_shader_string(fragmentstring, GL_FRAGMENT_SHADER);

	shader.ID = glCreateProgram();
	glAttachShader(shader.ID, shader.vertexshaderID);
	glAttachShader(shader.ID, shader.fragshaderID);
	//glBindFragDataLocation(shader.ID, 0, "outColor");
	glBindAttribLocation(shader.ID, 0, "position");
	glBindAttribLocation(shader.ID, 1, "normal");
	glBindAttribLocation(shader.ID, 2, "uv");
	glLinkProgram(shader.ID);
	glValidateProgram(shader.ID);

	glUseProgram(0);
	return shader;
}

//=============================================
//
//      UNIFORM VARIABLE UPLOADING
//
//=============================================

internal inline
void upload_float(Shader shader, const char* name, f32 value) {
	i32 location = get_uniform_location(shader, name);
	glUniform1f(location, value);
}

internal inline
void upload_float_array(Shader shader, const char* name, f32 arr[], i32 count) {
	i32 location = get_uniform_location(shader, name);
	glUniform1fv(location, count, arr);
}

internal inline
void upload_int(Shader shader, const char* name, i32 value) {
	i32 location = get_uniform_location(shader, name);
	glUniform1i(location, value);
}

internal inline
void upload_int_array(Shader shader, const char* name, i32 arr[], i32 count) {
	i32 location = get_uniform_location(shader, name);
	glUniform1iv(location, count, arr);
}

internal inline
void upload_vec2(Shader shader, const char* name, vec2 vec) {
	i32 location = get_uniform_location(shader, name);
	glUniform2f(location, vec.x, vec.y);
}

internal inline
void upload_vec3(Shader shader, const char* name, vec3 vec) {
	i32 location = get_uniform_location(shader, name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

internal inline
void upload_vec4(Shader shader, const char* name, vec4 vec) {
	i32 location = get_uniform_location(shader, name);
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

internal inline
void upload_bool(Shader shader, const char* name, bool value) {
	i32 location = get_uniform_location(shader, name);
	glUniform1f(location, value ? 1 : 0);
}

internal inline
void upload_mat4(Shader shader, const char* name, mat4 mat) {
	i32 location = get_uniform_location(shader, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.elements);
}

internal inline
void start_shader(Shader shader) {
	glUseProgram(shader.ID);
}

internal inline
void stop_shader() {
	glUseProgram(0);
}

internal inline
void dispose_shader(Shader shader) {
	glDeleteShader(shader.fragshaderID);
	glDeleteShader(shader.vertexshaderID);
	glDeleteProgram(shader.ID);
}

#endif
