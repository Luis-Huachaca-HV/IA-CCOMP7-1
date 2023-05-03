#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <map>
#include <stdlib.h>
#include <string>
#include<cstdlib>
#include <time.h>

using namespace std;



float promedio[100];
float mejores[100];

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
		srand(i);
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
    quickSort(poblacion, 0, individuos-1);
    mejores[0] = poblacion[0].distancia;
    promedio[0] = promedio[0] / individuos;

    for (int contador = 1; contador < 100; contador++)
    {
		srand(contador);
        num = 1 + rand() % (nciudades - 1);
            quickSort(poblacion, 0, individuos-1);
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
                poblacion[indice + (individuos / 2)] = poblacion[indice];
                poblacion[indice + 1 + (individuos / 2)] = poblacion[indice+1];

                combinar(poblacion[indice + (individuos / 2)], poblacion[indice + 1 + (individuos / 2)], num %nciudades-1);
                recalcular(poblacion[indice + (individuos / 2)], grupo);
                recalcular(poblacion[indice + 1 + (individuos / 2)], grupo);
            }
            
            
        }

        

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
	
	
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";


int main()
{
	srand(time(NULL));
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
	
	generar_individuos(grupo, grupo[0], 4, 15, 20); // A como valor de inicio

    cout << "Los promedios son: ";
    for (int indice = 0; indice < 100; indice++)
    {
        cout << promedio[indice] << " ";
    }
    cout << endl;
    cout << "Los mejores son: ";
    for (int indice = 0; indice < 100; indice++)
    {
        cout << mejores[indice] << " ";
    }
	
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
	
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	float *vertices = new float[300];	
	float *vertices2 = new float[300];		
	unsigned int *indices = new unsigned int[100];
	unsigned int *indices2 = new unsigned int[100];
	
	float i=-1.00f;
	float aumenta=0.02f;
	for(int x=0;x<100;x++)
	{
		
		vertices[(x*3)]=i;
		vertices[(x*3)+1]=-1+(promedio[x]/100*4);
		vertices[(x*3)+2]=0.0f;
		
		vertices2[x*3]=i;
		vertices2[(x*3)+1]=-1+(mejores[x]/100*4);
		vertices2[(x*3)+2]=0.0f;
		i=i+aumenta;
	}
	

    for(int x=0;x<100;x++)
	{
		indices[x]=x;
		indices2[x]=x;
	}



    unsigned int VBOs[2], VAOs[2], EBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(2, EBOs);
	
	
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 100 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
	glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 100* sizeof(unsigned int), indices2, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
   
   glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
	glPointSize(1.0f);
	glLineWidth(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]); 
		
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation,1.0f, 0.0f, 0.0f, 0.0f);
		glDrawElements(GL_LINE_STRIP,100, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(VAOs[1]);
		glUniform4f(vertexColorLocation,0.0f, 0.0f, 1.0f, 0.0f);
		glDrawElements(GL_LINE_STRIP,100 , GL_UNSIGNED_INT, 0);
		
		
		/*
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]); 
        glDrawElements(GL_TRIANGLES, ((numberOfMovements * 3)+1), GL_UNSIGNED_INT, 0);
		
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glLineWidth((GLfloat)4);
		glUniform4f(location, 0.0f, 0.5f, 0.5f, 1.0f);
		glDrawElements(GL_LINES, ((numberOfMovements * 2)+1), GL_UNSIGNED_INT, 0);  */ 

        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}