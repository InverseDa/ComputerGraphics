//
// Created by inver on 2022/9/11.
//

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

/*
 * VBO用于一次性发送多个顶点数据到GPU，这样可以优化效率
 * VAO用于解释顶点数据，以便GPU更好的利用
 * */

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "三角形", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Error: Fail to create window! \n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Error: Fail to initialize GLAD! \n";
        return -1;
    }

    //End initialize glfwSource and glad

    float triangleVertices[] = {
            //Location                      //Color
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    //initialize vbo
    unsigned int vbo;
    glGenBuffers(1, &vbo);  //spawn a new vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //bind vbo with ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices,
                 GL_STATIC_DRAW);  //transport triangleVertices data to buffer memory
    //GL_STATIC_DRAW means static data (not dynamic)

    //initialize vao
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //set triangleVertices pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //set color pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //initialize shader
    const char *vshSource = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "layout (location = 1) in vec3 aColor;\n"
                            "out vec3 ourColor;\n"      //out是给fsh使用的
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "   ourColor = aColor;\n"
                            "}\0";
    const char *fshSource = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "in vec3 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(ourColor, 1.0f);\n"
                            "}\0";

    //compile vshader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshSource, nullptr);
    glCompileShader(vertexShader);

    //compile fshader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fshSource, nullptr);
    glCompileShader(fragmentShader);

    //spawn shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
}