cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 17)

set(plugin_name "fanqie")

project(${plugin_name} VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr
    GIT_TAG        dec9422db3af470641f8b0d90e4b451c4daebf64) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    cryptopp_cmake
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git)
FetchContent_MakeAvailable(cryptopp_cmake)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
    easyloggingpp
    GIT_REPOSITORY https://github.com/abumq/easyloggingpp)
FetchContent_MakeAvailable(easyloggingpp)

# include_directories(${easyloggingpp_SOURCE_DIR}/src/)

include_directories(${PROJECT_SOURCE_DIR}/utils)

add_library(${plugin_name} SHARED ${plugin_name}.cpp)

include_directories(${easyloggingpp_SOURCE_DIR}/src/)
target_link_directories(${plugin_name} PRIVATE ${easyloggingpp_SOURCE_DIR}/src/)

target_link_libraries(${plugin_name} PRIVATE cpr::cpr)
target_link_libraries(${plugin_name} PRIVATE fmt::fmt)
target_link_libraries(${plugin_name} PRIVATE cryptopp::cryptopp)
target_link_libraries(${plugin_name} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${plugin_name} PRIVATE SQLiteCpp)
# target_link_libraries(${plugin_name} PRIVATE easyloggingpp::easyloggingpp)

# add_compile_options(-g -O0)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set_target_properties(${plugin_name} PROPERTIES
    VERSION 1.0
    SOVERSION 1
    PREFIX ""
    OUTPUT_NAME ${plugin_name}
)