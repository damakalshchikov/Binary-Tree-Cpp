//
// Created by damak on 02.04.2025.
//
#include "../Header Files/PathFinder.h"
#include <algorithm>
#include <unordered_set>

using namespace std;

/**
 * Находит все длиннейшие пути в бинарном дереве, вдоль которых номера вершин упорядочены по возрастанию
 * @param tree Вектор указателей на вершины дерева
 * @return Вектор путей, где каждый путь - это вектор указателей на вершины
 */
vector<vector<Node *>> PathFinder::findLongestPaths(const vector<Node *> &tree) {
    // Проверка на пустое дерево
    if (tree.empty() || tree[0] == nullptr) {
        return {};
    }

    // Кэщ для хранения максимальной длины пути, начинающегося с каждой вершины
    unordered_map<Node*, vector<vector<Node*>>> longestPathCache;
    int maxLength = 0;

    vector<vector<Node*>> allLongestPaths;

    // Ищем пути, которые начинаются с каждой вершины
    for (Node* node : tree) {
        if (node) {
            vector<vector<Node*>> nodePaths = dfs(node, longestPathCache);

            // Находим максимальную длину пути, начинающегося с этой вершины
            int longestFromNode = 0;
            for (auto& path : nodePaths) {
                longestFromNode = max(longestFromNode, static_cast<int>(path.size()));
            }

            // Обновляем максимальный путь
            if (longestFromNode > maxLength) {
                maxLength = longestFromNode;
                allLongestPaths = nodePaths;
            } else if (longestFromNode == maxLength) {
                allLongestPaths.insert(allLongestPaths.end(), nodePaths.begin(), nodePaths.end());
            }
        }
    }

    // Оставляем пути только максимального размера
    vector<vector<Node*>> resultPaths;
    unordered_set<string> seenPaths;

    for (auto& path : allLongestPaths) {
        if (path.size() == maxLength) {
            // Создаём уникальный ключ для пути
            string pathKey;
            for (Node* node : path) {
                pathKey += to_string(node->getNumber()) + "_";
            }

            // Добавляем путь, если он не встречался
            if (seenPaths.find(pathKey) == seenPaths.end()) {
                seenPaths.insert(pathKey);
                resultPaths.push_back(path);
            }
        }
    }

    return resultPaths;
}

/**
 * Рекурсивный поиск путей, начинающихся с заданной вершины
 * @param node Начальная вершина
 * @param cache Кэш для хранения уже найденных путей
 * @return Вектор путей, начинающихся с заданной вершины
 */
vector<vector<Node*>> PathFinder::dfs(Node* node, unordered_map<Node*, vector<vector<Node*>>>& cache) {
    // Проверяем, есть ли уже результат
    if (cache.find(node) == cache.end()) {
        return cache[node];
    }

    // Базовый случай - путь, который состоит только из одной вершины
    vector<vector<Node*>> paths = {{node}};

    // Проверяем левого потомка
    if (node->left && node->left->getNumber() > node->getNumber()) {
        vector<vector<Node*>> leftPaths = dfs(node->left, cache);
        for (auto& path : leftPaths) {
            vector<Node*> newPath = {node};
            newPath.insert(newPath.end(), path.begin(), path.end());
            paths.push_back(newPath);
        }
    }

    // Проверяем правого потомка
    if (node->right && node->right->getNumber() > node->getNumber()) {
        vector<vector<Node*>> rightPaths = dfs(node->right, cache);
        for (auto& path : rightPaths) {
            vector<Node*> newPath = {node};
            newPath.insert(newPath.end(), path.begin(), path.end());
            paths.push_back(newPath);
        }
    }

    // Находим максимальную длину пути
    int maxLengthPath = 0;
    for (auto& path : paths) {
        maxLengthPath = max(maxLengthPath, static_cast<int>(path.size()));
    }

    // Оставляем пути только максимальной длины
    vector<vector<Node*>> longestPathsFromNode;
    for (auto& path: paths) {
        if (path.size() > maxLengthPath) {
            longestPathsFromNode.push_back(path);
        }
    }

    // Сохраняем результат и кэш
    cache[node] = longestPathsFromNode;
    return longestPathsFromNode;
}
