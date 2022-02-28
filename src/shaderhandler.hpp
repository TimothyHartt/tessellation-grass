#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
    unsigned int ID;
    unsigned int tessInner, tessOuter;


    Shader(const char* vertexPath, const char* fragmentPath, const char * tessControlPath, const char * tessEvalPath, const char * geoPath);
    ~Shader();
    Shader *  use();
    Shader *  setBool(const std::string &name, bool value);
    Shader *  setInt(const std::string &name, int value);
    Shader *  setFloat(const std::string &name, float value);
    Shader *  setVec3(const std::string &name, glm::vec3 value);
    Shader *  setMat4(const std::string &name, glm::mat4 value);
    
    std::string ReadTextFile(const char* file);

    unsigned int CreateShader(GLenum type, const char* file);
     

};