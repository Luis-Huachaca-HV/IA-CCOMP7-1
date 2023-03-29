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







#BFS 
def bfs(Grafo,inicio,fin,visitados):

    camino=[]
    camino.append(inicio)
    padre=camino.pop(0)
    visitados.append(padre)
    pasos=0

    while(padre!=fin):  
        for x in Grafo[padre]:
            if(visitados.count(x)==0 and camino.count(x)==0):
                camino.append(x)
        if(len(camino)==0 and padre!=fin):
            print("No hay solucion")
            return -1
        padre=camino.pop(0)
        pasos=pasos+1
        
        if(visitados.count(padre)==0):
            visitados.append(padre)
    print("La cantidad de pasos es: ",pasos)
    print("Los nodos que hemos visitado son: ",visitados)

#DFS 
def dfs(Grafo,inicio,fin,visitados):

    camino=[]
    camino.append(inicio)
    padre=camino.pop(0)
    visitados.append(padre)
    pasos=0

    while(padre!=fin):  
        for x in Grafo[padre]:
            if(visitados.count(x)==0 and camino.count(x)==0):
                camino.insert(0,x)
        if(len(camino)==0 and padre!=fin):
            print("No hay solucion")
            return -1
        padre=camino.pop(0)
        pasos=pasos+1
        
        if(visitados.count(padre)==0):
            visitados.append(padre)
    print("La cantidad de pasos es: ",pasos)
    print("Los nodos que hemos visitado son: ",visitados)


G = nx.Graph()
visitados=[]

print("Elige el tamaÃ±o del grafo: ")
tam = int(input())
numero_nodos=initGrafo(G, tam)

print("Elige el nodo inicio y fin entre  0 y ",numero_nodos-1)
inicio = int(input("inicio -> "))
fin = int(input("fin -> "))

print("Escriba el porcentaje de nodos a eliminar:")
porcentaje = int(input())
eliminados=delNodes(G,porcentaje,numero_nodos,inicio,fin)
print("Se han eliminado ",eliminados," nodos")
nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = "blue", with_labels=True,node_size=20,font_size=7)
plt.show()

print("Escoja una accion \n")
print("1. BFS \n")
print("2. DFS \n")

opcion = int(input("-> "))
if(opcion == 1):
    bfs(G,inicio,fin,visitados)
    color_map = []
    for node in G:
        if(visitados.count(node)!=0):
            color_map.append('red')
        else: 
            color_map.append('blue')


    nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = color_map, with_labels=True,node_size=20,font_size=7)
    plt.show()
if(opcion == 2):
    dfs(G,inicio,fin,visitados)

    color_map = []
    for node in G:
        if(visitados.count(node)!=0):
            color_map.append('red')
        else: 
            color_map.append('blue')

    nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = color_map, with_labels=True,node_size=20,font_size=7)
    plt.show()
