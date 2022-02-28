#pragma once
#include <glm/glm.hpp>
#include "shaderhandler.hpp"
#include "apphandler.hpp"
#include <functional>
#include <vector>

using namespace std;

struct vertex_s {
    glm::vec3 position;
    glm::vec3 bitan;
    glm::vec3 tangent;
    glm::vec3 normal;
    glm::vec2 texCoord;
} typedef vertex;

class Object {
private:
    //Var to store relative position data for object
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;    

    //To store function address
    std::function<void(Object *)> onDraw;

    //Function to set vertex attributes and model data

    void setVertexAttrib();

public:
    //Textures
    unsigned int texture;
    unsigned int disMap;
    unsigned int normalMap;
    //No idea why this is here
    float degrees;
    glm::vec3 color;

    //Shader related to rendering object, could be moves elsewhere
    Shader * shaderProgram;

    //Functions to place Object in scene
    Object * setPosition(glm::vec3 position);
    Object * setRotation(glm::vec3 rotation, float degrees);
    Object * setScale(glm::vec3 scale);

    //Functions to get Object data
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    //Setup Vertext Attributes
    Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int texture, unsigned int disMap, Shader * shaderProgram);
    ~Object();
    
    //Each object has its own draw function that can be set with some c++ magic
    void Draw();
    Object * bindBuffers();
    Object * applyShader();
    Object * setOnDraw(std::function<void(Object *)> onDraw);

    
    //New Stuff to get Model loading working.
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textures;

    Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<unsigned int> textures, Shader * shaderProgram)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->shaderProgram = shaderProgram;

        this->position = position;
        this->rotation = rotation;
        this->scale = scale;


        //Need a function to setup vertex attributes
        setVertexAttrib();
    }



};

