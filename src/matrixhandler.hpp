#pragma once
#include <stack>
#include <glm/glm.hpp>

class MatStack {
    private:
        static MatStack* instance;
    public:
        MatStack();
        ~MatStack();
        static MatStack* getInstance();
        std::stack <glm::mat4> stack;

        MatStack * Translate(float x, float y, float z);
        MatStack * Rotate(float angle, float x, float y, float z);
        MatStack * Scale(float x, float y, float z);

        MatStack * Translate(glm::vec3 vec);
        MatStack * Rotate(float angle, glm::vec3 vec);
        MatStack * Scale(glm::vec3 vec);

        glm::mat4 getTop();

        MatStack * Push();
        MatStack * Pop();
        MatStack * loadIdentity();
};

