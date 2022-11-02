//
// Created by inver on 2022/9/11.
//

#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

/*
 * VBO用于一次性发送多个顶点数据到GPU，这样可以优化效率
 * VAO用于解释顶点数据，以便GPU更好的利用
 * EBO与VBO类似，只是顶点换成了元素，用于输出组合的图形
 *
 * 利用uniform全局变量动态颜色的矩形
 * */

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "矩形", nullptr, nullptr);
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

    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    //这里的数字就是代表数组vertices的下标（从上往下看，不是传统的数组下标）
    unsigned int indices[] = {
            0, 1, 3,    //第一个三角形
            1, 2, 3     //第二个三角形
    };
    //两个三角形组合成一个矩形

    //initialize vbo
    unsigned int vbo;
    glGenBuffers(1, &vbo);  //spawn a new vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //bind vbo with ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);  //transport vertices data to buffer memory
    //GL_STATIC_DRAW means static data (not dynamic)

    //initialize vao
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //initialize ebo
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set vertices pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //initialize shader
    const char *vshSource = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
    const char *fshSource = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "uniform vec4 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = ourColor;\n"
                            "}\n\0";

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

        //uniform变量的具体应用地方，实现变化的颜色，并激活着色器程序
        auto timeValue = (float) glfwGetTime();     //获取时间
        auto greenValue = (float) (sin(timeValue) / 2.0f) + 0.5f;   //设置绿颜色通道值，随时间变化
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //设置uniform值

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
}