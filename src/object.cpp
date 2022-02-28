#include "object.hpp"
#include "windowhandle.hpp"
#include "matrixhandler.hpp"
#include <stdio.h>
#include <vector>


using namespace std;

Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int texture, unsigned int disMap, Shader * shaderProgram) {

    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    this->texture = texture;
    this->disMap = disMap;
    this->normalMap = disMap;

    this->shaderProgram = shaderProgram;

    //Generate Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);  
    //BitTan
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);  
    //Tan
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(2 *sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);  
    //Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(3);  
    //Texture
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(4 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(4);  
    

}

Object::~Object() {

    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &disMap);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}


void Object::setVertexAttrib(){
     //Generate Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);  
    //BitTan
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, bitan));
    glEnableVertexAttribArray(1);  
    //Tan
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tangent));
    glEnableVertexAttribArray(2);  
    //Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(3);  
    //Texture
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texCoord));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

}

void Object::Draw() {
    onDraw(this);
}


Object * Object::setPosition(glm::vec3 position) {
    this->position = position;
    return this;
}

Object * Object::setRotation(glm::vec3 rotation, float degrees) {
    this->rotation = rotation;
    this->degrees = degrees;
    return this;
}

Object * Object::setScale(glm::vec3 scale) {
    this->scale = scale;
    return this;
}

//Getters

glm::vec3 Object::getPosition() {
    return position;
}

glm::vec3 Object::getRotation() {
    return rotation;
}

glm::vec3 Object::getScale() {
    return scale;
}

Object * Object::bindBuffers() {
    //Bind VAO to set attributes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    return this;
}

 Object * Object::applyShader(){
    shaderProgram->use();
    shaderProgram->setInt("texture1", 0);
    shaderProgram->setInt("dispMap", 1);
    shaderProgram->setInt("normalMap", 2);

    //Set Tess Level
    shaderProgram->setInt("innerTess", WindowHandler::getInstance()->tessInner);
    shaderProgram->setInt("outerTess", WindowHandler::getInstance()->tessOuter);


    //Apply Camera matrix
    shaderProgram->setMat4("model", MatStack::getInstance()->getTop());
    shaderProgram->setMat4("view", WindowHandler::getInstance()->camera->getViewMatrix());
    shaderProgram->setMat4("projection", WindowHandler::getInstance()->camera->getProjectionMatrix());

    //Set Lighting Variables
    shaderProgram->setVec3("lightPos", AppHandler::getInstance()->light->getPosition());
    shaderProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shaderProgram->setVec3("viewPos", WindowHandler::getInstance()->camera->m_position);

     return this;
 }

 Object * Object::setOnDraw(std::function<void(Object *)> onDraw) {
    this->onDraw = onDraw;
    return this;
 }

