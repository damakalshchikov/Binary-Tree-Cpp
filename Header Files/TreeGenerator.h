//
// Created by damak on 02.04.2025.
//

#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H

#include <string>
#include <vector>
#include "Node.h"

using namespace std;

class TreeGenerator {
public:
    /**
     * Генерирует файл с представлением бинарного дерева
     * @param fileName Имя выходного файла
     * @param n Количество вершин
     */
    static void generateTestFile(const string& fileName, int n);

private:
    /**
     * Рекурсивно создает структуру бинарного дерева
     * @param values Вектор значений вершин
     * @return Указатель на корень созданного дерева или nullptr
     */
    static Node* generateTree(const vector<int>& values);

    /**
     * Создает текстовое представление дерева для записи в файл
     * @param root Корень дерева
     * @return Вектор строк для записи в файл
     */
    static vector<string> getTextTreeRepresentation(Node* root);

    /**
     * Очищает память, выделенную под дерево
     * @param root Корень дерева
     */
    static void cleanupTree(Node* root);
};

#endif //TREEGENERATOR_H
