cmake_minimum_required(VERSION 3.28.3)
project(cclshell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

set(SOURCES
        src/main.cxx
        src/REPL.cxx
        src/FileSystemUtils.cxx
        src/command_src/ChangeDirectoriesCommand.cxx
        src/command_src/CopyCommand.cxx
        src/command_src/FunctionCommands.cxx
        src/command_src/ListDirectoriesCommand.cxx
        src/command_src/MakeDirectoriesCommand.cxx
        src/command_src/RemoveCommand.cxx
        src/command_src/RemoveDirectoriesCommand.cxx
        src/command_src/RenameCommand.cxx
)

add_executable(cclshell ${SOURCES})
include_directories(include)

find_package(Threads REQUIRED)

find_package(replxx REQUIRED)

if(replxx_FOUND)
    message(STATUS "Found replxx package")
    target_link_libraries(cclshell PRIVATE replxx::replxx)
endif()

if(UNIX)
    set_target_properties(cclshell PROPERTIES
            LINK_SEARCH_START_STATIC ON
            LINK_SEARCH_END_STATIC ON
    )
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

install(TARGETS cclshell DESTINATION bin)