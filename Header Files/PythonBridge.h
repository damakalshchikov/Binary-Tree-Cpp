//
// Created by damak on 02.04.2025.
//

#ifndef PYTHONBRIDGE_H
#define PYTHONBRIDGE_H

#include <string>
#include <vector>
#include "Node.h"

using namespace std;

/**
 * Класс для взаимодействия с Python функциями
 */
class PythonBridge {
public:
    /**
    * Инициализирует интерпретатор Python и загружает модули
    */
    static void initialize();

    /**
    * Освобождает ресурсы Python
    */
    static void finalize();

    /**
    * Очищает папку с изображениями
    * @param folder Путь к папке с изображениями
    */
    static void cleanImagesFolder(const string& folder = "./Images/");

    /**
    * Создает изображение бинарного дерева
    * @param tree Вектор указателей на вершины дерева
    * @param outputPng Имя выходного файла
    */
    static void drawTree(const vector<Node*>& tree, const string& outputPng = "./Images/tree.png");

    /**
    * Создает изображение бинарного дерева с выделенным путем
    * @param tree Вектор указателей на вершины дерева
    * @param path Путь, который нужно выделить
    * @param outputPng Имя выходного файла
    */
    static void drawPath(
        const vector<Node*>& tree,
        const vector<Node*>& path,
        const string& outputPng = "./Images/path.png");

private:
    static bool isPythonInitialized;
};

#endif //PYTHONBRIDGE_H
