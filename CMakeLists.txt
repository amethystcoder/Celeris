cmake_minimum_required(VERSION 3.16)

project(Celeris VERSION 1.0.0 LANGUAGES CXX)

#include(CTest)
#include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Celeris 
    src/main.cpp

    # ast
    src/ast/ast.cpp
    src/ast/ast_factory.cpp
    src/ast/ast_manager.cpp

    # core
    src/core/APINode.cpp
    src/core/databaseNode.cpp
    src/core/FileSystem.cpp
    src/core/itemNode.cpp
    src/core/requestNode.cpp
    src/core/RatelimitNode.cpp
    src/core/RouteNode.cpp
    src/core/ServerNode.cpp
    src/core/storeNode.cpp
    src/core/taskNode.cpp
    src/core/ifNode.cpp

    # db
    src/db/databaseCon.cpp

    # net
    src/net/cleansocket.cpp
    src/net/clientsock.cpp
    src/net/serversock.cpp
    src/net/socket.cpp
    src/net/Socket_manager.cpp

    # platform
    src/platform/plugin_loader.cpp

    # process
    src/process/process.cpp

    # protocols
    src/protocols/http/httprequest.cpp

    # util
    src/util/fileParser.cpp
    src/util/logger.cpp
    src/util/mimetypes.cpp
    src/util/PathResolver.cpp

    # util/decisions
    src/util/decisions/decNode.cpp
    src/util/decisions/ifDecTree.cpp)


target_include_directories(Celeris PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# tests
enable_testing()
add_subdirectory(tests)
# target_link_libraries(${PROJECT_NAME} celeris_test)

# Compiler warnings
if(MSVC)
    target_compile_options(Celeris PRIVATE /W4)
else()
    target_compile_options(Celeris PRIVATE -Wall -Wextra -Wpedantic)
endif()