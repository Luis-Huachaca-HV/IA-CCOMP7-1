from ctypes import sizeof
from os import remove
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os


            

def initGrafo(G, num):
    contador = 0
    for i in range(0,num):
        if(i == 0):
            G.add_node(contador,pos = (0,i))
            contador=contador+1
        else:
            G.add_node(contador,pos = (0,i))
            contador=contador+1
            G.add_edge(i,i-1, peso = 1)

    for i in range(1,num+(num-1)):
        if(i%2==1):
            for j in range (0,num-1):
                G.add_node(contador,pos=(i,0.5+(j*1)))
                G.add_edge(contador,contador-num,peso=math.sqrt(2)/2)
                G.add_edge(contador,contador-num+1,peso=math.sqrt(2)/2)
                contador=contador+1
        else:
            for j in range(0,num):
                
                if(j==0):
                    G.add_node(contador,pos=(i,j))
                    G.add_edge(contador,contador-(num+(num-1)),peso=1)
                    G.add_edge(contador,contador-num+1,peso=math.sqrt(2)/2)
                    contador=contador+1
                    
                elif(j==num-1):
                    G.add_node(contador,pos=(i,j))
                    G.add_edge(contador,contador-(num+(num-1)),peso=1)
                    G.add_edge(contador,contador-num,peso=math.sqrt(2)/2)
                    G.add_edge(contador,contador-1,peso=1)
                    contador=contador+1
                else:
                    G.add_node(contador,pos=(i,j))
                    G.add_edge(contador,contador-(num+(num-1)),peso=1)
                    G.add_edge(contador,contador-num+1,peso=math.sqrt(2)/2)
                    G.add_edge(contador,contador-num,peso=math.sqrt(2)/2,)
                    G.add_edge(contador,contador-1,peso=1)
                    contador=contador+1
    return contador

def get_start_end_nodes(graph):
    start_node = None
    end_node = None
    while start_node is None or end_node is None:
        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(graph, 'pos')
        nx.draw_networkx(graph, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in graph.nodes()])
        plt.draw()
        plt.pause(0.001)
        click_event = plt.ginput(n=1, timeout=0)
        node = None
        for n, (x, y) in pos.items():
            if abs(x - click_event[0][0]) < 0.2 and abs(y - click_event[0][1]) < 0.2:
                node = n
                break
        if node is not None:
            if start_node is None:
                start_node = node
            elif end_node is None and node != start_node:
                end_node = node
    
    return start_node, end_node


def get_del_nodes(graph):
    start_node = None
    end_node = None
    while start_node is None or end_node is None:
        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(graph, 'pos')
        nx.draw_networkx(graph, pos, node_color=['red' if node == start_node else 'red' if node == end_node else 'white' for node in graph.nodes()])
        plt.draw()
        plt.pause(0.001)
        click_event = plt.ginput(n=1, timeout=0)
        node = None
        for n, (x, y) in pos.items():
            if abs(x - click_event[0][0]) < 0.2 and abs(y - click_event[0][1]) < 0.2:
                node = n
                break
        if node is not None:
            if start_node is None:
                start_node = node
            elif end_node is None and node != start_node:
                end_node = node
    
    return start_node, end_node

def find_shortest_path_euc(graph, start, end):
    nx.set_node_attributes(graph, 0, 'weight')
    open_set = set()
    closed_set = set()
    open_set.add(start)
    parents = {}
    graph.nodes[start]['weight'] = 0
    while len(open_set) > 0:
        current = None
        for node in open_set:
            if current is None or graph.nodes[node]['weight'] + graph.nodes[node]['h'] < graph.nodes[current]['weight'] + graph.nodes[current]['h']:
                current = node
        if current == end:
            path = []
            while current in parents:
                path.append(current)
                current = parents[current]
            path.append(start)
            path.reverse()
            return path
        open_set.remove(current)
        closed_set.add(current)
        for neighbor in graph.neighbors(current):
            if neighbor in closed_set:
                continue
            tentative_g_score = graph.nodes[current]['weight'] + graph[current][neighbor]['peso']
            if neighbor not in open_set:
                open_set.add(neighbor)
                parents[neighbor] = current
                graph.nodes[neighbor]['weight'] = tentative_g_score
                graph.nodes[neighbor]['h'] = math.sqrt((graph.nodes[neighbor]['pos'][0]-graph.nodes[end]['pos'][0])**2+(graph.nodes[neighbor]['pos'][1]-graph.nodes[end]['pos'][1])**2)
            elif tentative_g_score >= graph.nodes[neighbor]['weight']:
                continue
            parents[neighbor] = current
            graph.nodes[neighbor]['weight'] = tentative_g_score
        # Add the following code to prompt when there is no path found
        if current != end and len(open_set) == 0:
            print("No path found between start and end nodes.")
            return []


def delete_nodes_in_range(graph):
    start_node, end_node = get_del_nodes(graph)
    nodes_to_remove = []
    for node in graph.nodes():
        if node in range(start_node, end_node+1):
            nodes_to_remove.append(node)
    graph.remove_nodes_from(nodes_to_remove)
    return graph

def main():
    G = nx.Graph()
    visitados=[]

    print("Elige el tamaÃ±o del grafo: ")
    tam = int(input())
    numero_nodos=initGrafo(G, tam)
    
    G = delete_nodes_in_range(G)

    #del_node1,del_node2 = get_del_nodes(G)
    #delNodes(G,del_node1,del_node2,start_node,end_node)
    
    start_node, end_node = get_start_end_nodes(G)
    # Find the shortest path using A*
    shortest_path = find_shortest_path_euc(G, start_node, end_node)
    
    # Draw the final graph with the shortest path highlighted
    plt.clf() # Clear the previous graph
    pos = nx.get_node_attributes(G, 'pos') # get the grid positions of each node
    nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in G.nodes()])
    nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=5)
    plt.show()

if __name__ == '__main__':
    main()
