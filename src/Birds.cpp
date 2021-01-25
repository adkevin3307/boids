#include "Birds.h"

#include <random>
#include <functional>

#include "constant.h"

#define GENERATE_RANGE 100

using namespace std;

Birds::Bird::Bird()
{
    this->position = glm::vec3(0.0);
    this->velocity = glm::vec3(0.0);
}

Birds::Bird::Bird(glm::vec3 position, glm::vec3 velocity)
{
    this->position = position;
    this->velocity = velocity;
}

Birds::Bird::~Bird()
{
}

Birds::Birds(size_t size)
{
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<double> distribution(-1, 1);
    auto random = bind(distribution, generator);

    this->_birds.resize(size);
    this->_vertices.resize(size * 3);

    for (size_t i = 0; i < this->_birds.size(); i++) {
        glm::vec3 position(random() * GENERATE_RANGE, random() * GENERATE_RANGE, random() * GENERATE_RANGE);
        glm::vec3 velocity(random(), random(), random());

        this->_birds[i] = Bird(position, velocity);
    }
}

Birds::~Birds()
{
    this->_birds.clear();
    this->_birds.shrink_to_fit();

    this->_vertices.clear();
    this->_vertices.shrink_to_fit();
}

void Birds::convert()
{
    // TODO tetrahedron
    int index = 0;
    for (auto bird : this->_birds) {
        this->_vertices[index++] = bird.position.x;
        this->_vertices[index++] = bird.position.y;
        this->_vertices[index++] = bird.position.z;
    }
}

void Birds::update()
{
    // TODO separation, alignment, cohesion
    for (size_t i = 0; i < this->_birds.size(); i++) {
        this->_birds[i].position += glm::vec3(1.0);
    }

    this->convert();
}

vector<GLfloat>& Birds::vertices()
{
    return this->_vertices;
}
