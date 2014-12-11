#include "flower.h"
#include "shape.h"
#include "sphere.h"
#include "cylinder.h"
#include "common.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include "gtc/type_ptr.hpp"
#include <iostream>
Flower::Flower(GLuint position, GLuint normal)
{
    init(position, normal);
}

Flower::Flower()
{
    m_isInitialized = false;
}

Flower::~Flower()
{
    if (m_vertexBufferData != NULL) {
        free(m_vertexBufferData);
    }
    for (std::list<Shape *>::iterator iterator = m_shapes.begin(), end = m_shapes.end(); iterator != end; ++iterator) {
        Shape *s = *iterator;
        delete s;
    }
}

void Flower::init(GLuint position, GLuint normal)
{
    m_isInitialized = true;
    int petals = 3;//glm::min(5.0, floor(8 * rand()));
    m_shapeCount = petals + 2;
    m_transforms = new glm::mat4x4[m_shapeCount];
    //glm::mat4x4 petalTransformation = glm::translate(glm::vec3{1, 0 , 0}) * glm::mat4(1.f);// scaleMatrix(glm::vec3{1.5, 0.01, 0.5}) *
    int i = 0;
    /*for (i = 0; i < petals; i++) {
        printf("Added a petal at index %d\n", i);
        m_shapes.push_back(new Sphere(position, normal));
        m_transforms[i] = petalTransformation;// * rotateMatrix(glm::vec3{0, 1, 0}, i * (2.f * M_PI / (float)petals));
        std::cout << glm::to_string(m_transforms[i]) << std::endl;
    }
    //printf("Added the center at index %d\n", i);
    m_shapes.push_back(new Sphere(position, normal));
    m_transforms[i++] = glm::translate(glm::vec3{0, 0, 1}) * glm::mat4(1.f);// * scaleMatrix(glm::vec3{0.5, 0.01, 0.5});
    //std::cout << glm::to_string(m_transforms[i - 1]) << std::endl;*/

    //printf("Added the stem at index %d\n", i);

    m_shapes.push_back(new Sphere(position, normal));
    m_transforms[i++] = glm::translate(glm::vec3(-2.f, 0.f, 0.f)) * glm::mat4(1.f);// * rotateMatrix(glm::vec3{0, 1, 0}, i * (2.f * M_PI / (float)petals));
    m_shapes.push_back(new Cylinder(position, normal));
    printf("setting %d\n", i);
    m_transforms[i++] = glm::scale(glm::vec3(0.1f, 0.5f, 0.1f)) * glm::translate(glm::vec3(2.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);

    std::cout << glm::to_string(m_transforms[i - 1]) << std::endl;
}

void Flower::render(GLuint modelLoc, glm::mat4x4 overallTransform)
{
    if (m_isInitialized) {
        int i = 0;
        for (std::list<Shape *>::const_iterator iterator = m_shapes.begin(), end = m_shapes.end(); iterator != end; ++iterator) {
            Shape *s = *iterator;
            printf("Rendering Shape %d\n", i);
            std::cout << glm::to_string(m_transforms[i]) << std::endl;
            glm::mat4x4 transformed = m_transforms[i++] * overallTransform;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &(transformed)[0][0]);
            s->render();
        }
    } else {
        printf("Can't render flower until it's been initialized!\n");
    }
}
