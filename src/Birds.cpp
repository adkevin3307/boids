#include "Birds.h"

#include <random>
#include <functional>

#include "constant.h"

#define GENERATE_RANGE 500
#define VERTICES_STRIDE 6

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
    this->_vertices.resize(3 * size * VERTICES_STRIDE);

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

void Birds::cohesion(size_t index)
{
    float visual_range = 150;

    int count = 0;
    glm::vec3 center(0.0);

    for (size_t i = 0; i < this->_birds.size(); i++) {
        if (glm::distance(this->_birds[index].position, this->_birds[i].position) < visual_range) {
            count += 1;
            center += this->_birds[i].position;
        }
    }

    if (count) {
        center /= (float)count;

        this->_birds[index].velocity += ((center - this->_birds[index].position) * 0.05f);
    }
}

void Birds::separation(size_t index)
{
    float min_distance = 75;

    glm::vec3 move(0.0);

    for (size_t i = 0; i < this->_birds.size(); i++) {
        if (i == index) continue;

        if (glm::distance(this->_birds[index].position, this->_birds[i].position) < min_distance) {
            move += (this->_birds[index].position - this->_birds[i].position);
        }
    }

    this->_birds[index].velocity += (move * 0.05f);
}

void Birds::alignment(size_t index)
{
    float visual_range = 150;

    int count = 0;
    glm::vec3 average_velocity(0.0);

    for (size_t i = 0; i < this->_birds.size(); i++) {
        if (glm::distance(this->_birds[index].position, this->_birds[i].position) < visual_range) {
            count += 1;
            average_velocity += this->_birds[i].velocity;
        }
    }

    if (count) {
        average_velocity /= (float)count;

        this->_birds[index].velocity += ((average_velocity - this->_birds[index].velocity) * 0.05f);
    }
}

void Birds::speed(size_t index)
{
    float max_speed = 10.0;

    if (glm::length(this->_birds[index].velocity) > max_speed) {
        this->_birds[index].velocity = glm::normalize(this->_birds[index].velocity) * max_speed;
    }
}

void Birds::bound(size_t index)
{
    glm::vec3 _margin(GENERATE_RANGE + log2(this->_birds.size()) * 30);

    if (this->_birds[index].position.x < _margin.x) {
        this->_birds[index].velocity.x += 1.0;
    }
    if (this->_birds[index].position.x > _margin.x * -1.0) {
        this->_birds[index].velocity.x -= 1.0;
    }

    if (this->_birds[index].position.y < _margin.y) {
        this->_birds[index].velocity.y += 1.0;
    }
    if (this->_birds[index].position.y > _margin.y * -1.0) {
        this->_birds[index].velocity.y -= 1.0;
    }

    if (this->_birds[index].position.z < _margin.z) {
        this->_birds[index].velocity.z += 1.0;
    }
    if (this->_birds[index].position.z > _margin.z * -1.0) {
        this->_birds[index].velocity.z -= 1.0;
    }
}

void Birds::convert()
{
    int index = 0;
    for (auto bird : this->_birds) {
        glm::vec3 normal = glm::cross(bird.velocity, glm::vec3(0.0, 1.0, 0.0));

        glm::vec3 front = glm::normalize(bird.velocity) * 25.0f;
        glm::vec3 side = glm::normalize(glm::cross(bird.velocity, normal)) * 10.0f;

        this->_vertices[index++] = bird.position.x + front.x;
        this->_vertices[index++] = bird.position.y + front.y;
        this->_vertices[index++] = bird.position.z + front.z;

        this->_vertices[index++] = normal.x;
        this->_vertices[index++] = normal.y;
        this->_vertices[index++] = normal.z;
        
        this->_vertices[index++] = bird.position.x + side.x;
        this->_vertices[index++] = bird.position.y + side.y;
        this->_vertices[index++] = bird.position.z + side.z;

        this->_vertices[index++] = normal.x;
        this->_vertices[index++] = normal.y;
        this->_vertices[index++] = normal.z;

        this->_vertices[index++] = bird.position.x - side.x;
        this->_vertices[index++] = bird.position.y - side.y;
        this->_vertices[index++] = bird.position.z - side.z;

        this->_vertices[index++] = normal.x;
        this->_vertices[index++] = normal.y;
        this->_vertices[index++] = normal.z;
    }
}

void Birds::update()
{
    for (size_t index = 0; index < this->_birds.size(); index++) {
        this->cohesion(index);
        this->separation(index);
        this->alignment(index);

        this->speed(index);
        this->bound(index);

        this->_birds[index].position += this->_birds[index].velocity;
    }

    this->convert();
}

int Birds::stride()
{
    return VERTICES_STRIDE;
}

vector<GLfloat>& Birds::vertices()
{
    return this->_vertices;
}
