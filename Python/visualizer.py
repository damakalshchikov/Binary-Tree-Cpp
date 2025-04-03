"""
Модуль для визуализации бинарного дерева и путей в нем.
Использует библиотеку pydot для создания графических представлений.
"""

import os
import pydot

def draw_tree(nodes, output_png="./Images/tree.png"):
    """
    Создает изображение бинарного дерева.
    
    Args:
        nodes: Список узлов дерева в формате словарей {'number': int, 'left': int, 'right': int}
        output_png: Путь к выходному файлу изображения
    """
    # Создаем директорию для изображений, если она не существует
    os.makedirs(os.path.dirname(output_png), exist_ok=True)
    
    # Создаем новый граф
    graph = pydot.Dot(graph_type='digraph', rankdir='TB')
    
    # Добавляем узлы в граф
    for i, node in enumerate(nodes):
        if node is not None:
            # Создаем узел с номером
            graph_node = pydot.Node(str(i), label=str(node['number']), 
                                   shape="circle", style="filled", 
                                   fillcolor="lightblue")
            graph.add_node(graph_node)
            
            # Добавляем связи с потомками
            if node['left'] is not None:
                edge = pydot.Edge(str(i), str(node['left']))
                graph.add_edge(edge)
            
            if node['right'] is not None:
                edge = pydot.Edge(str(i), str(node['right']))
                graph.add_edge(edge)
    
    # Сохраняем граф в файл
    graph.write_png(output_png)
    return output_png

def draw_path(nodes, path, output_png="./Images/path.png"):
    """
    Создает изображение бинарного дерева с выделенным путем.
    
    Args:
        nodes: Список узлов дерева в формате словарей {'number': int, 'left': int, 'right': int}
        path: Список индексов узлов, образующих путь
        output_png: Путь к выходному файлу изображения
    """
    # Создаем директорию для изображений, если она не существует
    os.makedirs(os.path.dirname(output_png), exist_ok=True)
    
    # Создаем новый граф
    graph = pydot.Dot(graph_type='digraph', rankdir='TB')
    
    # Добавляем узлы в граф
    for i, node in enumerate(nodes):
        if node is not None:
            # Определяем, входит ли узел в путь
            in_path = i in path
            
            # Выбираем цвет узла в зависимости от того, входит ли он в путь
            fillcolor = "red" if in_path else "lightblue"
            
            # Создаем узел с номером
            graph_node = pydot.Node(str(i), label=str(node['number']), 
                                   shape="circle", style="filled", 
                                   fillcolor=fillcolor)
            graph.add_node(graph_node)
            
            # Добавляем связи с потомками
            if node['left'] is not None:
                # Определяем, является ли связь частью пути
                edge_in_path = i in path and node['left'] in path
                edge_color = "red" if edge_in_path else "black"
                edge_width = "2.0" if edge_in_path else "1.0"
                
                edge = pydot.Edge(str(i), str(node['left']), 
                                 color=edge_color, penwidth=edge_width)
                graph.add_edge(edge)
            
            if node['right'] is not None:
                # Определяем, является ли связь частью пути
                edge_in_path = i in path and node['right'] in path
                edge_color = "red" if edge_in_path else "black"
                edge_width = "2.0" if edge_in_path else "1.0"
                
                edge = pydot.Edge(str(i), str(node['right']), 
                                 color=edge_color, penwidth=edge_width)
                graph.add_edge(edge)
    
    # Сохраняем граф в файл
    graph.write_png(output_png)
    return output_png
