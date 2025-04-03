#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../Header Files/Node.h"
#include "../Header Files/TreeReader.h"
#include "../Header Files/PathFinder.h"
#include "../Header Files/PythonBridge.h"

int main() {
    // Устанавливаем локаль для корректного отображения русских символов
    setlocale(LC_ALL, "");

    // Инициализация интерпретатора Python
    PythonBridge::initialize();

    // Очистка папки с изображениями
    PythonBridge::cleanImagesFolder("./Images");

    std::wcout << L"Программа для поиска длиннейшего пути с возрастающими значениями в бинарном дереве" << std::endl;
    std::cout << "---------------------------------------------------------------------------------" << std::endl;

    // Запрос имени файла с деревом
    std::string filename;
    std::wcout << L"Введите имя файла с деревом (или нажмите Enter для использования 'tree.txt'): ";
    std::getline(std::cin, filename);
    if (filename.empty()) {
        filename = "./Cases/tree.txt";
    }

    // Чтение дерева из файла
    std::vector<Node*> tree;
    try {
        tree = TreeReader::readFile(filename);
        std::wcout << L"Дерево успешно загружено из файла " << std::endl;
    } catch (const std::exception& e) {
        std::wcerr << L"Ошибка при чтении дерева из файла: " << e.what() << std::endl;
        PythonBridge::finalize();
        return 1;
    }

    // Визуализация дерева
    try {
        PythonBridge::drawTree(tree, "./Images/tree.png");
        std::wcout << L"Изображение дерева сохранено в файл ./Images/tree.png" << std::endl;
    } catch (const std::exception& e) {
        std::wcerr << L"Ошибка при создании изображения дерева: " << e.what() << std::endl;
    }

    // Поиск длиннейшего пути с возрастающими значениями
    std::vector<std::vector<Node*>> longestPaths = PathFinder::findLongestPaths(tree);

    if (longestPaths.empty()) {
        std::wcout << L"В дереве не найдено путей с возрастающими значениями" << std::endl;
    } else {
        std::wcout << L"\nНайдено " << longestPaths.size() << L" длиннейших путей с возрастающими значениями:" << std::endl;
        
        for (size_t i = 0; i < longestPaths.size(); ++i) {
            const auto& path = longestPaths[i];
            
            // Вывод пути
            std::wcout << L"Путь " << (i + 1) << L" (длина " << path.size() << "): ";
            for (size_t j = 0; j < path.size(); ++j) {
                std::cout << path[j]->getNumber();
                if (j < path.size() - 1) {
                    std::cout << " -> ";
                }
            }
            std::cout << std::endl;
            
            // Визуализация пути
            try {
                std::string outputFile = "./Images/path_" + std::to_string(i + 1) + ".png";
                PythonBridge::drawPath(tree, path, outputFile);
                //std::wcout << L"Изображение пути сохранено в файл " << outputFile << std::endl;
            } catch (const std::exception& e) {
                std::wcerr << L"Ошибка при создании изображения пути: " << e.what() << std::endl;
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

    std::wcout << L"\nПрограмма завершена. Нажмите Enter для выхода...";
    std::cin.get();
    
    return 0;
}
