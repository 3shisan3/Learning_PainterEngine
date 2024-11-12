# 设置CMake最低版本要求  
cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称  
project(dadishu)

# 是否将图像资源打包到程序内部
option(PakageImg "打包图像到程序内，否则拷贝" OFF)

# 指定编译输出的路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 添加源代码文件  
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
# 生成动态库文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 创建一个目录来存放生成的C源文件
set(RESOURCE_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")
# 项目使用的资源文件
file(GLOB RES_FILES "${Resoources_Dir}/demo/game/assets/*")

if(PakageImg)
    # 遍历资源目录中的项目所需的图像文件，并生成对应的C源文件
    foreach(img_file ${IMAGE_FILES})
        string(REGEX REPLACE "^.*/([^/]*)$" "\\1" img_name ${img_file})
        string(TOUPPER ${img_name} img_name_upper)
        string(REPLACE "." "_" img_name_var ${img_name_upper})
    
        # 生成.h文件
        configure_file(
            "${CMAKE_SOURCE_DIR}/resource_template.h.in"
            "${RESOURCE_OUTPUT_DIR}/${img_name_var}_header.h"
            @ONLY
        )
    
        # 使用xxd生成.c文件 xxd工具（需要安装vim-common或类似包）
        add_custom_command(
            OUTPUT "${RESOURCE_OUTPUT_DIR}/${img_name_var}.c"
            COMMAND xxd -i "${img_file}" "${RESOURCE_OUTPUT_DIR}/${img_name_var}.c"
            DEPENDS "${img_file}"
        )
    
        # 将生成的.c和.h文件添加到项目中
        list(APPEND RESOURCE_FILES "${RESOURCE_OUTPUT_DIR}/${img_name_var}.c")
        list(APPEND RESOURCE_HEADERS "${RESOURCE_OUTPUT_DIR}/${img_name_var}_header.h")
    endforeach()

    # 将生成的资源文件添加到可执行文件
    source_group("Resources" FILES ${RESOURCE_FILES} ${RESOURCE_HEADERS})
    target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILES} ${RESOURCE_HEADERS})
else()
    # 创建一个自定义目标来复制资源文件
    add_custom_target(CopyResources ALL
        COMMAND mkdir -p ${RESOURCE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RES_FILES} ${RESOURCE_OUTPUT_DIR}
        COMMENT "Copying resources to ${RESOURCE_OUTPUT_DIR}"
    )
endif()

# 为特定目标添加编译时的宏定义，包括值
# target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME="${PROJECT_NAME} ")

# 添加需要链接的库文件  
target_link_libraries(${PROJECT_NAME} 
    learning_test_lib
)

# 设置RPATH, 确保运行时可以找到依赖库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib")