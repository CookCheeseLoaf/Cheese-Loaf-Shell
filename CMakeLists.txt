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

function(setup_replxx target)
    find_package(Threads REQUIRED)

    find_package(replxx QUIET)

    if(replxx_FOUND)
        message(STATUS "Found replxx package")
        target_link_libraries(${target} PRIVATE replxx::replxx)
    else()
        message(WARNING "replxx package not found. Building from source.")
        include(FetchContent)
        FetchContent_Declare(
                replxx
                GIT_REPOSITORY https://github.com/AmokHuginnsson/replxx.git
                GIT_TAG        master
                CMAKE_ARGS -DREPLXX_BUILD_PACKAGE=OFF -DBUILD_SHARED_LIBS=OFF
                EXCLUDE_FROM_ALL
        )
        FetchContent_MakeAvailable(replxx)
        target_link_libraries(${target} PRIVATE replxx)
    endif()
endfunction()

setup_replxx(cclshell)

if(UNIX)
    set_target_properties(cclshell PROPERTIES
            LINK_SEARCH_START_STATIC ON
            LINK_SEARCH_END_STATIC ON
    )
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

install(TARGETS cclshell DESTINATION bin)