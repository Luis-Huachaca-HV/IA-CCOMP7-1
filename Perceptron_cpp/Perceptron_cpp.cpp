#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cmath>

using namespace std;

const double tasa_aprendizaje = 0.1;
const int iteraciones = 10000; //iteraciones de actualizaciones de pesos
const int tamanho_imagen = 30; //imagen de tamanho 5x6 
const int salida_binaria = 1;

int width = 0, height = 0;
const int cant_data_train = 10;

//clase neurona (una neurona para cada digito que queremos reconocer)
class Neurona {
public:
    int _bias;
    int _entrada[cant_data_train][(tamanho_imagen * salida_binaria) + 1]; //(x)
    float _matriz_pesos[tamanho_imagen * salida_binaria]; //(w)

    //inicializamos los valores
    Neurona(int entrada[cant_data_train][31])
    {
        //inicializamos la matriz de entradas
        _bias = 0;
        for (int y = 0; y < cant_data_train; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                _entrada[y][x] = entrada[y][x]; //entrada va a ser la matriz que queremos identificar 
            }
            _entrada[y][30] = entrada[y][30];
        }

        //inicializamos la matriz pesos
        for (int x = 0; x < 30; x++)
        {
            _matriz_pesos[x] = 0;
        }


    }

    //imprimimos la matriz pesos
    void print_pesos()
    {
        int contador = 0;
        for (int x = 0; x < 30; x++)
        {
            contador++;
            cout << _matriz_pesos[x] << "\t";
            if (contador == 5)
            {
                contador = 0;
                cout << endl;
            }
        }
        cout << endl;
    }


    //imprimimos la matriz de entrada
    void print_entrada()
    {
        int contador = 0;
        for (int y = 0; y < cant_data_train; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                contador++;
                cout << _entrada[y][x] << "\t";
                if (contador == 5)
                {
                    contador = 0;
                    cout << endl;
                }
            }
            cout << endl;
        }

    }

    double activacion(double valor)
    {
        return 1 / (1 + exp(-valor));
    }

    //funcion para calcular la sumatoria de pesos por la entrada
    int funcion_y(int input[30])
    {
        double suma = 0.0;

        //sumatoria entrada*peso
        for (int x = 0; x < 30; x++)
        {
            suma = suma + input[x] * _matriz_pesos[x];
        }

        //se agrega el bias
        suma = suma + _bias;

        //funcion(y) si es <=0 es 0 caso contrario 1
        return activacion(suma);
    }
    double loss(double prediction, int label)
    {
        return -(label * log(prediction) + (1 - label) * log(1 - prediction));
    }

    //funcion que va a comprobar si la entrada es valida cambiando los pesos
    //igual que el procedimiento manual de perceptron
    void funcion_perceptron()
    {
        for (int x = 0; x < iteraciones; x++)
        {
            for (int x = 0; x < cant_data_train; x++)
            {
                double prediccion = funcion_y(_entrada[x]);
                double cost = loss(prediccion, _entrada[x][30]);

                for (int i = 0; i < 30; ++i)
                {
                    _matriz_pesos[i] -= tasa_aprendizaje * _entrada[x][i] * (prediccion - _entrada[x][30]);
                }
                _bias -= tasa_aprendizaje * (prediccion - _entrada[x][30]);
            }
        }

        //print_entrada();
        //print_pesos();


    }

    int predict(int input[31])
    {
        double prediction = funcion_y(input);
        return (prediction >= 0.5) ? 1 : 0;
    }

};

int* ReadBMP(char* filename) {

    FILE* f = fopen(filename, "rb");
    if (f == NULL) cout << "Archivo no especificado";

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, f);
    width = *(int*)&header[18];
    height = *(int*)&header[22];

    int completar_fila = (width * 3 + 3) & (~3);
    unsigned char* data = new unsigned char[completar_fila];
    int* informacion = new int[width * height];


    unsigned char tmp;
    //cout<<"Tam: "<<width<<" "<<height<<endl;
    int count = width * height - 1;
    for (int i = 0; i < height; i++) {
        fread(data, sizeof(unsigned char), completar_fila, f);
        for (int j = 0; j < width * 3; j += 3) {// Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;
            if ((int)data[j] == 255) { informacion[count] = 0; }
            else { informacion[count] = 1; }
            //cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
          //cout<<informacion[count]<<endl;
            count--;
        }
    }
    //cout<<"info "<< count<<" esperado "<<width*height<<endl;

    fclose(f);
    return informacion;
}

void comparar(int* a, int* b) {
    for (int i = 0; i < 30; i++) {
        cout << a[i] << " = " << b[i] << endl;
    }
}

void llenar(int(&casos_prueba)[cant_data_train][31], int* prueba, int numero, int reconoce)
{
    for (int y = 0; y < 30; y++)
    {
        casos_prueba[numero - 1][y] = prueba[y];
    }
    casos_prueba[numero - 1][30] = reconoce;
}

int main()
{
    //entrada dibujando un 0
    char* filename = (char*)"cero1_imagen.bmp";
    int* prueba_cero1 = ReadBMP(filename);
    filename = (char*)"cero2_imagen.bmp";
    int* prueba_cero2 = ReadBMP(filename);
    filename = (char*)"cero3_imagen.bmp";
    int* prueba_cero3 = ReadBMP(filename);
    filename = (char*)"cero4_imagen.bmp";
    int* prueba_cero4 = ReadBMP(filename);
    filename = (char*)"cero5_imagen.bmp";
    int* prueba_cero5 = ReadBMP(filename);

    //entrada dibujando un 1
    filename = (char*)"uno1_imagen.bmp";
    int* prueba_uno1 = ReadBMP(filename);
    filename = (char*)"uno2_imagen.bmp";
    int* prueba_uno2 = ReadBMP(filename);
    filename = (char*)"uno3_imagen.bmp";
    int* prueba_uno3 = ReadBMP(filename);
    filename = (char*)"uno4_imagen.bmp";
    int* prueba_uno4 = ReadBMP(filename);
    filename = (char*)"uno5_imagen.bmp";
    int* prueba_uno5 = ReadBMP(filename);

    //entrada dibujando un 2
    filename = (char*)"dos1_imagen.bmp";
    int* prueba_dos1 = ReadBMP(filename);
    filename = (char*)"dos2_imagen.bmp";
    int* prueba_dos2 = ReadBMP(filename);
    filename = (char*)"dos3_imagen.bmp";
    int* prueba_dos3 = ReadBMP(filename);
    filename = (char*)"dos4_imagen.bmp";
    int* prueba_dos4 = ReadBMP(filename);
    filename = (char*)"dos5_imagen.bmp";
    int* prueba_dos5 = ReadBMP(filename);

    //entrada dibujando un 3
    filename = (char*)"tres1_imagen.bmp";
    int* prueba_tres1 = ReadBMP(filename);
    filename = (char*)"tres2_imagen.bmp";
    int* prueba_tres2 = ReadBMP(filename);
    filename = (char*)"tres3_imagen.bmp";
    int* prueba_tres3 = ReadBMP(filename);
    filename = (char*)"tres4_imagen.bmp";
    int* prueba_tres4 = ReadBMP(filename);
    filename = (char*)"tres5_imagen.bmp";
    int* prueba_tres5 = ReadBMP(filename);

    //entrada dibujando un 4
    filename = (char*)"cuatro1_imagen.bmp";
    int* prueba_cuatro1 = ReadBMP(filename);
    filename = (char*)"cuatro2_imagen.bmp";
    int* prueba_cuatro2 = ReadBMP(filename);
    filename = (char*)"cuatro3_imagen.bmp";
    int* prueba_cuatro3 = ReadBMP(filename);
    filename = (char*)"cuatro4_imagen.bmp";
    int* prueba_cuatro4 = ReadBMP(filename);
    filename = (char*)"cuatro5_imagen.bmp";
    int* prueba_cuatro5 = ReadBMP(filename);

    //entrada dibujando un 5
    filename = (char*)"cinco1_imagen.bmp";
    int* prueba_cinco1 = ReadBMP(filename);
    filename = (char*)"cinco2_imagen.bmp";
    int* prueba_cinco2 = ReadBMP(filename);
    filename = (char*)"cinco3_imagen.bmp";
    int* prueba_cinco3 = ReadBMP(filename);
    filename = (char*)"cinco4_imagen.bmp";
    int* prueba_cinco4 = ReadBMP(filename);
    filename = (char*)"cinco5_imagen.bmp";
    int* prueba_cinco5 = ReadBMP(filename);

    //entrada dibujando un 6
    filename = (char*)"seis1_imagen.bmp";
    int* prueba_seis1 = ReadBMP(filename);
    filename = (char*)"seis2_imagen.bmp";
    int* prueba_seis2 = ReadBMP(filename);
    filename = (char*)"seis3_imagen.bmp";
    int* prueba_seis3 = ReadBMP(filename);
    filename = (char*)"seis4_imagen.bmp";
    int* prueba_seis4 = ReadBMP(filename);
    filename = (char*)"seis5_imagen.bmp";
    int* prueba_seis5 = ReadBMP(filename);

    //entrada dibujando un 7
    filename = (char*)"siete1_imagen.bmp";
    int* prueba_siete1 = ReadBMP(filename);
    filename = (char*)"siete2_imagen.bmp";
    int* prueba_siete2 = ReadBMP(filename);
    filename = (char*)"siete3_imagen.bmp";
    int* prueba_siete3 = ReadBMP(filename);
    filename = (char*)"siete4_imagen.bmp";
    int* prueba_siete4 = ReadBMP(filename);
    filename = (char*)"siete5_imagen.bmp";
    int* prueba_siete5 = ReadBMP(filename);

    //entrada dibujando un 8
    filename = (char*)"ocho1_imagen.bmp";
    int* prueba_ocho1 = ReadBMP(filename);
    filename = (char*)"ocho2_imagen.bmp";
    int* prueba_ocho2 = ReadBMP(filename);
    filename = (char*)"ocho3_imagen.bmp";
    int* prueba_ocho3 = ReadBMP(filename);
    filename = (char*)"ocho4_imagen.bmp";
    int* prueba_ocho4 = ReadBMP(filename);
    filename = (char*)"ocho5_imagen.bmp";
    int* prueba_ocho5 = ReadBMP(filename);

    //entrada dibujando un 9
    filename = (char*)"nueve1_imagen.bmp";
    int* prueba_nueve1 = ReadBMP(filename);
    filename = (char*)"nueve2_imagen.bmp";
    int* prueba_nueve2 = ReadBMP(filename);
    filename = (char*)"nueve3_imagen.bmp";
    int* prueba_nueve3 = ReadBMP(filename);
    filename = (char*)"nueve4_imagen.bmp";
    int* prueba_nueve4 = ReadBMP(filename);
    filename = (char*)"nueve5_imagen.bmp";
    int* prueba_nueve5 = ReadBMP(filename);



    //datos de entrenamiento neurona 1
    int casos_prueba[cant_data_train][31];
    //ceros
    llenar(casos_prueba, prueba_cero1, 1, 0);
    llenar(casos_prueba, prueba_cero2, 2, 0);
    llenar(casos_prueba, prueba_cero3, 3, 0);
    llenar(casos_prueba, prueba_cero4, 4, 0);
    llenar(casos_prueba, prueba_cero5, 5, 0);
    //unos
    llenar(casos_prueba, prueba_uno1, 6, 1);
    llenar(casos_prueba, prueba_uno2, 7, 1);
    llenar(casos_prueba, prueba_uno3, 8, 1);
    llenar(casos_prueba, prueba_uno4, 9, 1);
    llenar(casos_prueba, prueba_uno5, 10, 1);


    Neurona test1(casos_prueba);
    test1.funcion_perceptron();

    int prediccion = test1.predict(prueba_cero4);
    cout << "se reconoce como: " << prediccion << endl;
    prediccion = test1.predict(prueba_uno2);
    cout << "se reconoce como: " << prediccion << endl;

    //datos de entrenamiento neurona 2
    int casos_prueba2[cant_data_train][31];
    //ceros
    llenar(casos_prueba2, prueba_dos1, 1, 0);
    llenar(casos_prueba2, prueba_dos2, 2, 0);
    llenar(casos_prueba2, prueba_dos3, 3, 0);
    llenar(casos_prueba2, prueba_dos4, 4, 0);
    llenar(casos_prueba2, prueba_dos5, 5, 0);
    //unos
    llenar(casos_prueba2, prueba_tres1, 6, 1);
    llenar(casos_prueba2, prueba_tres2, 7, 1);
    llenar(casos_prueba2, prueba_tres3, 8, 1);
    llenar(casos_prueba2, prueba_tres4, 9, 1);
    llenar(casos_prueba2, prueba_tres5, 10, 1);


    Neurona test2(casos_prueba2);
    test2.funcion_perceptron();

    prediccion = test2.predict(prueba_tres4);
    cout << "se reconoce como: " << prediccion + 2 << endl;
    prediccion = test2.predict(prueba_dos1);
    cout << "se reconoce como: " << prediccion + 2 << endl;

    //datos de entrenamiento neurona 3
    int casos_prueba3[cant_data_train][31];
    //ceros
    llenar(casos_prueba3, prueba_cuatro1, 1, 0);
    llenar(casos_prueba3, prueba_cuatro2, 2, 0);
    llenar(casos_prueba3, prueba_cuatro3, 3, 0);
    llenar(casos_prueba3, prueba_cuatro4, 4, 0);
    llenar(casos_prueba3, prueba_cuatro5, 5, 0);
    //unos
    llenar(casos_prueba3, prueba_cinco1, 6, 1);
    llenar(casos_prueba3, prueba_cinco2, 7, 1);
    llenar(casos_prueba3, prueba_cinco3, 8, 1);
    llenar(casos_prueba3, prueba_cinco4, 9, 1);
    llenar(casos_prueba3, prueba_cinco5, 10, 1);


    Neurona test3(casos_prueba3);
    test3.funcion_perceptron();

    prediccion = test3.predict(prueba_cinco3);
    cout << "se reconoce como: " << prediccion + 4 << endl;
    prediccion = test3.predict(prueba_cuatro4);
    cout << "se reconoce como: " << prediccion + 4 << endl;

    //datos de entrenamiento neurona 4
    int casos_prueba4[cant_data_train][31];
    //ceros
    llenar(casos_prueba4, prueba_seis1, 1, 0);
    llenar(casos_prueba4, prueba_seis2, 2, 0);
    llenar(casos_prueba4, prueba_seis3, 3, 0);
    llenar(casos_prueba4, prueba_seis4, 4, 0);
    llenar(casos_prueba4, prueba_seis5, 5, 0);
    //unos
    llenar(casos_prueba4, prueba_siete1, 6, 1);
    llenar(casos_prueba4, prueba_siete2, 7, 1);
    llenar(casos_prueba4, prueba_siete3, 8, 1);
    llenar(casos_prueba4, prueba_siete4, 9, 1);
    llenar(casos_prueba4, prueba_siete5, 10, 1);


    Neurona test4(casos_prueba4);
    test4.funcion_perceptron();

    prediccion = test4.predict(prueba_seis3);
    cout << "se reconoce como: " << prediccion + 6 << endl;
    prediccion = test4.predict(prueba_siete3);
    cout << "se reconoce como: " << prediccion + 6 << endl;

    //datos de entrenamiento neurona 5
    int casos_prueba5[cant_data_train][31];
    //ceros
    llenar(casos_prueba5, prueba_ocho1, 1, 0);
    llenar(casos_prueba5, prueba_ocho2, 2, 0);
    llenar(casos_prueba5, prueba_ocho3, 3, 0);
    llenar(casos_prueba5, prueba_ocho4, 4, 0);
    llenar(casos_prueba5, prueba_ocho5, 5, 0);
    //unos
    llenar(casos_prueba5, prueba_nueve1, 6, 1);
    llenar(casos_prueba5, prueba_nueve2, 7, 1);
    llenar(casos_prueba5, prueba_nueve3, 8, 1);
    llenar(casos_prueba5, prueba_nueve4, 9, 1);
    llenar(casos_prueba5, prueba_nueve5, 10, 1);


    Neurona test5(casos_prueba5);
    test5.funcion_perceptron();

    prediccion = test5.predict(prueba_nueve4);
    cout << "se reconoce como: " << prediccion + 8 << endl;
    prediccion = test5.predict(prueba_ocho2);
    cout << "se reconoce como: " << prediccion + 8 << endl;
}