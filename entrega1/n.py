from ctypes import sizeof
from os import remove
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os
from grave import plot_network
from grave.style import use_attributes


sys.setrecursionlimit(5000)

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
                    G.add_edge(contador,contador-num,peso=math.sqrt(2)/2)
                    G.add_edge(contador,contador-1,peso=1)
                    contador=contador+1
    return 









#Best First Search
def Best_First_Search(start, target, graph, res, queue = [], visited = []):
    if start not in visited:
        res.append(start)
        visited.append(start)

    queue = queue + [x for x in graph[start].items() if x[0] not in visited]
    queue.sort(key=lambda x:x[1]['peso'])

    if queue[0][0] == target:
        res.append(queue[0][0])
        # print("entro fin")
    else:
        pricessing = queue[0]
        queue.remove(pricessing)
        # print("entro else")
        Best_First_Search(pricessing[0], target, graph, res, queue, visited)

#DFS
def dfs(start, target, graph):
        
    path = [[start]]
    steps =0
    while path:
        steps = steps + 1
        index = -1
        s_path = path.pop(index)
        l_node = s_path[-1] # the last node is our target, it's done

        if l_node == target:
            print("Cantidad de Pasos -> {}" .format(steps))
            return s_path
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]  
                    path.append(new_path)               
    print('El camino no existe' %(start, target))


#BFS
def bfs1(start, target, graph):
    
    path = [[start]]
    steps = 0
    while path:
        steps = steps + 1
        index = 0
        s_path = path.pop(index)
        l_node = s_path[-1] # the last node is our target, it's done

        if l_node == target:
            print("Cantidad de Pasos -> {}" .format(steps))
            return s_path
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]  
                    path.append(new_path)
    
    print("El camino no existe {} {}" .format(start, target))                
    
def hilighter(event):
    
    # if we did not hit a node, bail
    if not hasattr(event, 'nodes') or not event.nodes:
        return

    # pull out the graph,
            
    #este captura el evento en el 'grafo'
    graph = event.artist.graph
    
    # clear any non-default color on nodes
    for node, attributes in graph.nodes.data():
        attributes.pop('color', None)

    for u, v, attributes in graph.edges.data():
        attributes.pop('width', None)

    for node in event.nodes:
        graph.nodes[node]['color'] = 'C3'
        print(graph.nodes[node])# print all items

        for edge_attribute in graph[node].values():
            edge_attribute['width'] = 3

    # update the screen
    event.artist.stale = True
    event.artist.figure.canvas.draw_idle()


def delNodes(G, porcentaje, tam, ini, fin):
    porcentajeInt = math.floor((tam*tam)*porcentaje/100)
    for i in random.sample(range(0,tam*tam),porcentajeInt):
        if(i != ini and i != fin):
            G.remove_node(i)


def onclick(event):
    print('%s click: button=%d, x=%d, y=%d, xdata=%f, ydata=%f' %
          ('double' if event.dblclick else 'single', event.button,
           event.x, event.y, event.xdata, event.ydata))
    
G = nx.Graph()
initGrafo(G, 3)
fig,ax = plt.subplots()
#can = nx.draw_networkx(G, nx.get_node_attributes(G, 'pos'), with_labels=True)
art = plot_network(G, ax=ax, node_style=use_attributes(), edge_style=use_attributes())
art.set_picker(10)

#can.set_picker(10)
#fig.canvas.mpl_connect('button_press_event', hilighter)

cid = fig.canvas.mpl_connect('pick_event', hilighter)
plt.show()
