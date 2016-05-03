#include "Shader.hpp"
#include "Math.hpp"
#include "Vector2.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "DirectionalLight.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

Shader::Shader(std::string const & fragShader, std::string const & vertShader)
{
	init(fragShader, vertShader);
}

Shader::Shader(Shader const & shader)
{
	*this = shader;
}

Shader::~Shader(void)
{
	glDetachShader(m_program, m_shaders[0]);
	glDetachShader(m_program, m_shaders[1]);
	glDeleteShader(m_shaders[0]);
	glDeleteShader(m_shaders[1]);
	glDeleteProgram(m_program);
}

Shader & Shader::operator=(Shader const & shader)
{
	m_program = shader.m_program;
	m_shaders[0] = shader.m_shaders[0];
	m_shaders[1] = shader.m_shaders[1];
	for (std::size_t i = 0u; i < AttributeCount; i++)
		m_attributes[i] = shader.m_attributes[i];
	m_params = shader.m_params;
	return (*this);
}

void Shader::bind(void)
{
	glUseProgram(m_program);
}

void Shader::unbind(void)
{
	glUseProgram(0);
}

void Shader::setParameter(std::string const & name, std::size_t value)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform1ui(location, value);
	}
}

void Shader::setParameter(std::string const & name, int value)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform1i(location, value);
	}
}

void Shader::setParameter(std::string const & name, float value)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform1f(location, value);
	}
}

void Shader::setParameter(std::string const & name, Vector2 const & vector)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform2f(location, vector.x, vector.y);
	}
}

void Shader::setParameter(std::string const & name, Vector3 const & vector)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform3f(location, vector.x, vector.y, vector.z);
	}
}

void Shader::setParameter(std::string const & name, Color const & color)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniform4f(location, color.r, color.g, color.b, color.a);
	}
}

void Shader::setParameter(std::string const & name, Matrix const & matrix)
{
	if (m_program)
	{
		glUseProgram(m_program);
		int location = getParamIndex(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getMatrix());
	}
}

void Shader::setParameter(std::string const & name, std::size_t index, DirectionalLight & light)
{
	if (m_program)
	{
		glUseProgram(m_program);
		std::ostringstream s;
		s << name << "[" << index << "].";
		setParameter(s.str() + "color", light.getColor());
		setParameter(s.str() + "direction", light.getRotatedDirection().normalize());
		setParameter(s.str() + "ambient_intensity", light.getAmbientIntensity());
		setParameter(s.str() + "diffuse_intensity", light.getDiffuseIntensity());
	}
}

void Shader::setParameter(std::string const & name, std::vector<DirectionalLight> & lights)
{
	if (m_program)
	{
		glUseProgram(m_program);
		assert(lights.size() <= DirectionalLight::MaxLight);
		for (std::size_t i = 0u; i < lights.size(); i++)
			setParameter(name, i, lights[i]);
	}
}

int Shader::getAttribute(Attribute attribute) const
{
	return (m_attributes[attribute]);
}

int Shader::getParamIndex(std::string const & name)
{
	ParamMap::iterator it = m_params.find(name);
	if (it != m_params.end())
		return (it->second);
	int location = glGetUniformLocation(m_program, name.c_str());
	m_params.insert(std::make_pair(name, location));
	if (location == -1)
		std::cerr << "Parameter \"" << name << "\" not found in shader." << std::endl;
	return (location);
}

void Shader::init(std::string const & fragShader, std::string const & vertShader)
{
	m_program = glCreateProgram();
	m_shaders[0] = loadShader(fragShader, GL_FRAGMENT_SHADER);
	m_shaders[1] = loadShader(vertShader, GL_VERTEX_SHADER);
	glAttachShader(m_program, m_shaders[0]);
	glAttachShader(m_program, m_shaders[1]);
	glLinkProgram(m_program);

	m_attributes[Attribute::PositionAtt] = glGetAttribLocation(m_program, "in_Position");
	m_attributes[Attribute::TexCoordAtt] = glGetAttribLocation(m_program, "in_TexCoord");
	m_attributes[Attribute::NormalAtt] = glGetAttribLocation(m_program, "in_Normal");
	m_attributes[Attribute::ColorAtt] = glGetAttribLocation(m_program, "in_Color");
	// TODO add accessor to get good uniform location
}

std::string Shader::readShader(std::string const & fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());
	std::string str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
	file.close();
	return (str);
}

GLuint Shader::loadShader(std::string const & filename, GLenum shaderType)
{
	GLuint shaderId;
	char const * glslSource;

	shaderId = 0;
	std::string str = readShader(filename);
	glslSource = str.c_str();
	shaderId = glCreateShader(shaderType);
	if (glslSource && shaderId != 0)
	{
		glShaderSource(shaderId, 1, (const GLchar **)&glslSource, NULL);
		glCompileShader(shaderId);

		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			std::cout << "ERROR: Could not compile the shader" << std::endl;
			std::cout << glslSource << std::endl;

			//TODO
			//GLint maxLength = 0;
			//glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			////The maxLength includes the NULL character
			//	std::vector<GLchar> infoLog(maxLength);
			//glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

			////We don't need the shader anymore.
			//glDeleteShader(shaderId);
			//for (auto & i : infoLog)
			//	std::cout << i;

			//Use the infoLog as you see fit.

			//In this simple program, we'll just leave
		}
	}
	else
		std::cerr << "ERROR: Could not create a shader." << std::endl;
	return (shaderId);
}
