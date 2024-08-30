# 添加需要编译的源文件目录
AUX_SOURCE_DIRECTORY(./src DIR_SRCS1)

# 其余需要编译的源文件,将其他不在上面包含的源文件目录下的源文件包含进来
# SET(SRC_LIST ../../public/example.cpp)

# 头文件搜索路径，其中${PROJECT_SOURCE_DIR}为Cmake根目录路径
INCLUDE_DIRECTORIES(./include)

# C++编译参数
SET(CMAKE_CXX_FLAGS "-Wunused-variable ${CMAKE_CXX_FLAGS}")
#release与debug编译参数
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 指定生成库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 编译成库文件
ADD_LIBRARY(components SHARED ${DIR_SRCS1})

# 将生成的库链接其他库
# SET(optionalLibs t2sdk m c sm34_algo t2manage pthread)
# TARGET_LINK_LIBRARIES(编译生成的库名 ${optionalLibs})

# 需要静态库解除注释
# ADD_LIBRARY(库名(不需要lib和.so) STATIC ${DIR_SRCS1} )
# SET_TARGET_PROPERTIES(库名(不需要lib和.so) PROPERTIES OUTPUT_NAME "库名(不需要lib和.so)")








