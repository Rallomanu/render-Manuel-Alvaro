#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

int activeCamera = 1;
glm::mat4 viewMatrix;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_1) activeCamera = 1;
        if (key == GLFW_KEY_2) activeCamera = 2;
        if (key == GLFW_KEY_3) activeCamera = 3;
    }
}

glm::mat4 getViewMatrix() {
    switch (activeCamera) {
        case 1:return glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        case 2: return glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
        case 3: return glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        default: return glm::mat4(1.0f);
    }
}

void setProjection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (activeCamera == 2) {
        // Proyección ortográfica
        float aspect = (float)width / (float)height;
        float orthoSize = 5.0f;
        glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, 0.1f, 100.0f);
    } else {
        // Proyección en perspectiva
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glLoadMatrixf(&projection[0][0]);
    }
}

void drawCube() {
    glBegin(GL_QUADS);

    // Cara fronta
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);

    // Cara superior
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f,  -1.0f);
    glVertex3f(-1.0f, 1.0f,   1.0f);
    glVertex3f( 1.0f, 1.0f,   1.0f);
    glVertex3f( 1.0f, 1.0f,  -1.0f);

    // Cara inferior
    glNormal3f( 0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    glEnd();
}

void drawSimpleSphere() {
    const float radius = 1.0f;
    const int stacks = 10, slices = 20;

    for (int i = 0; i <= stacks; ++i) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / stacks);
        float z0 = sin(lat0), zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / stacks);
        float z1 = sin(lat1), zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * M_PI * (float)(j - 1) / slices;
            float x = cos(lng), y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);

            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
        }
        glEnd();
    }
}

void drawPyramid() {
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.707f, 0.707f);

    // Cara frontal
    glNormal3f(0.0f, 0.5f, 1.0f);
    glVertex3f(0, 1, 0);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);

    // Cara derecha
    glNormal3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0, 1, 0);
    glVertex3f(1, -1, 1); 
    glVertex3f(1, -1, -1);

    // Cara trasera
    glNormal3f(0.0f, 0.5f, -1.0f);
    glVertex3f(0, 1, 0);
    glVertex3f(1, -1, -1);
    glVertex3f(-1, -1, -1);

    // Cara izquierda
    glNormal3f(-1.0f, 0.5f, 0.0f);
    glVertex3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
    
    glEnd();

    glBegin(GL_QUADS);
    
    // Base
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(-1, -1, -1);
    
    glEnd();
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Escena 3D OpenGL", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glewInit();
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);
    glLoadMatrixf(&projection[0][0]);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMatrix = getViewMatrix();
        setProjection(800, 600);

        glMatrixMode(GL_MODELVIEW);
        viewMatrix = getViewMatrix();
        glLoadMatrixf(&viewMatrix[0][0]);

        // Cubo
        glPushMatrix();
        glTranslatef(-3.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawCube();
        glPopMatrix();
        // Esfera
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawSimpleSphere();
        glPopMatrix();
        // Pirámide
        glPushMatrix();
        glTranslatef(3.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        drawPyramid();
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}