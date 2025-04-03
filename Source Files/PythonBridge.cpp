//
// Created by damak on 02.04.2025.
//

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include "../Header Files/PythonBridge.h"

// Статическая переменная для отслеживания состояния инициализации Python
bool PythonBridge::isPythonInitialized = false;

/**
 * Инициализирует интерпретатор Python и загружает модули
 */
void PythonBridge::initialize() {
    if (!isPythonInitialized) {
        Py_Initialize();
        isPythonInitialized = true;
    }
}

/**
 * Освобождает ресурсы Python
 */
void PythonBridge::finalize() {
    if (isPythonInitialized) {
        Py_Finalize();
        isPythonInitialized = false;
    }
}

/**
 * Очищает папку с изображениями
 * @param folder Путь к папке с изображениями
 */
void PythonBridge::cleanImagesFolder(const std::string& folder) {
    if (!isPythonInitialized) {
        initialize();
    }
    
    // Импортируем модуль os для работы с файловой системой
    PyObject* osModule = PyImport_ImportModule("os");
    if (!osModule) {
        PyErr_Print();
        return;
    }
    
    // Импортируем модуль shutil для удаления директорий
    PyObject* shutilModule = PyImport_ImportModule("shutil");
    if (!shutilModule) {
        Py_DECREF(osModule);
        PyErr_Print();
        return;
    }
    
    // Проверяем существование директории
    PyObject* pathExists = PyObject_GetAttrString(osModule, "path");
    PyObject* existsFunc = PyObject_GetAttrString(pathExists, "exists");
    PyObject* args = Py_BuildValue("(s)", folder.c_str());
    PyObject* result = PyObject_CallObject(existsFunc, args);
    
    if (PyObject_IsTrue(result)) {
        // Если директория существует, удаляем все файлы в ней
        PyObject* rmtreeFunc = PyObject_GetAttrString(shutilModule, "rmtree");
        PyObject* rmArgs = Py_BuildValue("(s)", folder.c_str());
        PyObject_CallObject(rmtreeFunc, rmArgs);
        Py_DECREF(rmtreeFunc);
        Py_DECREF(rmArgs);
    }
    
    // Создаем директорию заново
    PyObject* mkdirFunc = PyObject_GetAttrString(osModule, "makedirs");
    PyObject* mkdirArgs = Py_BuildValue("(si)", folder.c_str(), 0755);
    PyObject_CallObject(mkdirFunc, mkdirArgs);
    
    // Освобождаем ресурсы
    Py_DECREF(osModule);
    Py_DECREF(shutilModule);
    Py_DECREF(pathExists);
    Py_DECREF(existsFunc);
    Py_DECREF(args);
    Py_DECREF(result);
    Py_DECREF(mkdirFunc);
    Py_DECREF(mkdirArgs);
}

/**
 * Создает изображение бинарного дерева
 * @param tree Вектор указателей на вершины дерева
 * @param outputPng Имя выходного файла
 */
void PythonBridge::drawTree(const std::vector<Node*>& tree, const std::string& outputPng) {
    if (!isPythonInitialized) {
        initialize();
    }
    
    // Импортируем модуль visualizer из нашего пакета Python
    PyObject* visualizerModule = PyImport_ImportModule("Python.visualizer");
    if (!visualizerModule) {
        PyErr_Print();
        return;
    }
    
    // Получаем функцию draw_tree из модуля
    PyObject* drawTreeFunc = PyObject_GetAttrString(visualizerModule, "draw_tree");
    if (!drawTreeFunc || !PyCallable_Check(drawTreeFunc)) {
        Py_XDECREF(drawTreeFunc);
        Py_DECREF(visualizerModule);
        PyErr_Print();
        return;
    }
    
    // Создаем список узлов для передачи в Python
    PyObject* nodesList = PyList_New(tree.size());
    for (size_t i = 0; i < tree.size(); ++i) {
        if (tree[i]) {
            // Для каждого узла создаем словарь с его данными
            PyObject* nodeDict = PyDict_New();
            
            // Добавляем номер узла
            PyDict_SetItemString(nodeDict, "number", PyLong_FromLong(tree[i]->getNumber()));
            
            // Добавляем левого потомка (если есть)
            if (tree[i]->left) {
                for (size_t j = 0; j < tree.size(); ++j) {
                    if (tree[j] == tree[i]->left) {
                        PyDict_SetItemString(nodeDict, "left", PyLong_FromLong(j));
                        break;
                    }
                }
            } else {
                PyDict_SetItemString(nodeDict, "left", Py_None);
                Py_INCREF(Py_None);
            }
            
            // Добавляем правого потомка (если есть)
            if (tree[i]->right) {
                for (size_t j = 0; j < tree.size(); ++j) {
                    if (tree[j] == tree[i]->right) {
                        PyDict_SetItemString(nodeDict, "right", PyLong_FromLong(j));
                        break;
                    }
                }
            } else {
                PyDict_SetItemString(nodeDict, "right", Py_None);
                Py_INCREF(Py_None);
            }
            
            PyList_SetItem(nodesList, i, nodeDict);
        } else {
            Py_INCREF(Py_None);
            PyList_SetItem(nodesList, i, Py_None);
        }
    }
    
    // Вызываем функцию draw_tree
    PyObject* args = PyTuple_New(2);
    PyTuple_SetItem(args, 0, nodesList);
    PyTuple_SetItem(args, 1, PyUnicode_FromString(outputPng.c_str()));
    
    PyObject* result = PyObject_CallObject(drawTreeFunc, args);
    
    // Проверяем результат
    if (!result) {
        PyErr_Print();
    }
    
    // Освобождаем ресурсы
    Py_XDECREF(result);
    Py_DECREF(args);
    Py_DECREF(drawTreeFunc);
    Py_DECREF(visualizerModule);
}

/**
 * Создает изображение бинарного дерева с выделенным путем
 * @param tree Вектор указателей на вершины дерева
 * @param path Путь, который нужно выделить
 * @param outputPng Имя выходного файла
 */
void PythonBridge::drawPath(const std::vector<Node*>& tree, 
                           const std::vector<Node*>& path, 
                           const std::string& outputPng) {
    if (!isPythonInitialized) {
        initialize();
    }
    
    // Импортируем модуль visualizer из нашего пакета Python
    PyObject* visualizerModule = PyImport_ImportModule("Python.visualizer");
    if (!visualizerModule) {
        PyErr_Print();
        return;
    }
    
    // Получаем функцию draw_path из модуля
    PyObject* drawPathFunc = PyObject_GetAttrString(visualizerModule, "draw_path");
    if (!drawPathFunc || !PyCallable_Check(drawPathFunc)) {
        Py_XDECREF(drawPathFunc);
        Py_DECREF(visualizerModule);
        PyErr_Print();
        return;
    }
    
    // Создаем список узлов для передачи в Python
    PyObject* nodesList = PyList_New(tree.size());
    for (size_t i = 0; i < tree.size(); ++i) {
        if (tree[i]) {
            // Для каждого узла создаем словарь с его данными
            PyObject* nodeDict = PyDict_New();
            
            // Добавляем номер узла
            PyDict_SetItemString(nodeDict, "number", PyLong_FromLong(tree[i]->getNumber()));
            
            // Добавляем левого потомка (если есть)
            if (tree[i]->left) {
                for (size_t j = 0; j < tree.size(); ++j) {
                    if (tree[j] == tree[i]->left) {
                        PyDict_SetItemString(nodeDict, "left", PyLong_FromLong(j));
                        break;
                    }
                }
            } else {
                PyDict_SetItemString(nodeDict, "left", Py_None);
                Py_INCREF(Py_None);
            }
            
            // Добавляем правого потомка (если есть)
            if (tree[i]->right) {
                for (size_t j = 0; j < tree.size(); ++j) {
                    if (tree[j] == tree[i]->right) {
                        PyDict_SetItemString(nodeDict, "right", PyLong_FromLong(j));
                        break;
                    }
                }
            } else {
                PyDict_SetItemString(nodeDict, "right", Py_None);
                Py_INCREF(Py_None);
            }
            
            PyList_SetItem(nodesList, i, nodeDict);
        } else {
            Py_INCREF(Py_None);
            PyList_SetItem(nodesList, i, Py_None);
        }
    }
    
    // Создаем список индексов узлов пути
    PyObject* pathList = PyList_New(path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        for (size_t j = 0; j < tree.size(); ++j) {
            if (tree[j] == path[i]) {
                PyList_SetItem(pathList, i, PyLong_FromLong(j));
                break;
            }
        }
    }
    
    // Вызываем функцию draw_path
    PyObject* args = PyTuple_New(3);
    PyTuple_SetItem(args, 0, nodesList);
    PyTuple_SetItem(args, 1, pathList);
    PyTuple_SetItem(args, 2, PyUnicode_FromString(outputPng.c_str()));
    
    PyObject* result = PyObject_CallObject(drawPathFunc, args);
    
    // Проверяем результат
    if (!result) {
        PyErr_Print();
    }
    
    // Освобождаем ресурсы
    Py_XDECREF(result);
    Py_DECREF(args);
    Py_DECREF(drawPathFunc);
    Py_DECREF(visualizerModule);
}
