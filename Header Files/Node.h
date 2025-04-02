//
// Created by damak on 02.04.2025.
//

#ifndef NODE_H
#define NODE_H

#include <string>

using namespace std;

/**
 * Класс, представляющий вершину бинарного дерева
 */
class Node {
public:
    /**
     * Конструктор класса Node
     * @param number - Числовое значение вершины
     */
    Node(int number);

    /**
     * Получить строковое представление вершины
     * @return Строка с номером вершины
     */
    string toString() const;

    /**
     * Получить числовое значение вершины
     * @return Номер вершины
     */
    int getNumber() const;

    // Левый и правый потомки
    Node* left;
    Node* right;

private:
    // Номер вершины
    int number;
};

#endif //NODE_H
