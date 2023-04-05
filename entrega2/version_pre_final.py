from ctypes import sizeof
from os import remove
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os
import tkinter as tk
from tkinter import simpledialog



            

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


def get_del_nodes(graph,path_star_node,path_end_node):
    start_node = None
    end_node = None
    while start_node is None or end_node is None:
        # Draw the graph with the current start and end nodes highlighted

        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(graph, 'pos')
        nx.draw_networkx(graph, pos, node_color=['yellow' if node == path_star_node else 'green' if node == path_end_node else 'red' if node == start_node else 'red' if node == end_node else 'white' for node in graph.nodes()  ])
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

def mejor_primero(G,inicio,fin):
    visitados = []
    camino = []
    actual=inicio
    hijos=0
    visitados.append(actual)
    camino.append(actual)
    pos=nx.get_node_attributes(G, 'pos')
    
    
    while actual!=fin and len(camino)>0:
        min=100000000000000000
        for nodos in G[actual]:
            if nodos not in visitados:
                hijos=hijos+1
                aux=math.pow(pos[fin][0]-pos[nodos][0],2)+math.pow(pos[fin][1]-pos[nodos][1],2)
                if (min>aux):
                    min=aux
                    indice=nodos
        if hijos==0:
            camino.pop(len(camino)-1)
            if(len(camino)>0):
                actual=camino[len(camino)-1]
            indice=actual
            min=math.pow(pos[fin][0]-pos[inicio][0],2)+math.pow(pos[fin][1]-pos[inicio][1],2)
        
        else:
            hijos=0
            if indice not in visitados:
                actual=indice
                camino.append(actual)
                visitados.append(actual)
                min=math.pow(pos[fin][0]-pos[inicio][0],2)+math.pow(pos[fin][1]-pos[inicio][1],2)
    return camino
    #print(camino)



def find_shortest_path_euc(graph, start, end):
    # Set the weight attribute for all nodes in the graph
    nx.set_node_attributes(graph, 0, 'weight')
    
    # Initialize the open and closed sets
    open_set = set()
    closed_set = set()
    
    # Add the start node to the open set
    open_set.add(start)
    
    # Create a dictionary to keep track of the parent node for each node in the path
    parents = {}
    
    # Set the weight of the start node to 0
    graph.nodes[start]['weight'] = 0
    
    # While the open set is not empty
    while len(open_set) > 0:
        # Find the node in the open set with the lowest f-score
        current = None
        for node in open_set:
            if current is None or graph.nodes[node]['weight'] + graph.nodes[node]['h'] < graph.nodes[current]['weight'] + graph.nodes[current]['h']:
                current = node

        # If we've reached the end node, we're done
        if current == end:
            path = []
            while current in parents:
                path.append(current)
                current = parents[current]
            path.append(start)
            path.reverse()
            return path
        
        # Move the current node from the open set to the closed set
        open_set.remove(current)
        closed_set.add(current)
        for neighbor in graph.neighbors(current):
            # If the neighbor is in the closed set, skip it
            if neighbor in closed_set:
                continue

            # Compute the tentative g-score for the neighbor
            tentative_g_score = graph.nodes[current]['weight'] + graph[current][neighbor]['peso']
            
            # If the neighbor is not in the open set, add it
            if neighbor not in open_set:
                open_set.add(neighbor)
                parents[neighbor] = current
                graph.nodes[neighbor]['weight'] = tentative_g_score
                graph.nodes[neighbor]['h'] = math.sqrt((graph.nodes[neighbor]['pos'][0]-graph.nodes[end]['pos'][0])**2+(graph.nodes[neighbor]['pos'][1]-graph.nodes[end]['pos'][1])**2)
            # If the neighbor is already in the open set, update its g-score if this path is better
            elif tentative_g_score >= graph.nodes[neighbor]['weight']:
                continue
            parents[neighbor] = current
            graph.nodes[neighbor]['weight'] = tentative_g_score
        # Add the following code to prompt when there is no path found
        if current != end and len(open_set) == 0:
            print("No path found between start and end nodes.")
            return []


def delNodes(G, del_node1,del_node2,ini,fin):
    pos=nx.get_node_attributes(G, 'pos')
    if(del_node1<del_node2):
        min=del_node1
        max=del_node2
    else:
        max=del_node1
        min=del_node2
    eliminados=[]
    nodo_eliminado=-1
    for i in range(min,max):
        
        if i != ini and i != fin and eliminados.count(i)==0 and i in G:
            if pos[i][1]>pos[max][1] or pos[i][1]<pos[min][1]:
                continue
            else:
                G.remove_node(i)
                eliminados.append(nodo_eliminado)
        else:
            continue
    return len(eliminados)


def main():
    G = nx.Graph()
    visitados=[]

    # Set up tkinter window for user input
    root = tk.Tk()
    root.withdraw()
    tam = simpledialog.askinteger("Graph Size", "Enter the graph size:")
    numero_nodos = initGrafo(G, tam)

    while True:
        # Prompt user for graph size

        # Prompt user for start and end nodes
        start_node, end_node = get_start_end_nodes(G)

        # Prompt user for nodes to delete
        del_node1, del_node2 = get_del_nodes(G, start_node, end_node)
        delNodes(G, del_node1, del_node2, start_node, end_node)

        # Prompt user for shortest path algorithm choice
        algorithm_choice = simpledialog.askstring("Algorithm Choice", "Which algorithm would you like to use for finding the shortest path? (best_first_search, a_star or finish)")

        # Find the shortest path using chosen algorithm
        if algorithm_choice.lower() == "best_first_search":
            shortest_path = mejor_primero(G, start_node, end_node)
        elif algorithm_choice.lower() == "a_star":
            shortest_path = find_shortest_path_euc(G, start_node, end_node)
        elif algorithm_choice.lower() == "finish":
            break
        else:
            print("Invalid algorithm choice")

        # Draw the final graph with the shortest path highlighted
        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(G, 'pos') # get the grid positions of each node
        nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in G.nodes()])
        nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=5)
        plt.show()


if __name__ == '__main__':
    main()