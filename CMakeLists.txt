# 设置CMake最低版本要求  
cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称  
project(learning_test)

# 判断操作系统类型  
if(UNIX)  
    # 在 Unix-like 系统中使用 $HOME 环境变量  
    execute_process(COMMAND sh -c "echo $HOME" OUTPUT_VARIABLE USER_HOME_DIR)  
    string(STRIP ${USER_HOME_DIR} USER_HOME_DIR)  
elseif(WIN32)  
    # 在 Windows 中使用 %USERPROFILE% 环境变量  
    execute_process(COMMAND cmd /C "echo %USERPROFILE%" OUTPUT_VARIABLE USER_HOME_DIR)  
    string(STRIP ${USER_HOME_DIR} USER_HOME_DIR)  
    # 将反斜杠替换为正斜杠（可选，但可以使路径更易于处理）  
    string(REPLACE "\\" "/" USER_HOME_DIR ${USER_HOME_DIR})  
endif() 

set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/program_running/${PROJECT_NAME}")
# 指定编译输出的路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 设置构建选项前提
option(BUILD_TEST "编译测试部分用例"	ON)

# 设置构建中用到的变量
set(DependCodePath "${CMAKE_CURRENT_SOURCE_DIR}/depend/code")
set(ThirdPartyPath "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
# 设置图像资源依赖目录(主要本地复现PainterEngine中demo所需)
set(Resoources_Dir "${CMAKE_CURRENT_SOURCE_DIR}/depend/PainterEngine/documents")

# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD 17)
# 强制要求使用C++17标准   
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# 编译位置无关代码标志添加
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# 添加源代码文件  
file(GLOB_RECURSE DEPNED_SOURCES "${DependCodePath}/*.c")
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")  
  
# 添加头文件目录  
include_directories(${ThirdPartyPath}/include
					${DependCodePath}/PainterEngine
					${PROJECT_SOURCE_DIR}/src
)

# 生成文件
add_library(${PROJECT_NAME}_lib SHARED ${DEPNED_SOURCES})
add_executable(${PROJECT_NAME}  ${DEPNED_SOURCES}
                                ${SOURCES}
)
# 为特定目标添加编译时的宏定义，包括值
# target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME="${PROJECT_NAME} ")

# 添加需要链接的库文件
target_link_libraries(${PROJECT_NAME} PUBLIC ${ThirdPartyPath}/lib/libglut.so.3
									  		 GL
)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${ThirdPartyPath}/lib/libglut.so.3
									  		     GL
)

# 测试项目
if (BUILD_TEST)
	# 设置日志打印级别
	add_definitions(-DGLOBAL_LOG_LEVEL=1)

	message(STATUS "build test modules")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/贪食蛇 ${CMAKE_BINARY_DIR}/test)
endif()

# 添加必要的安装动态库文件
file(GLOB SHARED_LIBS ${ThirdPartyPath}/lib/*.so.*)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${SHARED_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# 设置RPATH, 确保运行时可以找到依赖库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib")