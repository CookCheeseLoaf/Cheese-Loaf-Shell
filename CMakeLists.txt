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

    # Search for libreplxx.a in common system directories
    find_library(REPLXX_STATIC_LIB
            NAMES replxx
            PATHS /usr/local/lib /usr/lib
            NO_DEFAULT_PATH
    )

    if(REPLXX_STATIC_LIB AND EXISTS ${REPLXX_STATIC_LIB})
        message(STATUS "Found replxx static library at: ${REPLXX_STATIC_LIB}")

        add_library(replxx::replxx STATIC IMPORTED)
        set_target_properties(replxx::replxx PROPERTIES
                IMPORTED_LOCATION ${REPLXX_STATIC_LIB}
        )

        find_path(REPLXX_INCLUDE_DIR
                NAMES replxx.h
                PATHS /usr/local/include /usr/include
                NO_DEFAULT_PATH
        )

        if(REPLXX_INCLUDE_DIR)
            set_target_properties(replxx::replxx PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES ${REPLXX_INCLUDE_DIR}
            )
        else()
            message(WARNING "replxx include directory not found. Building from source.")
            set(REPLXX_STATIC_LIB "REPLXX_STATIC_LIB-NOTFOUND") # Force FetchContent
        endif()
    endif()

    if(NOT REPLXX_STATIC_LIB OR NOT EXISTS ${REPLXX_STATIC_LIB})
        message(WARNING "replxx not found. Building from source.")
        include(FetchContent)
        FetchContent_Declare(
                replxx
                GIT_REPOSITORY https://github.com/AmokHuginnsson/replxx.git
                GIT_TAG        master
                CMAKE_ARGS -DREPLXX_BUILD_PACKAGE=OFF -DBUILD_SHARED_LIBS=OFF
        )
        FetchContent_MakeAvailable(replxx)
        target_link_libraries(${target} PRIVATE replxx)

        set_target_properties(replxx PROPERTIES EXCLUDE_FROM_ALL TRUE)
        if(TARGET replxx::replxx)
            set_target_properties(replxx PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()
    else()
        # Use the found system library
        target_link_libraries(${target} PRIVATE replxx::replxx)
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