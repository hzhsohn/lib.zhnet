
#设置工程名称
PROJECT(dtrs_trans)

#设置CMAKE最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#编译选项
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_STANDARD 11)
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    message(^^^^^^  编译成Debug模式  ^^^^^^^)
    set(ADD_DEFAULT "-g -Wall -Wno-unused-variable -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADD_DEFAULT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADD_DEFAULT} -std=c++11")

else( CMAKE_BUILD_TYPE STREQUAL "Debug" )  
    message(^^^^^^  编译顾Release模式  ^^^^^^^)
    set(CMAKE_BUILD_TYPE "Release")
    set(ADD_DEFAULT "-O1 -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADD_DEFAULT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADD_DEFAULT} -std=c++11")

endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )
#message("Build Type:"${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})


message(^^^^^^  添加编译宏参数  ^^^^^^^)
if(APPLE)
add_definitions(-DOS_MACOSX)
elseif(UNIX)
add_definitions(-DOS_LINUX)
endif()

message(^^^^^^  设置头文件目录  ^^^^^^^)
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib

	${CMAKE_CURRENT_SOURCE_DIR}/lib/AESStringCrypt
	${CMAKE_CURRENT_SOURCE_DIR}/lib/findfile
	${CMAKE_CURRENT_SOURCE_DIR}/lib/urlencode
	${CMAKE_CURRENT_SOURCE_DIR}/lib/XTTransCrypt
	${CMAKE_CURRENT_SOURCE_DIR}/lib/zhHttp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/json
	${CMAKE_CURRENT_SOURCE_DIR}/lib/md5
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sha1
	${CMAKE_CURRENT_SOURCE_DIR}/lib/xml
	${CMAKE_CURRENT_SOURCE_DIR}/lib/zhlist
	${CMAKE_CURRENT_SOURCE_DIR}/lib/zhNet_Core
	${CMAKE_CURRENT_SOURCE_DIR}/lib/print_msg
	${CMAKE_CURRENT_SOURCE_DIR}/lib/base64
	${CMAKE_CURRENT_SOURCE_DIR}/lib/miniData
	${CMAKE_CURRENT_SOURCE_DIR}/lib/hxnet
	${CMAKE_CURRENT_SOURCE_DIR}/lib/sbufhex
	${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/ini
	${CMAKE_CURRENT_SOURCE_DIR}/lib/handy/handy
	${CMAKE_CURRENT_SOURCE_DIR}/lib/dtrsNetLib
)


message(^^^^^^  链接库搜索路径  ^^^^^^^)
LINK_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}/lib
)

message(^^^^^^  添加子目录  ^^^^^^^)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/base64)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/zhNet_Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/AESStringCrypt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/findfile)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/urlencode)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/XTTransCrypt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/zhHttp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/md5)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/sha1)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/xml)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/zhlist)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/print_msg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/hxnet)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/miniData)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/sbufhex)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/ini)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/handy/handy)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/dtrsNetLib)


message(^^^^^^  添加要编译的可执行文件  ^^^^^^^)
ADD_EXECUTABLE(${PROJECT_NAME} 
	src/main.cpp
)
#执行文件链接属性
TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
	src #项目代码一定要放在最前面,不然会编译时找不到其它库
	print_msg
	zhNetCore
	#zhlist
	json
	mxml
	AesStringCrypt
	#findfile
	urlencode
	#zhHttp
	md5
	#sha1
	base64
	miniData
	#hxnet
	#sbufhex
	#zlib
	zhini
	handy
	dtrsNetLib
)

#设置安装地址
set(CMAKE_INSTALL_PREFIX /usr/local)
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
