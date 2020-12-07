# 添加需要编译的源文件目录
AUX_SOURCE_DIRECTORY(. DIR_SRCS1)
AUX_SOURCE_DIRECTORY(../ DIR_SRCS4)

# 其余需要编译的源文件,将其他不在上面包含的源文件目录下的源文件包含进来
SET(SRC_LIST ../../public/hspack.c ../../public/spub.c ../../public/pack_public.cpp
			../../public/spackage.c ../../public/t2sdk.cpp ../../public/hqlog.cpp
			../../public/hsfbase_comm.cpp ../ParentThread/thread_tran.cpp
)

# 头文件搜索路径，其中${PROJECT_SOURCE_DIR}为Cmake根目录路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/Include  ${PROJECT_SOURCE_DIR}/public/t2rpt 
                    ${PROJECT_SOURCE_DIR}/public/t2manage ${PROJECT_SOURCE_DIR}/public/ ${PROJECT_SOURCE_DIR}/TaskModule
					${PROJECT_SOURCE_DIR}/TaskModule/ParentThread
)


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

# C++编译参数
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wunused-variable ${CMAKE_CXX_FLAGS}")
#release与debug编译参数
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 指定生成库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 编译成库文件
ADD_LIBRARY(库名(不需要lib和.so) SHARED ${DIR_SRCS1} ${DIR_SRCS4} ${SRC_LIST}
)

# 将生成的库链接其他库
SET(optionalLibs t2sdk m c sm34_algo t2manage pthread)
TARGET_LINK_LIBRARIES(编译生成的库名 ${optionalLibs})

# 需要静态库解除注释
# ADD_LIBRARY(库名(不需要lib和.so) STATIC ${DIR_SRCS1} )
# SET_TARGET_PROPERTIES(库名(不需要lib和.so) PROPERTIES OUTPUT_NAME "库名(不需要lib和.so)")








