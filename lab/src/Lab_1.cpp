#include "Angel.h"
#include <string>

const glm::vec3 WHITE(1.0, 1.0, 1.0);
const glm::vec3 BLACK(0.0, 0.0, 0.0);
const glm::vec3 RED(1.0, 0.0, 0.0);
const glm::vec3 GREEN(0.0, 1.0, 0.0);
const glm::vec3 BLUE(0.0, 0.0, 1.0);
const int CIRCLE_NUM_POINTS = 100;
const int ELLIPSE_NUM_POINTS = 100;
const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM;

// 根据角度生成颜色
float generateAngleColor(double angle) {
    return 1.0 / (2 * M_PI) * angle;
}

// 获得三角形的每个角度
double getTriangleAngle(int point) {
    return 2 * M_PI / 3 * point;
}

// 获得正方形的每个角度
double getSquareAngle(int point) {
    return M_PI / 4 + (M_PI / 2 * point);
}

// 计算椭圆/圆上的点
glm::vec2 getEllipseVertex(glm::vec2 center, double scale, double verticalScale, double angle) {
    glm::vec2 vertex(sin(angle), cos(angle));
    vertex *= scale;
    vertex.y *= verticalScale;
    vertex += center;
    return vertex;
}

// 获得椭圆/圆的每个顶点
void generateEllipsePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
                           glm::vec2 center, double scale, double verticalScale) {
    double angleIncrement = (2 * M_PI) / numPoints;
    double currentAngle = M_PI / 2;

    for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
        vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
        if (verticalScale == 1.0) colors[i] = glm::vec3(0.0, generateAngleColor(currentAngle), 0.0);
        else colors[i] = RED;

        currentAngle += angleIncrement;
    }
}

// 获得三角形的每个顶点
void generateTrianglePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex) {
    auto index = startVertexIndex;
    glm::vec2 scale(0.25, 0.25);
    glm::vec2 center(0.0, 0.70);

    for (int i = 0; i < 3; ++i) {
        double currentAngle = getTriangleAngle(i);
        vertices[startVertexIndex + i] = glm::vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
    }

    colors[index++] = RED;
    colors[index++] = GREEN;
    colors[index++] = BLUE;
}

// 获得正方形的每个顶点
void generateSquarePoints(glm::vec2 vertices[], glm::vec3 colors[], int squareNumber, int startVertexIndex) {
    glm::vec2 scale(0.90, 0.90);
    double scaleDecrease = 0.15;
    glm::vec2 center(0.0, -0.25);
    int vertexIndex = startVertexIndex;

    //  画六个矩形，奇数白偶数黑
    for (int i = 0; i < squareNumber; ++i) {
        glm::vec3 color;
        color = i % 2 ? BLACK : WHITE;
        //  每个矩形有四个顶点
        for (int j = 0; j < 4; ++j) {
            //  以中心生成四个顶点，然后每个顶点的颜色都是相同的（不然会有渐变色）
            double currentAngle = getSquareAngle(j);
            vertices[vertexIndex] = glm::vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
            colors[vertexIndex] = color;
            vertexIndex++;
        }
        scale -= scaleDecrease;
    }
}


GLuint vao[4], program;

void init() {

    // 定义三角形的点
    glm::vec2 triangle_vertices[TRIANGLE_NUM_POINTS];
    glm::vec3 triangle_colors[TRIANGLE_NUM_POINTS];
    // 定义矩形的点
    glm::vec2 square_vertices[SQUARE_NUM_POINTS];
    glm::vec3 square_colors[SQUARE_NUM_POINTS];

    // 生成圆形和椭圆上的点和颜色
    glm::vec2 circle_vertices[CIRCLE_NUM_POINTS];
    glm::vec3 circle_colors[CIRCLE_NUM_POINTS];

    glm::vec2 ellipse_vertices[ELLIPSE_NUM_POINTS];
    glm::vec3 ellipse_colors[ELLIPSE_NUM_POINTS];

    // 调用生成形状顶点位置的函数
    generateTrianglePoints(triangle_vertices, triangle_colors, 0);
    generateSquarePoints(square_vertices, square_colors, SQUARE_NUM, 0);
    generateEllipsePoints(circle_vertices, circle_colors, 0, CIRCLE_NUM_POINTS,
                          glm::vec2(-0.65, 0.70), 0.15, 1.0);
    generateEllipsePoints(ellipse_vertices, ellipse_colors, 0, ELLIPSE_NUM_POINTS,
                          glm::vec2(0.65, 0.70), 0.25, 0.25);

    // 读取着色器并使用
    std::string vshader, fshader;
    vshader = "lab/labShaders/lab2/vshader.glsl";
    fshader = "lab/labShaders/lab2/fshader.glsl";
    program = InitShader(vshader.c_str(), fshader.c_str());
    glUseProgram(program);

    // 创建顶点缓存对象，vbo[2]是因为我们将要使用两个缓存对象
    GLuint vbo[2];


    /*
    * 初始化三角形的数据
    */

    glGenVertexArrays(1, &vao[0]);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    GLuint location = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
            location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec2),
            BUFFER_OFFSET(0));

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
    GLuint cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    /*
    * 初始化正方形的数据
    */

    glGenVertexArrays(1, &vao[1]);
    glBindVertexArray(vao[1]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
    location = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
            location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec2),
            BUFFER_OFFSET(0));

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_colors), square_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    //  圆
    glGenVertexArrays(1, &vao[2]);
    glBindVertexArray(vao[2]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vertices), circle_vertices, GL_STATIC_DRAW);
    location = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
            location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec2),
            BUFFER_OFFSET(0));

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_colors), circle_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    //  椭圆
    glGenVertexArrays(1, &vao[3]);
    glBindVertexArray(vao[3]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices), ellipse_vertices, GL_STATIC_DRAW);
    location = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
            location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec2),
            BUFFER_OFFSET(0));

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors), ellipse_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_POINTS);

    glBindVertexArray(vao[1]);
    for (int i = 0; i < SQUARE_NUM; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, (i * 4), 4);
    }

    // 绘制圆
    glBindVertexArray(vao[2]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    // 绘制椭圆
    glBindVertexArray(vao[3]);
    for (int i = 0; i < ELLIPSE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * ELLIPSE_NUM_POINTS, ELLIPSE_NUM_POINTS);
    }


    glFlush();
}

int main(int argc, char **argv) {
    // 初始化GLFW库
    glfwInit();

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 配置窗口属性
    GLFWwindow *window = glfwCreateWindow(512, 512, "2020274045_缪克达_实验一", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // 调用任何OpenGL的函数之前初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();

    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


    while (!glfwWindowShouldClose(window)) {
        display();
        // 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
