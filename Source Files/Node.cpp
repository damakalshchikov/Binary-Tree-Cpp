//
// Created by damak on 02.04.2025.
//
#include <string>
#include "../Header Files/Node.h"

using namespace std;

/**
 * Конструктор класса Node
 * @param number - Числовое значение вершины
 */
Node::Node(int number) : number(number), left(nullptr), right(nullptr) {}

/**
 * Получить строковое представление вершины
 * @return Строка с номером вершины
 */
string Node::toString() const {
    return to_string(number);
}

/**
 * Получить числовое значение вершины
 * @return Номер вершины
 */
int Node::getNumber() const {
    return number;
}