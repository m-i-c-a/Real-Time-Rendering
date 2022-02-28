#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "Defines.hpp"

std::string read_file(const std::string filepath)
{
   std::ifstream ifs { filepath, std::ios::in };

   if (ifs.is_open())
   {
      std::ostringstream ss;
      ss << ifs.rdbuf();

      ifs.close();

      return ss.str();
   }
   else
      EXIT("Failed to open file " + filepath);
}

void check_compilation_status(const GLuint shader_handle, const std::string& shader_path)
{
   GLint success;
   char info_log[512];
   glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(shader_handle, 512, NULL, info_log);
      std::cout << info_log << '\n';
      EXIT("ERROR: " + shader_path + " compilation failed!\n " + std::string(info_log));
   }
}

static void check_link_status(const GLuint shader_program, const std::string& program_name)
{
   GLint success;
   char info_log[512];
   glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << info_log << '\n';
      EXIT("ERROR: " + program_name + " linking failed!\n " + std::string(info_log));
   }
}

GLuint compile_shader(const std::string shader_path, const uint64_t gl_shader_type)
{
   const std::string shader_source = read_file(shader_path);
   const char* shader_source_cstr = shader_source.c_str();

   const GLuint shader_handle = glCreateShader(gl_shader_type);
   glShaderSource(shader_handle, 1, &shader_source_cstr, NULL);
   glCompileShader(shader_handle);
   check_compilation_status(shader_handle, shader_path);
   return shader_handle;
}

GLuint create_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string program_name)
{
   const GLuint vertex_shader_handle   = compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
   const GLuint fragment_shader_handle = compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

   GLuint shader_program_handle = glCreateProgram();
   glAttachShader(shader_program_handle, vertex_shader_handle);
   glAttachShader(shader_program_handle, fragment_shader_handle);
   glLinkProgram(shader_program_handle);
   check_link_status(shader_program_handle, program_name);

   glDeleteShader(vertex_shader_handle);
   glDeleteShader(fragment_shader_handle);

   return shader_program_handle;
}

#endif // PROGRAM_HPP