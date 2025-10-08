cmake_minimum_required(VERSION 3.28.3)
project(cshell LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

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
        include/ParsedCmd.hxx
        include/StringUtils.hxx
        src/portable_getch.c
        include/portable_getch.h
        include/using_arguments.h
        include/get_console_width.h
        src/get_console_width.c
        include/ErrorPrinter.hxx
        src/ErrorPrinter.cxx
        include/StartupDirectory.hxx
        src/StartupDirectory.cxx
)

add_executable(cshell ${SOURCES})
include_directories(include)

find_package(Threads REQUIRED)

find_package(replxx REQUIRED)

if(replxx_FOUND)
    message(STATUS "Found replxx package")
    target_link_libraries(cshell PRIVATE replxx::replxx)
endif()

if (MSVC)
    target_compile_options(cshell PRIVATE
            /W4     # warning level 4
            /WX     # treat all warnings into errors
            /permissive-
            /we4244 # 'conversion' conversion from 'type1' to 'type2', possible loss of data
            /we4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
            /we4305 # errors for floating point issues
            /we4838 # conversion from 'type_1' to 'type_2' requires a narrowing conversion
    )
else()
    target_compile_options(cshell PRIVATE -Wconversion -Wall -Werror)
endif()

install(TARGETS cshell DESTINATION bin)