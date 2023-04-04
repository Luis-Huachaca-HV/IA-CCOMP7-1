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
### manhattan heuristic
def find_shortest_path_man(graph, start, end):
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
        
        # Check all the neighbors of the current node
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
                graph.nodes[neighbor]['h'] = math.sqrt((graph.nodes[neighbor]['pos'][0] - graph.nodes[end]['pos'][0])**2 + (graph.nodes[neighbor]['pos'][1] - graph.nodes[end]['pos'][1])**2)
            # If the neighbor is already in the open set, update its g-score if this path is better
            elif tentative_g_score < graph.nodes[neighbor]['weight']:
                parents[neighbor] = current
                graph.nodes[neighbor]['weight'] = tentative_g_score
    
    # If we reach this point, there is no path from the start node to the end node
    return None
##euclidean heuristic
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
        
        # Check all the neighbors of the current node
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
                graph.nodes[neighbor]['h'] = math.sqrt((graph.nodes[neighbor]['pos'][0] - graph.nodes[end]['pos'][0])**2 + (graph.nodes[neighbor]['pos'][1] - graph.nodes[end]['pos'][1])**2)
            # If the neighbor is already in the open set, update its g-score if this path is better
            elif tentative_g_score < graph.nodes[neighbor]['weight']:
                parents[neighbor] = current
                graph.nodes[neighbor]['weight'] = tentative_g_score
        # Replace the Manhattan heuristic with Euclidean distance
        if 'h' in graph.nodes[neighbor]:
            graph.nodes[neighbor]['h'] = math.sqrt((graph.nodes[neighbor]['pos'][0] - graph.nodes[end]['pos'][0])**2 + (graph.nodes[neighbor]['pos'][1] - graph.nodes[end]['pos'][1])**2)
    
    # If we reach this point, there is no path from the start node to the end node
    return None

def main():
    G = nx.Graph()
    visitados=[]

    print("Elige el tamaÃ±o del grafo: ")
    tam = int(input())
    numero_nodos=initGrafo(G, tam)
    
    
    start_node, end_node = get_start_end_nodes(G)
    
    # Find the shortest path using A*
    shortest_path = find_shortest_path_man(G, start_node, end_node)
    
    # Draw the final graph with the shortest path highlighted
    plt.clf() # Clear the previous graph
    pos = nx.get_node_attributes(G, 'pos') # get the grid positions of each node
    nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'white' for node in G.nodes()])
    nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=2)
    plt.show()

if __name__ == '__main__':
    main()
