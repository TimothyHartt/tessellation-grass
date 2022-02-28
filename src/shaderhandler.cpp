#include "shaderhandler.hpp"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char * tessControlPath, const char * tessEvalPath, const char * geoPath)
{
    tessInner = tessOuter = 1;

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int tessControlShader;
    unsigned int tessEvalShader;
    unsigned int geoShader;

    ID = glCreateProgram();

    vertexShader = CreateShader(GL_VERTEX_SHADER, vertexPath);
    fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentPath);


    if(tessControlPath != nullptr && tessEvalPath != nullptr)
    {
        tessControlShader = CreateShader(GL_TESS_CONTROL_SHADER, tessControlPath);
        tessEvalShader = CreateShader(GL_TESS_EVALUATION_SHADER, tessEvalPath);
    }
    if(geoPath != nullptr)
    {
        geoShader = CreateShader(GL_GEOMETRY_SHADER, geoPath);
    }


    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if(tessControlPath != nullptr && tessEvalPath != nullptr)
    {
        glDeleteShader(tessControlShader);
        glDeleteShader(tessEvalShader);
    }
    if(geoPath != nullptr)
    {
        glDeleteShader(geoShader);
    }
    return;
}



/*
 *  Read text file
 */
std::string  Shader::ReadTextFile(const char *file)
{


    std::string dataCode;
    std::ifstream dataFile;
    dataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{

        dataFile.open(file);
        std::stringstream dataStream;
        
        dataStream << dataFile.rdbuf();

        dataFile.close();

        dataCode = dataStream.str();

    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    

    return dataCode;


}


unsigned int Shader::CreateShader(GLenum type, const char* file)
{
    int success;
    char infoLog[512];

    std::string data = ReadTextFile(file);
    const char* dataCode = data.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &dataCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(ID, shader);

    return shader;
}

Shader * Shader::use(){
    glUseProgram(ID);
    return this;
}

Shader * Shader::setBool(const std::string &name, bool value) 
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    return this;
}
Shader * Shader::setInt(const std::string &name, int value) 
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    return this;
}
Shader * Shader::setFloat(const std::string &name, float value) 
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    return this;
}

Shader * Shader::setVec3(const std::string &name, glm::vec3 value) 
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    return this;
}

Shader * Shader::setMat4(const std::string &name, glm::mat4 value) 
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
    return this;
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}