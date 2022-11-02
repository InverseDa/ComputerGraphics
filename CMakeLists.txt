cmake_minimum_required(VERSION 3.23.2)
cmake_policy(VERSION 3.0)
project(ComputerGraphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message("Now System is Mac")
    # 检查环境变量
    if (NOT DEFINED ENV{GLFW_HOME})
        message(FATAL_ERROR "found no env named GLFW_HOME")
    endif()
    if (NOT DEFINED ENV{GLAD_HOME})
        message(FATAL_ERROR "found no env named GLAD_HOME")
    endif()

    # 暂存环境变量
    set(GLFW_HOME $ENV{GLFW_HOME})
    set(GLAD_HOME $ENV{GLAD_HOME})

    # 设置头文件目录
    include_directories("${GLFW_HOME}/include")
    include_directories("${GLAD_HOME}/include")
    include_directories("${PROJECT_SOURCE_DIR}/include ")

    # 添加 GLFW3 预编译库
    add_library(glfw SHARED IMPORTED)
    SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.dylib")

    # 链接 GLFW3 预编译库
    link_libraries(glfw)

    add_executable(main main.cpp src/glad.c include/InitShader.cpp include/Camera.cpp include/TriMesh.cpp)
    add_executable(demo demo.cpp src/glad.c include/InitShader.cpp include/Camera.cpp include/TriMesh.cpp)

    file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "lab/src/*")
    foreach (file_path ${my_c_list})
        string(REPLACE ".cpp" "" new_name ${file_path})
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} src/glad.c include/InitShader.cpp include/Camera.cpp include/Texture/Texture.cpp  include/TriMesh.cpp)
    endforeach ()

    # 利用for循环逐步编译tutorial的文件
    file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "tutorial/*")
    foreach (file_path ${my_c_list})
        string(REPLACE ".cpp" "" new_name ${file_path})
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} src/glad.c include/Camera/Camera.cpp include/Texture/Texture.cpp)
    endforeach ()
endif ()

# 如果编译期的环境是Windows的话，那么就使用如下的cmake
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message("Now System is Windows")
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib)

    find_package(assimp REQUIRED)

    link_libraries(${PROJECT_SOURCE_DIR}/lib/glfw3.dll)

    add_executable(main main.cpp src/glad.c include/InitShader.cpp include/Camera/Camera.cpp)
    add_executable(demo demo.cpp src/glad.c include/InitShader.cpp include/Camera.cpp include/TriMesh.cpp)

    file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "lab/src/*")
    foreach (file_path ${my_c_list})
        string(REPLACE ".cpp" "" new_name ${file_path})
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} src/glad.c include/InitShader.cpp include/Camera/Camera.cpp include/Texture/Texture.cpp  include/TriMesh.cpp)
    endforeach ()

    link_libraries(assimp)

    # 利用for循环逐步编译tutorial的文件
    file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "tutorial/*")
    foreach (file_path ${my_c_list})
        string(REPLACE ".cpp" "" new_name ${file_path})
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} src/glad.c include/Camera/Camera.cpp include/Texture/Texture.cpp)
    endforeach ()
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("Now System is Linux")
    link_directories(/usr/lib)
    add_subdirectory(glfwSource)
    find_package(glfw3 3.3 REQUIRED)
    # 利用for循环逐步编译tutorial的文件
    file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "tutorial/*")
    foreach (file_path ${my_c_list})
        string(REPLACE ".cpp" "" new_name ${file_path})
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} src/glad.c)
        target_link_libraries(${filename} glfw ${CMAKE_DL_LIBS})
    endforeach ()
endif ()