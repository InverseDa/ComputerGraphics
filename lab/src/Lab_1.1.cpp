#include "Angel.h"
#include <string>

const glm::vec3 WHITE(1.0, 1.0, 1.0);
const glm::vec3 BLACK(0.0, 0.0, 0.0);
const glm::vec3 RED(1.0, 0.0, 0.0);
const glm::vec3 GREEN(0.0, 1.0, 0.0);
const glm::vec3 BLUE(0.0, 0.0, 1.0);
const glm::vec3 CLOUD_BLUE(0.34, 0.98, 1.0);
const glm::vec3 BROWN(0.50, 0.25, 0);
const glm::vec3 YELLOW(1.0, 1.0, 0.0);
const int CIRCLE_NUM_POINTS = 100;
const int ELLIPSE_NUM_POINTS = 100;
const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4;
const int CURE_NUM_POINTS = 2;

// 根据角度生成颜色
float generateAngleColor(double angle) {
    return 1.0 / (2 * M_PI) * angle;
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
        if (verticalScale == 1.0) colors[i] = glm::vec3(1.0, generateAngleColor(currentAngle), 0.0);
        else colors[i] = YELLOW;

        currentAngle += angleIncrement;
    }
}

void generateWhiteCircle(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
                         glm::vec2 center, double scale, double verticalScale) {
    double angleIncrement = (2 * M_PI) / numPoints;
    double currentAngle = M_PI / 2;

    for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
        vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
        colors[i] = WHITE;

        currentAngle += angleIncrement;
    }
}

void generateBlackCircle(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
                         glm::vec2 center, double scale, double verticalScale) {
    double angleIncrement = (2 * M_PI) / numPoints;
    double currentAngle = M_PI / 2;

    for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
        vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
        if (verticalScale == 1.0) colors[i] = glm::vec3(1.0, generateAngleColor(currentAngle), 0.0);
        else colors[i] = BLACK;

        currentAngle += angleIncrement;
    }
}

void generateCure(glm::vec2 vertices[], glm::vec3 colors[], glm::vec2 start, glm::vec2 end) {
    vertices[0] = start;
    vertices[1] = end;
    colors[0] = colors[1] = BROWN;
}

GLuint vao[8], program;

void init() {

    // 生成圆形和椭圆上的点和颜色
    glm::vec2 face[CIRCLE_NUM_POINTS];
    glm::vec3 face_colors[CIRCLE_NUM_POINTS];

    glm::vec2 mouse[ELLIPSE_NUM_POINTS];
    glm::vec3 mouse_colors[ELLIPSE_NUM_POINTS];

    glm::vec2 whiteEye1[ELLIPSE_NUM_POINTS];
    glm::vec3 whiteEye1_colors[ELLIPSE_NUM_POINTS];
    glm::vec2 whiteEye2[ELLIPSE_NUM_POINTS];
    glm::vec3 whiteEye2_colors[ELLIPSE_NUM_POINTS];

    glm::vec2 blackEye1[ELLIPSE_NUM_POINTS];
    glm::vec3 blackEye1_colors[ELLIPSE_NUM_POINTS];
    glm::vec2 blackEye2[ELLIPSE_NUM_POINTS];
    glm::vec3 blackEye2_colors[ELLIPSE_NUM_POINTS];

    glm::vec2 line1[CURE_NUM_POINTS];
    glm::vec3 line1_colors[CURE_NUM_POINTS];
    glm::vec2 line2[CURE_NUM_POINTS];
    glm::vec3 line2_colors[CURE_NUM_POINTS];

    // 调用生成形状顶点位置的函数
    generateEllipsePoints(face, face_colors, 0, CIRCLE_NUM_POINTS,
                          glm::vec2(0.0, 0.0), 0.75, 1.0);
    generateWhiteCircle(mouse, mouse_colors, 0, CIRCLE_NUM_POINTS,
                        glm::vec2(-0.30, -0.43), 0.10, 2.50);

    generateWhiteCircle(whiteEye1, whiteEye1_colors, 0, CIRCLE_NUM_POINTS,
                        glm::vec2(-0.52, 0.20), 0.18, 1.10);
    generateWhiteCircle(whiteEye2, whiteEye2_colors, 0, CIRCLE_NUM_POINTS,
                        glm::vec2(-0.08, 0.20), 0.25, 1.10);

    generateBlackCircle(blackEye1, blackEye1_colors, 0, CIRCLE_NUM_POINTS,
                        glm::vec2(-0.44, 0.18), 0.10, 1.10);
    generateBlackCircle(blackEye2, blackEye2_colors, 0, CIRCLE_NUM_POINTS,
                        glm::vec2(0.009, 0.18), 0.16, 1.10);

    generateCure(line1, line1_colors, glm::vec2(-0.55, 0.30), glm::vec2(-0.25, 0.20));

    // 读取着色器并使用
    std::string vshader, fshader;
    vshader = "shaders/vshader.glsl";
    fshader = "shaders/fshader.glsl";
    program = InitShader(vshader.c_str(), fshader.c_str());
    glUseProgram(program);

    // 创建顶点缓存对象，vbo[2]是因为我们将要使用两个缓存对象
    GLuint vbo[2];

    glGenVertexArrays(1, &vao[0]);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(face_colors), face_colors, GL_STATIC_DRAW);
    GLuint cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[1]);
    glBindVertexArray(vao[1]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mouse), mouse, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(mouse_colors), mouse_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[2]);
    glBindVertexArray(vao[2]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteEye1), whiteEye1, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteEye1_colors), whiteEye1_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[3]);
    glBindVertexArray(vao[3]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteEye2), whiteEye2, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(whiteEye2_colors), whiteEye2_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[4]);
    glBindVertexArray(vao[4]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackEye1), blackEye1, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackEye1_colors), blackEye1_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[5]);
    glBindVertexArray(vao[5]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackEye2), blackEye2, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackEye2_colors), blackEye2_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[6]);
    glBindVertexArray(vao[6]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line1), line1, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(line1_colors), line1_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));

    glGenVertexArrays(1, &vao[7]);
    glBindVertexArray(vao[7]);

    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line2), line2, GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(line2_colors), line2_colors, GL_STATIC_DRAW);
    cLocation = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(cLocation);
    glVertexAttribPointer(
            cLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            BUFFER_OFFSET(0));


    glClearColor(1.00, 1.00, 1.00, 1.0);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindVertexArray(vao[0]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glBindVertexArray(vao[1]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glBindVertexArray(vao[2]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glBindVertexArray(vao[3]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glBindVertexArray(vao[4]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glBindVertexArray(vao[5]);
    for (int i = 0; i < CIRCLE_NUM_POINTS; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * CIRCLE_NUM_POINTS, CIRCLE_NUM_POINTS);
    }

    glFlush();
}

int main(int argc, char **argv) {
    // 初始化GLFW库
    glfwInit();

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    system("pause");
    return 0;
}
