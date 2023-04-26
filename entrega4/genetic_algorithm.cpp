#include <cmath>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include<cstdlib>
#include <time.h>
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

using namespace std;

const int nciudades = 16;

class ciudad {
public:
    char nombre;
    int x, y;

    ciudad(char n, int u, int v) {
        nombre = n;
        x = u;
        y = v;
    }

    ciudad() {
        nombre = ' ';
        x = 0;
        y = 0;
    }

    void set(char n, int u, int v) {
        this->nombre = n;
        this->x = u;
        this->y = v;
    }
};

class individuo {
public:
    string ruta;
    int distancia;

    individuo(string nombre, int aptitud) {
        ruta = nombre;
        distancia = aptitud;
    }
    individuo() {
        ruta = "";
        distancia = 0;
    }

    void asignar(string nombre, int aptitud) {
        this->ruta = nombre;
        this->distancia = aptitud;
    }

    void valores() { cout << ruta << " con distancia de " << distancia << endl; }

    void nueva_ruta(string aux) { this->ruta = aux; }
    void nueva_dist(int aux) { this->distancia = aux; }
};



int partition(individuo poblacion[], int start, int end)
{

    individuo pivot = poblacion[start];

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (poblacion[i].distancia <= pivot.distancia)
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(poblacion[pivotIndex], poblacion[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (poblacion[i].distancia <= pivot.distancia) {
            i++;
        }

        while (poblacion[j].distancia > pivot.distancia) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(poblacion[i++], poblacion[j--]);
        }
    }

    return pivotIndex;
}

void quickSort(individuo poblacion[], int start, int end)
{

    // base case
    if (start >= end)
        return;

    // partitioning the array
    int p = partition(poblacion, start, end);

    // Sorting the left part
    quickSort(poblacion, start, p - 1);

    // Sorting the right part
    quickSort(poblacion, p + 1, end);
}


int euclidiana(ciudad a, ciudad b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

int porcentaje(int total, int porcentaje) { return (total * porcentaje) / 100; }

void swap(string& a, string& b, int index) {
    char aux = a[index];
    a[index] = b[index];
    b[index] = aux;
}

void cambio(string& a, string b, int index) { a[index] = b[index]; }

void llenar(string& a, int tam) {
    for (int i = 0; i < tam; i++)
        a = a + "_";
}

void combinar(individuo& a, individuo& b, int cambios) {
    srand(time(NULL));
    int tam = a.ruta.length();
    string ar;
    llenar(ar, tam);
    string br;
    llenar(br, tam);
    cambio(ar, a.ruta, 0);
    cambio(ar, a.ruta, tam - 1);
    cambio(br, b.ruta, 0);
    cambio(br, b.ruta, tam - 1);
    int r;
    for (int i = 0; i < cambios; i++) {
        r = rand() % (tam - 1) + 1;
        cambio(ar, b.ruta, r);
        cambio(br, a.ruta, r);
    }

    for (int i = 0; i < tam; i++) {
        if (ar[i] == '_') {
            for (int j = 0; j < tam; j++) {
                if (ar.find(a.ruta[j]) == string::npos) {
                    ar[i] = a.ruta[j];
                }
            }
        }
    }

    for (int i = 0; i < tam; i++) {
        if (br[i] == '_') {
            for (int j = 0; j < tam; j++) {
                if (br.find(b.ruta[j]) == string::npos) {
                    br[i] = b.ruta[j];
                }
            }
        }
    }

    a.nueva_ruta(ar);
    b.nueva_ruta(br);
}

ciudad buscar(char a, ciudad grupo[nciudades]) {
    for (int i = 0; i < nciudades; i++) {
        if (grupo[i].nombre == a) {
            return grupo[i];
        }
    }
    ciudad aux;
    return aux;
}

void recalcular(individuo& a, ciudad grupo[nciudades]) {
    int tam = a.ruta.length();
    int distancia = 0;
    for (int i = 1; i < tam; i++) {
        distancia +=
            euclidiana(buscar(a.ruta[i - 1], grupo), buscar(a.ruta[i], grupo));
    }
    a.nueva_dist(distancia);
}

void generar_individuos(ciudad grupo[nciudades], ciudad ciudad_inicio, int individuos, int cant_ci, int mutacion)
{
    int promedio[100];
    int mejores[100];
    srand(time(NULL));
    int num;

    int x = 1;
    ciudad* individuo_aux;
    individuo_aux = new ciudad[cant_ci + 2]; // Ciudades del medio+inicio+fin
    individuo* poblacion;
    poblacion = new individuo[individuos];
    int flags[nciudades];
    int distancia = 0;
    string ruta = "";
    for (int i = 0; i < 100; i++)
    {
        promedio[i] = 0;
        mejores[i] = 0;
    }

    // flags para saber que ciudades usamos y no repetir
    for (int i = 0; i < nciudades; i++)
    {
        flags[i] = 0;
    }

    // la primera y ultima posicion son la ciudad escogida (ciudad_inicio)
    individuo_aux[0] = ciudad_inicio;
    individuo_aux[cant_ci + 1] = ciudad_inicio;
    x = 1;
    distancia = 0;
    ruta = "";

    // sacamos 4 individuos
    for (int i = 0; i < individuos; i++)
    {
        // buscamos las 4 ciudades por las que va a pasar
        while (x < nciudades)
        {
            num = 1 + rand() % (nciudades - 1);
            //cout << num << endl;
            if (grupo[num].nombre != ciudad_inicio.nombre && flags[num] != 1)
            {
                flags[num] = 1;
                individuo_aux[x] = grupo[num];
                x++;
            }
        }
        // imprimimos el camino
        for (int ii = 0; ii < cant_ci + 2; ii++)
        {
            //cout << individuo_aux[ii].nombre << " ";
            ruta = ruta + individuo_aux[ii].nombre;
        }
        //cout << endl;

        // sacamos la distancia
        for (int ii = 1; ii < cant_ci + 2; ii++)
        {

            distancia = distancia + euclidiana(individuo_aux[ii - 1], individuo_aux[ii]);
        }
        //cout << distancia << endl;

        poblacion[i].asignar(ruta, distancia);

        // reiniciamos todo paa sacar otro individuo
        for (int ii = 0; ii < nciudades; ii++)
        {
            flags[ii] = 0;
        }

        individuo_aux[0] = ciudad_inicio;
        individuo_aux[cant_ci + 1] = ciudad_inicio;
        x = 1;
        distancia = 0;
        ruta = "";
    }
    for (int indice = 0; indice < individuos; indice++)
    {
        //poblacion[indice].valores();
        promedio[0] = promedio[0] + poblacion[indice].distancia;
    }
    //cout << endl;
    mejores[0] = poblacion[0].distancia;
    promedio[0] = promedio[0] / individuos;

    for (int contador = 1; contador < 100; contador++)
    {
        quickSort(poblacion, 0, individuos - 1);
        for (int indice = 0; indice < individuos; indice++)
        {
            //poblacion[indice].valores();
            promedio[contador] = promedio[contador] + poblacion[indice].distancia;
        }
        //cout << endl;
        mejores[contador] = poblacion[0].distancia;
        promedio[contador] = promedio[contador] / individuos;
        for (int indice = 0; indice < individuos / 2; indice = indice + 2)
        {
            poblacion[indice + (individuos / 2)] = poblacion[0];
            poblacion[indice + 1 + (individuos / 2)] = poblacion[1];

            combinar(poblacion[indice + (individuos / 2)], poblacion[indice + 1 + (individuos / 2)], contador % nciudades - 1);
            recalcular(poblacion[indice + (individuos / 2)], grupo);
            recalcular(poblacion[indice + 1 + (individuos / 2)], grupo);
        }


    }


    cout << "Los promedios son: ";
    for (int indice = 0; indice < 100; indice++)
    {
        cout << promedio[indice] << ", ";
    }
    cout << endl;
    cout << "Los mejores son: ";
    for (int indice = 0; indice < 100; indice++)
    {
        cout << mejores[indice] << ", ";
    }
    initwindow(600, 600, "Graficos");
    make(promedio, mejores, 45);
    getch();
    closegraph();
}
void texto(int x, int y, int A) { //pone los puntos en las coordenadas y A es el valor que se quiere poner
    ostringstream str1;
    str1 << A;
    string str2 = str1.str();
    char* str = &str2[0u];
    //settextstyle(DEFAULT_FONT, 0, 3);
    outtextxy(x, y, str);

} //

void putText(int A[], int top) {
    texto(90, 500, 0);
    texto(80, 400, (top / 4));   //esto hace poner el texto en el plano
    texto(80, 300, (top / 2));
    texto(80, 200, (top / 4) * 3);
    texto(80, 100, top);
    for (int i = 0; i < 8; i++)
        texto(((400 / 8) * i) + 100, 500, (100 / 8) * i);
    texto(500, 500, 100);
}

void plano2() { //grafica el plano inicial
    int xycero[2] = { 100,500 };
    int xytop[2] = { 100,100 };
    int xyfin[2] = { 500,500 };
    line(xycero[0], xycero[1], xytop[0], xytop[1]);
    line(xycero[0], xycero[1], xyfin[0], xyfin[1]);
}

void puntos2(int A[], int i, int x, float div)
{
    line(x + (i * 4), 500 - (A[i] * div), x + ((i + 1) * 4), 500 - (A[i + 1] * div));
    //cout << x + (i * 4) << "!";
}

void graficar(int A[], int aux) {
    float div = 400 / aux;
    int x = 100;
    for (int i = 0; i < 99; i++) {
        puntos2(A, i, x, div);
    }
    putText(A, aux);
}
void make(int A[], int B[], int aux) {
    plano2();
    graficar(A, 43);
    graficar(B, 43);
}


int main() {
    // generamos las posiciones de los puntos
    ciudad grupo[nciudades];

    grupo[0].set('A', 1, 2);
    grupo[1].set('B', 2, 3);
    grupo[2].set('C', 3, 4);
    grupo[3].set('D', 5, 3);
    grupo[4].set('E', 4, 1);
    grupo[5].set('F', 6, 3);
    grupo[6].set('G', 5, 4);
    grupo[7].set('H', 2, 5);
    grupo[8].set('I', 3, 3);
    grupo[9].set('J', 5, 5);
    grupo[10].set('K', 2, 1);
    grupo[11].set('L', 1, 1);
    grupo[12].set('M', 4, 2);
    grupo[13].set('N', 4, 3);
    grupo[14].set('O', 3, 2);
    grupo[15].set('P', 3, 1);

    // generamos las ciudades
    
    generar_individuos(grupo, grupo[0], 4, 15, 20); // A como valor de inicio
}