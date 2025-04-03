#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../Header Files/Node.h"
#include "../Header Files/TreeReader.h"
#include "../Header Files/TreeGenerator.h"
#include "../Header Files/PathFinder.h"
#include "../Header Files/PythonBridge.h"

using namespace std;

int main() {
    // Устанавливаем локаль для корректного отображения русских символов
    setlocale(LC_ALL, "");

    // Инициализация интерпретатора Python
    PythonBridge::initialize();

    // Очистка папки с изображениями
    PythonBridge::cleanImagesFolder("./Images");

    wcout << L"Программа для поиска длиннейшего пути с возрастающими значениями в бинарном дереве" << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    // Запрос имени файла с деревом
    string filename;
    wcout << L"Введите имя файла с деревом (или нажмите Enter для использования 'tree.txt'): ";
    getline(std::cin, filename);
    if (filename.empty()) {
        filename = "./Cases/tree.txt";
    }

    // Чтение дерева из файла
    vector<Node*> tree;
    try {
        tree = TreeReader::readFile(filename);
        wcout << L"Дерево успешно загружено из файла " << endl;
    } catch (const std::exception& e) {
        wcerr << L"Ошибка при чтении дерева из файла: " << e.what() << endl;
        PythonBridge::finalize();
        return 1;
    }

    // Устанавливаем связи между вершинами
    TreeReader::buildTreeConnections(tree);

    // Визуализация дерева
    try {
        PythonBridge::drawTree(tree, "./Images/tree.png");
        wcout << L"Изображение дерева сохранено в файл ./Images/tree.png" << endl;
    } catch (const exception& e) {
        wcerr << L"Ошибка при создании изображения дерева: " << e.what() << endl;
    }

    // Поиск длиннейшего пути с возрастающими значениями
    vector<vector<Node*>> longestPaths = PathFinder::findLongestPaths(tree);

    if (longestPaths.empty()) {
        wcout << L"В дереве не найдено путей с возрастающими значениями" << endl;
    } else {
        wcout << L"\nНайдено " << longestPaths.size() << L" длиннейших путей с возрастающими значениями:" << endl;
        
        for (size_t i = 0; i < longestPaths.size(); ++i) {
            const auto& path = longestPaths[i];
            
            // Вывод пути
            wcout << L"Путь " << (i + 1) << L" (длина " << path.size() << "): ";
            for (size_t j = 0; j < path.size(); ++j) {
                cout << path[j]->getNumber();
                if (j < path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
            
            // Визуализация пути
            try {
                string outputFile = "./Images/path_" + std::to_string(i + 1) + ".png";
                PythonBridge::drawPath(tree, path, outputFile);
                //std::wcout << L"Изображение пути сохранено в файл " << outputFile << std::endl;
            } catch (const std::exception& e) {
                wcerr << L"Ошибка при создании изображения пути: " << e.what() << endl;
            }
        }
    }

    // Освобождение памяти
    for (auto node : tree) {
        delete node;
    }
    tree.clear();

    // Завершение работы с Python
    PythonBridge::finalize();

    wcout << L"\nПрограмма завершена. Нажмите Enter для выхода...";
    cin.get();
    
    return 0;
}
