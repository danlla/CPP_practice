# CMakeList.txt: проект CMake для graphs; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (graphs "graphs.cpp" "graph.h" )
add_executable (metro "metro.cpp" "graph.h"  "metro.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
