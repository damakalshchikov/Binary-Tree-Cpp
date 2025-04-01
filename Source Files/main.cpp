#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");

    // Инициализация интерпретатора Python
    Py_Initialize();

    // Импортирование Python-проекта
    PyObject* sysPath = PySys_GetObject("path");
    PyList_Append(sysPath, PyUnicode_FromString("C:/Programming/PythonProjects/CAPITest"));

    // Импортирование модуля из Python-проекта
    PyObject* pModule = PyImport_ImportModule("main");
    if (!pModule) {
        PyErr_Print();
        std::wcout << L"Ошибка. Не удалось загрузить модуль" << std::endl;
        Py_Finalize();
        return 1;
    }

    // Получение Python-функции
    PyObject* pFunc = PyObject_GetAttrString(pModule, "func");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::wcout << L"Ошибка. Не удалось найти функцию" << std::endl;
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // Инициализация аргументов для Python-функции
    PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString("Misha"));
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    // Вывод результата работы Python-функции
    if (pValue != nullptr) {
        std::wcout << L"Результат " << PyUnicode_AsUTF8(pValue) << std::endl;
        Py_DECREF(pValue);
    }
    else
        PyErr_Print();

    Py_DECREF(pModule);

    // Завершение работы интерпретатора Python
    Py_Finalize();

    return 0;
}