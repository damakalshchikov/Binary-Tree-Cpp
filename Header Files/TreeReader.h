//
// Created by damak on 02.04.2025.

#ifndef TREEREADER_H
#define TREEREADER_H

#include <string>
#include <vector>
#include <memory>
#include "Node.h"

/**
 * Класс для чтения файлов и создания структуры дерева
 */
class TreeReader {
public:
    /**
    * Читает файл и преобразует строки в вершины дерева или nullptr
    * @param fileName Имя файла
    * @return Вектор указателей на Node или nullptr
    */
    static vector<Node*> readFile(const string& fileName);

    /**
     * Устанавливает связи между вершинами в соответствии с их расположением в массиве
     * @param tree Вектор указателей на вершины
     */
    static void buildTreeConnections(vector<Node*>& tree);

    /**
     * Освобождает память, выделенную под дерево
     * @param tree Вектор указателей на вершины
     */
    static void cleanupTree(vector<Node*>& tree);
};

#endif //TREEREADER_H