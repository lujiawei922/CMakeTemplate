#添加需要编译的源文件目录
AUX_SOURCE_DIRECTORY(. DIR_SRCS1)

#C++编译参数
SET(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
#release与debug编译参数
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#任务单元头文件搜索路径
INCLUDE_DIRECTORIES(include/)

#指定另外的库路径
LINK_DIRECTORIES(lib)

#编译可执行文件
ADD_EXECUTABLE(main  ${DIR_SRCS1} )
# 
TARGET_LINK_LIBRARIES(main 库全名)








