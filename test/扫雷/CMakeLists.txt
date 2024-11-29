# 设置CMake最低版本要求  
cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称  
project(saolei)

# 是否将图像资源打包到程序内部
option(PakageImg "打包图像到程序内，否则拷贝" OFF)

# 指定编译输出的路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 添加源代码文件  
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
# 生成动态库文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 为特定目标添加编译时的宏定义，包括值
# target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME="${PROJECT_NAME} ")

# 添加需要链接的库文件  
target_link_libraries(${PROJECT_NAME} 
    learning_test_lib
)

# 设置RPATH, 确保运行时可以找到依赖库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib")