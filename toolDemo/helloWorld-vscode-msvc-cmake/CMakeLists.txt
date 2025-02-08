#cmake的最低版本要求
cmake_minimum_required(VERSION 3.5.0)

#定义项目工程名称，并指定版本与语言
project(helloworld_cmake VERSION 0.1.0 LANGUAGES C CXX)

#C++标准设置
set(CMAKE_CXX_STANDARD 17) #C++ 17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求使用C++17标准


#向项目添加多个头文件搜索目录
set(CMAKE_INCLUDE_CURRENT_DIR ON) #将当前目录视为包含目录
include_directories(./depend/include/) #第三方依赖头文件目录

#向项目添加多个库文件搜索目录
link_directories(./depend/lib/)


# Qt相关
# 启用Qt的自动处理功能
set(CMAKE_AUTOUIC ON)  # 自动处理UI文件
set(CMAKE_AUTOMOC ON)  # 自动处理moc文件
set(CMAKE_AUTORCC ON)  # 自动处理资源文件
find_package(Qt5Core REQUIRED) #导入Qt包



#设置要编译的源文件列表
#方法1,需要一个个列出要编译的源文件
#set(SOURCES src/main.cpp src/algorithm/basic.cpp) #set可以设置或者修改 变量
#add_executable(helloworld_cmake ${SOURCES}) #将编译目标添加到可执行文件

#方法2，这个方法可以使用通配符，但还是方法1更加有利于代码管理
#file(GLOB SOURCES src/algorithm/*.cpp src/*cpp) #这个可以添加一个文件夹里的所有cpp文件
#add_executable(helloworld_cmake ${SOURCES} )

#方法3，这个在上一个的基础上，可以递归获取文件夹下含子文件夹的所有cpp文件
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(helloworld_cmake ${SOURCES})


#程序链接
#为编译目标设定要链接的库
target_link_libraries(helloworld_cmake libmysql)
target_link_libraries(helloworld_cmake Qt5::Core)


#在构建过程中执行一些自定义命令
#执行windows下的 windeployqt 自动补上需要的qt动态库
if(WIN32)
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND windeployqt ARGS $<TARGET_FILE_DIR:helloworld_cmake>
                        COMMENT "-------------execute windeployqt--------------"
    )
endif()


#设置 CTest
include(CTest)
enable_testing()

#设置 CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

