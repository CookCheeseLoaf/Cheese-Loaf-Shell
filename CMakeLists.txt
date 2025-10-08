CMAKE_MINIMUM_REQUIRED(VERSION 3.28.3)
PROJECT(cshell LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

SET(SOURCES
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

ADD_EXECUTABLE(cshell ${SOURCES})
INCLUDE_DIRECTORIES(include)

FIND_PACKAGE(Threads REQUIRED)

FIND_PACKAGE(replxx REQUIRED)

IF(replxx_FOUND)
    MESSAGE(STATUS "Found replxx package")
    TARGET_LINK_LIBRARIES(cshell PRIVATE replxx::replxx)
ENDIF()

IF(MSVC)
    TARGET_COMPILE_OPTIONS(cshell PRIVATE
            /W4     # warning level 4
            /WX     # treat all warnings into errors
            /permissive-
            /we4244 # 'conversion' conversion from 'type1' to 'type2', possible loss of data
            /we4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
            /we4305 # errors for floating point issues
            /we4838 # conversion from 'type_1' to 'type_2' requires a narrowing conversion
    )
ELSE()
    TARGET_COMPILE_OPTIONS(cshell PRIVATE -Wconversion -Wall -Werror)
ENDIF()

INSTALL(TARGETS cshell DESTINATION bin)