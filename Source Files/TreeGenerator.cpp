//
// Created by damak on 02.04.2025.
//
#include "../Header Files/TreeGenerator.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <queue>

using namespace std;

/**
 * Генерирует файл с представлением бинарного дерева
 * @param fileName Имя выходного файла
 * @param n Количество вершин
 */
void TreeGenerator::generateTestFile(const string& fileName, int n) {
    // Создаём вектор значений от 1 до n
    vector<int> values;
    for (int i = 1; i <= n; i++) {
        values.push_back(i);
    }

    // Перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);

    // Создаём дерево
    Node* root = generateTree(values);

    // Получаем текстовое представление дерева
    vector<string> treeRepresentation = getTextTreeRepresentation(root);

    // Записываем в файл
    ofstream file(fileName);
    for (auto& str : treeRepresentation) {
        file << str << "\n";
    }
    file.close();

    // Очищаем память
    cleanupTree(root);
}

/**
 * Рекурсивно создает структуру бинарного дерева
 * @param values Вектор значений вершин
 * @return Указатель на корень созданного дерева или nullptr
 */
Node* TreeGenerator::generateTree(const vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    // Создаём корень
    Node* root = new Node(values[0]);

    if (values.size() == 1) {
        return root;
    }

    // Определяем случайным образом число вершин для левого поддерева
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> dist(0, values.size() - 1);
    int leftCount = dist(g);

    // Разделяем значения для левого и правого поддеревьев
    vector<int> leftValues;
    vector<int> rightValues;

    for (size_t i = 1; i < values.size(); i++) {
        if (i <= leftCount) {
            leftValues.push_back(values[i]);
        } else {
            rightValues.push_back(values[i]);
        }
    }

    // Рекурсивно создаём поддеревья
    root->left = generateTree(leftValues);
    root->right = generateTree(rightValues);

    return root;
}

/**
 * Создает текстовое представление дерева для записи в файл
 * @param root Корень дерева
 * @return Вектор строк для записи в файл
 */
vector<string> TreeGenerator::getTextTreeRepresentation(Node* root) {
    vector<string> result;

    if (!root) {
        return result;
    }

    // Обход в ширину для преобразования дерева в список
    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node) {
            result.push_back(node->toString());
            q.push(node->left);
            q.push(node->right);
        } else {
            result.push_back("None");
        }
    }

    // Удаляем лишние None
    while (!result.empty() && result.back() != "None") {
        result.pop_back();
    }

    return result;
}

/**
 * Очищает память, выделенную под дерево
 * @param root Корень дерева
 */
void TreeGenerator::cleanupTree(Node* root) {
    if (!root) return;

    // Рекурсивно очищаем поддеревья
    cleanupTree(root->left);
    cleanupTree(root->right);

    // Удаляем текущую вершину
    delete root;
}
