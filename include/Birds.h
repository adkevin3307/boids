#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

class Birds {
private:
    class Bird {
    private:
        glm::vec3 position, velocity;

    public:
        Bird();
        Bird(glm::vec3 position, glm::vec3 velocity);
        ~Bird();

        friend class Birds;
    };

    vector<Bird> _birds;
    vector<GLfloat> _vertices;

    void convert();

public:
    Birds(size_t size);
    ~Birds();

    void update();
    vector<GLfloat>& vertices();
};
