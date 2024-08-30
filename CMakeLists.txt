CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(Simulator)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    SET(MAKE_DEFINE "FBASE_MAKE")
    SET(OS "OS_LINUX")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    SET(MAKE_DEFINE "WINDOWS")
    SET(OS "OS_WINDOWS")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_STANDARD 14)

#预定义宏
MESSAGE(STATUS "MAKE_DEFINE: ${MAKE_DEFINE}")
add_definitions(-D${MAKE_DEFINE} -D${OS})

# 生成配置文件用于存放预定于宏,需要什么预定宏再config.hpp.in写入，再OPTION设置是否开启
# 其他文件包含config.hpp即可
CONFIGURE_FILE(
		"${PROJECT_SOURCE_DIR}/config/config.hpp.in"
		"${PROJECT_SOURCE_DIR}/config/config.hpp"
		)
# 设置预定义宏值
OPTION(FBASE_MAKE
		ON
		)

#添加子任务模块
ADD_SUBDIRECTORY(TaskModule)

#库路径
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
# 头文件搜索路径，其中${PROJECT_SOURCE_DIR}为Cmake根目录路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/TaskModule/Module1/include)
# 执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# 源文件路径
AUX_SOURCE_DIRECTORY(./src SRC_DIR)
# 编译成执行文件
ADD_EXECUTABLE(simulator ${SRC_DIR})
TARGET_LINK_LIBRARIES(simulator components pthread)


