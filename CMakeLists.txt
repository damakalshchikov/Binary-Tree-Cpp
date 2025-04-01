# Проект
cmake_minimum_required(VERSION 3.30)
project(BinaryTree)

# Версия стандарта C++
set(CMAKE_CXX_STANDARD 20)

# Добавляем исполняемый файл
add_executable(BinaryTree "Source Files/main.cpp"
        "Header Files/Node.h"
        "Header Files/TreeGenerator.h"
        "Header Files/TreeReader.h"
        "Header Files/PathFinder.h"
        "Header Files/PythonBridge.h")

# Python

# Найти установленный Python
find_package(Python3 REQUIRED COMPONENTS Development)

# Подключаем заголовочные файлы Python
include_directories(${Python3_INCLUDE_DIRS})

# Линкуем Python-библиотеку
target_link_libraries(BinaryTree ${Python3_LIBRARIES})