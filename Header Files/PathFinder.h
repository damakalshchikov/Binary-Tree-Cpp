//
// Created by damak on 02.04.2025.
//

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "Node.h"

using namespace std;


/**
 * Класс для поиска длиннейших путей в бинарном дереве
 */
class PathFinder {
public:

    /**
     * Находит все длиннейшие пути в бинарном дереве, вдоль которых номера вершин упорядочены по возрастанию
     * @param tree Вектор указателей на вершины дерева
     * @return Вектор путей, где каждый путь - это вектор указателей на вершины
     */
    static vector<vector<Node*>> findLongestPaths(const vector<Node*>& tree);

private:
    /**
     * Рекурсивный поиск путей, начинающихся с заданной вершины
     * @param node Начальная вершина
     * @param cache Уже найденные пути
     * @return Вектор путей, начинающихся с заданной вершины
     */
    static vector<vector<Node*>> dfs(Node* node, unordered_map<Node*, vector<vector<Node*>>>& cache);
};

#endif //PATHFINDER_H
