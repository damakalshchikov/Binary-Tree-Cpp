#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "../Header Files/Node.h"
#include "../Header Files/TreeReader.h"
#include "../Header Files/TreeGenerator.h"
#include "../Header Files/PathFinder.h"
#include "../Header Files/PythonBridge.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "");

    // Инициализация интерпретатора Python
    PythonBridge::initialize();

    // Очистка папки с изображениями
    PythonBridge::cleanImagesFolder("./Images");

    wcout << L"Программа для поиска длиннейшего пути(путей) с возрастающими значениями в бинарном дереве" << endl;

    int modeProgram;
    // Выбор режима работы программы
    wcout << L"Выберете режим работы программы:" << endl;
    wcout << L"1 - чтение существующего файла" << endl;
    wcout << L"2 - генерация файла и его последующее чтение" << endl << endl;
    wcout << L"Введите режим работы программы: ";
    cin >> modeProgram;

    // Запрос имени файла
    string filename;
    wcout << L"Введите имя файла: ";
    cin >> filename;

    if (modeProgram == 2) {
        int n;
        wcout << L"Введите количество вершин в дереве:  ";
        cin >> n;
        TreeGenerator::generateTestFile(filename, n);
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

    // Начало замера времени
    auto start = chrono::steady_clock::now();
    // Поиск длиннейшего пути с возрастающими значениями
    vector<vector<Node*>> longestPaths = PathFinder::findLongestPaths(tree);
    // Конец замера времени
    auto end = chrono::steady_clock::now();

    long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    wcout << L"Время поиска путей: " << duration << L"мксек" << endl;

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
            string outputFile = "./Images/path_" + std::to_string(i + 1) + ".png";
            PythonBridge::drawPath(tree, path, outputFile);
        }
    }

    // Освобождение памяти
    for (auto node : tree) {
        delete node;
    }
    tree.clear();

    // Завершение работы с Python
    PythonBridge::finalize();

    wcout << L"\nПрограмма завершена.";
    
    return 0;
}
