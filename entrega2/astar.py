from ctypes import sizeof
from os import remove
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os


def delNodes(G, porcentaje, cantidad_nodos, ini, fin):
    porcentajeInt = math.floor((cantidad_nodos-2)*porcentaje/100)
    print("Porcentaje",porcentajeInt)
    eliminados=[]
    nodo_eliminado=-1
    for i in range(0,porcentajeInt):
        while nodo_eliminado == ini or nodo_eliminado == fin or eliminados.count(nodo_eliminado)!=0 or nodo_eliminado==-1:
            nodo_eliminado=random.randint(0, cantidad_nodos-1)
        G.remove_node(nodo_eliminado)
        eliminados.append(nodo_eliminado)
    return len(eliminados)

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
        # Draw the graph with the current start and end nodes highlighted
        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(graph, 'pos')
        nx.draw_networkx(graph, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in graph.nodes()])
        plt.draw()
        plt.pause(0.001)
        
        # Get the next user input
        click_event = plt.ginput(n=1, timeout=0)
        node = None
        for n, (x, y) in pos.items():
            if abs(x - click_event[0][0]) < 0.05 and abs(y - click_event[0][1]) < 0.05:
                node = n
                break
                
        # Update the start or end node if the user clicked on a node
        if node is not None:
            if start_node is None:
                start_node = node
            elif end_node is None and node != start_node:
                end_node = node
    
    return start_node, end_node

def find_shortest_path(graph, start, end):
    # Set the weight attribute for all nodes in the graph
    nx.set_node_attributes(graph, 0, 'weight')

    # Compute the shortest path using A*
    path = nx.astar_path(graph, start, end, heuristic=lambda u, v: graph.nodes[v]['weight'])# here we define the shortest path given the algoritm
    return path


def main():
    G = nx.Graph()
    visitados=[]

    print("Elige el tamaÃ±o del grafo: ")
    tam = int(input())
    numero_nodos=initGrafo(G, tam)
    
    
    start_node, end_node = get_start_end_nodes(G)#we got the first drawing on this funcition, for the click
    
    # Find the shortest path using A*
    shortest_path = find_shortest_path(G, start_node, end_node)
    
    # Draw the final graph with the shortest path highlighted
    plt.clf() # Clear the previous graph
    pos = nx.get_node_attributes(G, 'pos') # get the grid positions of each node
    # here we print the normal graph giving a yellow look on the node we clicked on
    nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in G.nodes()])
    # once the clicks have given, we highlight the edges of the given path that the algorithm gave us
    nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=2)
    plt.show()

if __name__ == '__main__':
    main()
