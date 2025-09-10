cmake_minimum_required(VERSION 3.28.3)
project(cclshell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE release)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

include(FetchContent)

FetchContent_Declare(
        replxx
        GIT_REPOSITORY https://github.com/AmokHuginnsson/replxx.git
        GIT_TAG        master
)

FetchContent_MakeAvailable(replxx)

add_executable(cclshell
        src/main.cxx
        src/command_src/RenameCommand.cxx
        src/FileSystemUtils.cxx
        src/command_src/ChangeDirectoriesCommand.cxx
        src/command_src/RemoveDirectoriesCommand.cxx
        src/command_src/RemoveCommand.cxx
        src/command_src/MakeDirectoriesCommand.cxx
        src/command_src/CopyCommand.cxx
        src/command_src/ListDirectoriesCommand.cxx
        src/REPL.cxx
        src/command_src/RemoveCommand.cxx
        include/FunctionCommands.hxx
        src/command_src/FunctionCommands.cxx
)

include_directories(include)

if (TARGET replxx::replxx)
    target_link_libraries(cclshell PRIVATE replxx::replxx)
elseif (TARGET replxx)
    target_link_libraries(cclshell PRIVATE replxx)
else()
    message(WARNING "Couldn't find replxx target name; attempting to link 'replxx' library name")
    target_link_libraries(cclshell PRIVATE replxx)
endif()

if (UNIX)
    set_target_properties(cclshell PROPERTIES LINK_SEARCH_START_STATIC ON)
    set_target_properties(cclshell PROPERTIES LINK_SEARCH_END_STATIC ON)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

install(TARGETS cclshell DESTINATION "bin")

if(NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
            COMMENT "Uninstalling..."
    )
endif()
