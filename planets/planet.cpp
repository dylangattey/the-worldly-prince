#include "planet.h"
#include "resourceloader.h"
#include "glwidget.h"

#define VERTS_HIGH 48
#define VERTS_LOW 36
#define MOON 0
#define EARTHMARS 1

Planet::Planet() {
    refresh();
}

Planet::~Planet() {
    for (int i=0; i<m_planets.size(); i++) {
        delete m_planets.at(i);
    }
}

void Planet::createShaderProgram() {
    m_shader = ResourceLoader::loadShaders(":/shaders/noise.vert", ":/shaders/noise.frag");

    GLuint pos = glGetAttribLocation(m_shader, "position");
    GLuint norm = glGetAttribLocation(m_shader, "normal");

    // Order is moon, earth, mars
    m_planets += Sphere::generate(VERTS_LOW,pos,norm);
    m_planets += Sphere::generate(VERTS_HIGH,pos,norm);
}

void Planet::createFBO(glm::vec2 size) {
    GLWidget::createFBO(m_FBO, m_colorAttachment, getTextureID(), size, true);
}

void Planet::render(Transforms trans, glm::mat4x4 localizedOrbit, float rSpeed) {
    glUseProgram(m_shader);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the planet with depth and no blending
    drawPlanets(trans, localizedOrbit, rSpeed);

    // Clear
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}

void Planet::refresh() {
    randomizeSeed();
}

// GETTERS

int Planet::getTextureID() {
    return 1;
}

GLuint *Planet::getColorAttach() {
    return &m_colorAttachment;
}

GLuint *Planet::getFBO() {
    return &m_FBO;
}

// START OF PRIVATE METHODS

void Planet::drawPlanets(Transforms origTrans, glm::mat4x4 localizedOrbit, float rSpeed) {
    // Pass shared info to shader first
    glUniform1f(glGetUniformLocation(m_shader, "seed"), m_seed);

    Transforms trans = origTrans;
    GLuint mvp = glGetUniformLocation(m_shader, "mvp");
    GLuint colorLow = glGetUniformLocation(m_shader, "colorLow");
    GLuint colorHigh = glGetUniformLocation(m_shader, "colorHigh");
    GLuint threshold = glGetUniformLocation(m_shader, "threshold");

    // Colors
    glm::vec4 gray = glm::vec4(0.48, 0.48, 0.5, 0.4);
    glm::vec4 blue = glm::vec4(0.1, 0.1, 0.7, 0.6);
    glm::vec4 green = glm::vec4(0.08, 0.55, 0.25, 0.15);
    glm::vec4 red = glm::vec4(0.6, 0.25, 0.15, 0.3);
    glm::vec4 maroon = glm::vec4(0.4, 0.2, 0.2, 0.3);

    // Moon - local orbit only, and all gray
    glUniform4fv(colorHigh, 1, &gray[0]);
    glUniform1f(threshold, -999.0f); // Only show colorHigh
    trans.model = localizedOrbit *
                  origTrans.model;
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &trans.getTransform()[0][0]);
    m_planets.at(MOON)->render();


    // Earth - scale, local orbit, orbit around point, blue and green
    glUniform4fv(colorLow, 1, &blue[0]);
    glUniform4fv(colorHigh, 1, &green[0]);
    glUniform1f(threshold, 1.72f);
    trans.model = glm::rotate(rSpeed/4.0f, glm::vec3(0,1,0)) *
                  glm::translate(glm::vec3(7, 0, 17)) *
                  glm::rotate(-rSpeed/2.0f, glm::vec3(1,2,4)) *
                  glm::scale(glm::vec3(3.3f)) *
                  origTrans.model;
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &trans.getTransform()[0][0]);
    m_planets.at(EARTHMARS)->render();

    // Mars - scale, local orbit, orbit around point, red and maroon
    glUniform4fv(colorLow, 1, &red[0]);
    glUniform4fv(colorHigh, 1, &maroon[0]);
    glUniform1f(threshold, 1.32f);
    trans.model = glm::rotate(rSpeed/3.6f, glm::vec3(0,1,0)) *
                  glm::translate(glm::vec3(-40, 0, 20)) *
                  glm::rotate(-rSpeed/2.0f, glm::vec3(0,3,1)) *
                  glm::scale(glm::vec3(3.6f)) *
                  origTrans.model;
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &trans.getTransform()[0][0]);
    m_planets.at(EARTHMARS)->render();
}

void Planet::randomizeSeed() {
    m_seed = ((float)rand()) / ((float)RAND_MAX);
}

