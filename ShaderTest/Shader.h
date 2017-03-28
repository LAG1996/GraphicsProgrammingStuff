#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;
	//Constructor generates the shader on the fly
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
	{
		//----------------------------------------------------------
		//	1. Retrieve the vertex/fragment source code from filePath
		//----------------------------------------------------------
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		if (!vShaderFile)
		{
			std::cout << "Failed to read file " << vertexPath << std::endl;
		}

		if (!fShaderFile)
		{
			std::cout << "Failed to read file " << fragmentPath << std::endl;
		}

		std::stringstream vShaderStream, fShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close file handlers
		vShaderFile.close();
		vShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		
		//-------------------------------
		//	2. Compile shaders
		//-------------------------------
		GLuint vertexShader, fragmentShader;
		GLint success;
		GLchar infoLog[512];

		//Vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		//Print compile errors if any
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SHADER::VERTEX::COMPILATION_SUCCESS\n";
		}

		//Fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		//Print compile errors if any
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SHADER::FRAGMENT::COMPILATION_SUCCESS\n";
		}

		//Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertexShader);
		glAttachShader(this->Program, fragmentShader);
		glLinkProgram(this->Program);

		//Print errors if any
		glGetShaderiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SHADER::PROGRAM::LINKING_SUCCESS\n" << std::endl;
		}

		//Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}
};

#endif