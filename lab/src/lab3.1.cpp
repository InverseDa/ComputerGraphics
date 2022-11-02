//
// Created by 缪克达 on 2022/10/26.
//
#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include <vector>
#include <string>

int WIDTH = 600;
int HEIGHT = 600;
int mainWindow;


struct openGLObject
{
    // �����������
    GLuint vao;
    // ���㻺�����
    GLuint vbo;

    // ��ɫ������
    GLuint program;
    // ��ɫ���ļ�
    std::string vshader;
    std::string fshader;
    // ��ɫ������
    GLuint pLocation;
    GLuint cLocation;

    // ͶӰ�任����
    GLuint modelLocation;
    GLuint viewLocation;
    GLuint projectionLocation;
};


openGLObject cube_object;
TriMesh *cube = new TriMesh();
Camera* camera_1 = new Camera();
Camera* camera_2 = new Camera();
Camera* camera_3 = new Camera();
Camera *camera_4 = new Camera();


void bindObjectAndData(TriMesh* mesh, openGLObject& object, const std::string &vshader, const std::string &fshader) {

    // ���������������
    glGenVertexArrays(1, &object.vao);  	// ����1�������������
    glBindVertexArray(object.vao);  	// �󶨶����������

    // ��������ʼ�����㻺�����
    glGenBuffers(1, &object.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh->getPoints().size() * sizeof(glm::vec3) + mesh->getColors().size() * sizeof(glm::vec3),
                 NULL,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getPoints().size() * sizeof(glm::vec3), &mesh->getPoints()[0]);
    glBufferSubData(GL_ARRAY_BUFFER, mesh->getPoints().size() * sizeof(glm::vec3), mesh->getColors().size() * sizeof(glm::vec3), &mesh->getColors()[0]);

    object.vshader = vshader;
    object.fshader = fshader;
    object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

    // �Ӷ�����ɫ���г�ʼ�������λ��
    object.pLocation = glGetAttribLocation(object.program, "vPosition");
    glEnableVertexAttribArray(object.pLocation);
    glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // �Ӷ�����ɫ���г�ʼ���������ɫ
    object.cLocation = glGetAttribLocation(object.program, "vColor");
    glEnableVertexAttribArray(object.cLocation);
    glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mesh->getPoints().size() * sizeof(glm::vec3)));

    // ��þ���λ��
    object.modelLocation = glGetUniformLocation(object.program, "model");
    object.viewLocation = glGetUniformLocation(object.program, "view");
    object.projectionLocation = glGetUniformLocation(object.program, "projection");
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    HEIGHT = height;
    WIDTH = width;
    glViewport(0, 0, width, height);
}


void init()
{
    std::string vshader, fshader;
    // ��ȡ��ɫ����ʹ��
    vshader = "lab/labShaders/lab3/vshader.glsl";
    fshader = "lab/labShaders/lab3/fshader.glsl";

    // ��ȡ������
    cube->readOff("./assets/cube.off");
    bindObjectAndData(cube, cube_object, vshader, fshader);

    // ��ɫ����
    glClearColor(0.0, 0.0, 0.0, 1.0);
}


void display_1()
{
    glViewport(0, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

    camera_1->updateCamera();

    // ����ģ�͵ı任����
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(camera_1->upAngle), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(-camera_1->rotateAngle), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0 / camera_1->scale, 1.0 / camera_1->scale, 1.0 / camera_1->scale));

    glm::mat4 view = glm::mat4(1.0f);
    view[2][2] = -1.0;
    glm::mat4 projection = glm::mat4(1.0f);

    // ����ͶӰ�任����
    glUniformMatrix4fv(cube_object.modelLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(cube_object.viewLocation, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_FALSE, &projection[0][0]);

    glUseProgram(cube_object.program);
    glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}


void display_2()
{
    glViewport(0, 0, WIDTH / 2, HEIGHT / 2);

    camera_2->updateCamera();

    glm::mat4 modelMatrix= glm::mat4(1.0); // ����ģ�͵ı任����

    // ���� Camera::lookAt ����������ͼ�任����
    camera_2->viewMatrix = camera_2->lookAt(camera_2->eye, camera_2->at, camera_2->up);

    // ���� Camera:frustum ��������͸��ͶӰ����
    float top = tan(camera_2->fov * M_PI / 180 / 2) * camera_2->zNear;
    float right = top * camera_2->aspect;
    camera_2->projMatrix = camera_2->frustum(-right, right, -top, top, camera_2->zNear, camera_2->zFar);

    // ����ͶӰ�任����
    glUniformMatrix4fv(cube_object.modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(cube_object.viewLocation, 1, GL_FALSE, &camera_2->viewMatrix[0][0]);
    glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_FALSE, &camera_2->projMatrix[0][0]);

    glUseProgram(cube_object.program);
    glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}


void display_3()
{
    glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

    camera_3->updateCamera();

    glm::mat4 modelMatrix = glm::mat4(1.0); // ����ģ�͵ı任����
    camera_3->viewMatrix = camera_3->lookAt(camera_3->eye, camera_3->at, camera_3->up);	// ���� Camera::lookAt ����������ͼ�任����

    // @TODO: Task2: ���� Camera::ortho ������������ͶӰ����
    camera_3->projMatrix = glm::mat4(1.0f);
    camera_3->projMatrix = camera_3->ortho(-camera_3->scale, camera_3->scale, -camera_3->scale, camera_3->scale, camera_3->zNear, camera_3->zFar);

    // ����ͶӰ�任����
    glUniformMatrix4fv(cube_object.modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(cube_object.viewLocation, 1, GL_FALSE, &camera_3->viewMatrix[0][0]);
    glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_FALSE, &camera_3->projMatrix[0][0]);

    glUseProgram(cube_object.program);
    glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}


void display_4()
{
    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT / 2);

    camera_4->updateCamera();

    glm::mat4 modelMatrix = glm::mat4(1.0); // ����ģ�͵ı任����
    camera_4->viewMatrix = camera_4->lookAt(camera_4->eye, camera_4->at, camera_4->up);	// ���� Camera::lookAt ����������ͼ�任����

    // @TODO: Task3: ���� Camera::perspective ��������͸��ͶӰ����
    camera_4->projMatrix = glm::mat4(1.0f);
    camera_4->projMatrix = camera_4->perspective(camera_4->fov, camera_4->aspect, camera_4->zNear, camera_4->zFar);

    // ����ͶӰ�任����
    glUniformMatrix4fv(cube_object.modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(cube_object.viewLocation, 1, GL_FALSE, &camera_4->viewMatrix[0][0]);
    glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_FALSE, &camera_4->projMatrix[0][0]);

    glUseProgram(cube_object.program);
    glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ����4��display�ֱ���4������ͼ�ڻ���������
    // ÿ��displayʹ�õ�����ĵ�ͶӰ��������㷽������һ��
    display_1();
    display_2();
    display_3();
    display_4();
}


void reshape(GLsizei w, GLsizei h)
{
    WIDTH = w;
    HEIGHT = h;
    glViewport(0, 0, WIDTH, HEIGHT);
}


void printHelp()
{
    std::cout << "Keyboard Usage" << std::endl;
    std::cout <<
              "Q/ESC: 	Exit" << std::endl <<
              "h: 	Print help message" << std::endl <<
              "SPACE: 	Reset all parameters" << std::endl <<
              "x/(shift+x): 	Increase/Decrease the rotate angle" << std::endl <<
              "y/(shift+y): 	Increase/Decrease the up angle" << std::endl <<
              "r/(shift+r): 	Increase/Decrease the distance between camera and object" << std::endl <<
              "f/(shift+f): 	Increase/Decrease FOV of perspective projection" << std::endl <<
              "a/(shift+a): 	Increase/Decrease WIDTH/HEIGHT aspect of perspective projection" << std::endl <<
              "s/(shift+s):	Increase/Decrease the extent of orthogonal projection" << std::endl;
}


void mainWindow_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 'ESC���˳�'
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        printHelp();
    }
    else
    {
        camera_1->keyboard(key,action,mode);
        camera_2->keyboard(key,action,mode);
        camera_3->keyboard(key,action,mode);
        camera_4->keyboard(key,action,mode);
    }
}


void cleanData() {
    cube->cleanData();

    delete camera_1;
    delete camera_2;
    delete camera_3;
    delete camera_4;

    camera_1 = NULL;
    camera_2 = NULL;
    camera_3 = NULL;
    camera_4 = NULL;

    // �ͷ��ڴ�
    delete cube;
    cube = NULL;

    // ɾ���󶨵Ķ���
    glDeleteVertexArrays(1, &cube_object.vao);
    glDeleteBuffers(1, &cube_object.vbo);
    glDeleteProgram(cube_object.program);
}


int main(int argc, char **argv)
{
    glfwInit();

    // �������������������ͼ����Ȼ��ݸк����صĻ������Գ��Ե������Ĳ���ֵ��
    glfwWindowHint(GLFW_SAMPLES,5);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "Camera Localization & Projection", NULL, NULL);
    if (mainwindow == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(mainwindow);
    glfwSetFramebufferSizeCallback(mainwindow, framebuffer_size_callback);
    glfwSetKeyCallback(mainwindow,mainWindow_key_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    init();
    printHelp();
    while (!glfwWindowShouldClose(mainwindow))
    {

        display();
        glfwSwapBuffers(mainwindow);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}

