#include "matrixhandler.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

MatStack * MatStack::instance = nullptr;


MatStack::MatStack() {
    stack.push(glm::mat4(1.0f));
}

MatStack * MatStack::getInstance()
{
    if (instance == nullptr)
    {
        instance = new MatStack();
    }
    
    return instance;
}

MatStack * MatStack::Translate(float x, float y, float z) {
    stack.top() = glm::translate(stack.top(), glm::vec3(x, y, z));
    return this;
}

MatStack * MatStack::Translate(glm::vec3 vec) {
    stack.top() = glm::translate(stack.top(), vec);
    return this;
}

MatStack * MatStack::Rotate(float angle, float x, float y, float z) {
    stack.top() = glm::rotate(stack.top(), angle, glm::vec3(x, y, z));
    return this;
}

MatStack * MatStack::Rotate(float angle, glm::vec3 vec) {
    stack.top() = glm::rotate(stack.top(), angle, vec);
    return this;
}

MatStack * MatStack::Scale(float x, float y, float z) {
    stack.top() = glm::scale(stack.top(), glm::vec3(x, y, z));
    return this;
}

MatStack * MatStack::Scale(glm::vec3 vec) {
    stack.top() = glm::scale(stack.top(), vec);
    return this;
}

glm::mat4 MatStack::getTop() {
    return stack.top();
}

MatStack * MatStack::Push() {
    stack.push(stack.top());
    return this;
}

MatStack * MatStack::Pop() {
    stack.pop();
    return this;
}

MatStack * MatStack::loadIdentity() {
    stack.top() = glm::mat4(1.0f);
    return this;
}

MatStack::~MatStack() {
    delete instance;
}

