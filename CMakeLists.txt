cmake_minimum_required(VERSION 3.10)
project(ECE141db)


set(CMAKE_CXX_STANDARD 17)


include_directories(include)


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

set(SOURCES ${SOURCES} ${HEADERS}
        include/commandHandler/BasicHandler.hpp
        include/commandHandler/CommandHandler.hpp
        include/commandHandler/DatabaseHandler.hpp
        include/view/QueryView.hpp
        include/commandHandler/SQLProcessor.hpp
        include/statement/Statement.hpp
        include/parsing/ValueParser.hpp
        include/statement/SelectStatement.hpp
        include/parsing/SelectParser.hpp
        include/parsing/ExpressionParser.hpp
        include/parsing/ClauseParser.hpp
        include/parsing/OperandParser.hpp
        include/parsing/OperatorParser.hpp
        include/statement/UpdateStatement.hpp
        include/parsing/DeleteParser.hpp)



add_executable(ECE141db ${SOURCES})

#target_compile_options(ECE141db PRIVATE -Wall -Wreorder -Wunused-variable -Wreturn-type -Wpedantic)

add_subdirectory(Google_tests)