cmake_minimum_required(VERSION 3.4.1) # 最低支持Cmake版本

# TODO 第一步：导入头文件
include_directories("inc")

# 批量导入所有源文件
file(GLOB allCPP *.c *.h *.cpp)

add_library(
        native-lib # libnative-lib.so
        SHARED # 动态库
        ${allCPP}
)

# Win path="safdsf;safdsaf;safdsafsa;fdsafdsaf;dsaf;%JAVA_HOME%"

# CMAKE_SOURCE_DIR == CMakeLists.txt所在的路径

# CMAKE_ANDROID_ARCH_ABI == 当前的CPU架构==armeabi-v7a

# ${ANDROID_ABI} 之前用的

# TODO 第二步：导入库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")

find_library(
        log-lib
        log) # 自动寻找此路径的库 D:\Android\Sdk\ndk\21.0.6113669\platforms\android-23\arch-arm64\usr\lib\liblog.so]]

# TODO 第三步，链接具体的库，到我们的 libnative-lib.so总库
target_link_libraries(
        native-lib
        ${log-lib} # 自动寻找  # 具体的库 链接到 libnative-lib.so里面去
        log # 自动寻找  # 具体的库 链接到 libnative-lib.so里面去
        fmod # 具体的库 链接到 libnative-lib.so里面去
        fmodL # 具体的库 链接到 libnative-lib.so里面去
)