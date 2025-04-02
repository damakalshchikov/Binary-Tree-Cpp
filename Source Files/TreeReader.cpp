//
// Created by damak on 02.04.2025.
//
#include "../Header Files/TreeReader.h"
#include <fstream>
#include <string>

using namespace std;

/**
 * Читает файл и преобразует строки в вершины дерева или nullptr
 * @param fileName Имя файла
 * @return Вектор указателей на Node или nullptr
 */
vector<Node*> TreeReader::readFile(const string& fileName) {
    vector<Node*> tree;
    ifstream file(fileName);
    string line;

    if (!file.is_open()) throw runtime_error("Не удалось открыть файл: " + fileName);

    while (getline(file, line)) {
        // Удаляем пробельные символы с начала и с конца строки
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line == "None") {
            tree.push_back(nullptr);
        } else {
            try {
                int value = stoi(line);
                tree.push_back(new Node(value));
            } catch (const invalid_argument& e) {
                throw runtime_error("Некорректный формат строки в файле: " + line);
            }
        }
    }

    file.close();
    return tree;
}

/**
 * Устанавливает связи между вершинами в соответствии с их расположением в массиве
 * @param tree Вектор указателей на вершины
 */
void TreeReader::buildTreeConnections(vector<Node*>& tree) {
    int noneCount = 0;
    int limit = tree.size();

    for (int i = 0; i < limit; i++) {
        Node* node = tree[i];

        if (!node) {
            noneCount += 2;
            continue;
        }

        int leftIndex = i * 2 + 1 - noneCount;
        int rightIndex = i * 2 + 2 - noneCount;

        if (leftIndex < limit) {
            node->left = tree[leftIndex];
        }

        if (rightIndex < limit) {
            node->right = tree[rightIndex];
        }
    }
}

/**
 * Освобождает память, выделенную под дерево
 * @param tree Вектор указателей на вершины
 */
void TreeReader::cleanupTree(vector<Node*>& tree) {
    for (auto node : tree) {
        if (node) {
            delete node;
        }
    }

    tree.clear();
}
