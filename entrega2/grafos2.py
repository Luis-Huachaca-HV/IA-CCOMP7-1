from ctypes import sizeof
from os import remove
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os


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
    del_node1 = None
    del_node2 = None
    while start_node is None or end_node is None or del_node1 is None or del_node2 is None:
        # Draw the graph with the current start and end nodes highlighted
        plt.clf() # Clear the previous graph
        pos = nx.get_node_attributes(graph, 'pos')
        nx.draw_networkx(graph, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'red' if node ==del_node1 else 'red' if node ==del_node2 else 'white' for node in graph.nodes()])
        plt.draw()
        plt.pause(0.001)
        
        # Get the next user input
        click_event = plt.ginput(n=1, timeout=0)
        node = None
        for n, (x, y) in pos.items():
            if abs(x - click_event[0][0]) < 0.2 and abs(y - click_event[0][1]) < 0.2:
                node = n
                break
                
        # Update the start or end node if the user clicked on a node
        if node is not None:
            if start_node is None:
                start_node = node
            elif end_node is None and node != start_node:
                end_node = node
            elif del_node1 is None and node != start_node and node !=end_node:
                del_node1 = node
            elif del_node2 is None and node != start_node and node !=end_node and node !=del_node1:
                del_node2 = node
    plt.clf() # Clear the previous graph
    pos = nx.get_node_attributes(graph, 'pos')
    nx.draw_networkx(graph, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'red' if node ==del_node1 else 'red' if node ==del_node2 else 'white' for node in graph.nodes()])
    plt.draw()
    plt.pause(0.001)
    return start_node, end_node, del_node1,del_node2

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

def dfs(G,inicio,fin,visitados,camino):
    visitados.append(inicio)
    camino.append(inicio)
    if(inicio==fin):
        return
    for node in G[inicio]:
        if node not in visitados:
            if camino[len(camino)-1]==fin:
                return
            dfs(G,node,fin,visitados,camino)
    if camino[len(camino)-1]==fin:
        return
    else:
        camino.pop(len(camino)-1)
        return 

def mejor_primero(G,inicio,fin,visitados,camino):
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
    print(camino)



            

def main():
    G = nx.Graph()


    print("Elige el tamaÃ±o del grafo: ")
    tam = int(input())
    numero_nodos=initGrafo(G, tam)
        
    texto="s"
    
    while (texto=="s"):
        start_node, end_node,del_node1,del_node2 = get_start_end_nodes(G)
        delNodes(G,del_node1,del_node2,start_node,end_node)
        # Find the shortest path using A*
        plt.clf()
        camino=[]
        
        pos = nx.get_node_attributes(G, 'pos')
        visitados=[]
        color_map = []
        
        mejor_primero(G,start_node,end_node,visitados,camino)
        
        for node in G:
            if(camino.count(node)!=0):
                color_map.append('red')
            else: 
                color_map.append('blue')

            nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'red' if node == del_node1 else 'red' if node == del_node2  else 'white' for node in G.nodes()])
            nx.draw_networkx_edges(G, pos, edgelist=[(camino[i], camino[i+1]) for i in range(len(camino)-1)], edge_color='r', width=2)
        #shortest_path = find_shortest_path_man(G, start_node, end_node)
        #plt.clf() # Clear the previous graph
        #pos = nx.get_node_attributes(G, 'pos') # get the grid positions of each node
        #nx.draw_networkx(G, pos, node_color=['yellow' if node == start_node else 'green' if node == end_node else 'red' if node == del_node1 else 'red' if node == del_node2  else 'white' for node in G.nodes()])
        #nx.draw_networkx_edges(G, pos, edgelist=[(shortest_path[i], shortest_path[i+1]) for i in range(len(shortest_path)-1)], edge_color='r', width=2)
        print("Desea seguir? ")
        texto=input()
        plt.close('all')



if __name__ == '__main__':
    main()