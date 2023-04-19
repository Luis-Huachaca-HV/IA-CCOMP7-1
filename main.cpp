#include <iostream>
#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

class Tablero {
public:
    int size = 8;

    void Draw(sf::RenderWindow& ventana) {
        sf::RectangleShape casilla;
        casilla.setSize(sf::Vector2f(75.f, 75.f));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                casilla.setPosition(sf::Vector2f(75 * i, 75 * j));
                if ((i + j) % 2 == 0) {
                    casilla.setFillColor(sf::Color::White);
                }
                else {
                    casilla.setFillColor(sf::Color(55, 204, 179, 255));
                }
                ventana.draw(casilla);
            }
        }
    }

    void Select(sf::RenderWindow& ventana, int x, int y) {
        sf::RectangleShape casilla;
        casilla.setSize(sf::Vector2f(75.f, 75.f));
        casilla.setFillColor(sf::Color::Magenta);
        casilla.setPosition(sf::Vector2f(75 * x, 75 * y));
        ventana.draw(casilla);
    }
};

class Dama {
public:
    float r = 30;
    int x;
    int y;
    bool isAlive;
    bool isCrowned = false;
    sf::Color color;

    void Draw(sf::RenderWindow& ventana) {
        if (isAlive) {
            sf::CircleShape shape(r);
            shape.setFillColor(color);
            if (isCrowned) {
                shape.setOutlineThickness(3.f);
                shape.setOutlineColor(sf::Color::Yellow);
            }
            shape.setPosition(sf::Vector2f(x * 75 + (75 - r * 2) / 2, y * 75 + (75 - 2 * r) / 2));
            ventana.draw(shape);
        }
    }
};

void Init(sf::RenderWindow& ventana, Dama* rojas, Dama* negras) {
    int m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)!(i % 2 & 1); j < 8; j += 2) {
            negras[m].isAlive = true;
            negras[m].x = j;
            negras[m].y = i;
            m++;
        }

    }
    m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)(i % 2 & 1); j < 8; j += 2) {
            rojas[m].isAlive = true;
            rojas[m].x = j;
            rojas[m].y = 7 - i;
            m++;
        }

    }
}

Dama* Find(int x, int y, Dama* rojas, Dama* negras) {
    for (int i = 0; i < 12; i++) {
        if (rojas[i].x == x && rojas[i].y == y) {
            if (rojas[i].isAlive) {
                return &rojas[i];
            }
        }
        if (negras[i].x == x && negras[i].y == y) {
            if (negras[i].isAlive) {
                return &negras[i];
            }
        }
    }
    return NULL;
}

void Kill(int x, int y, Dama* rojas, Dama* negras, int* turno) {
    Find(x, y, rojas, negras)->isAlive = false;
    return;
}

int Move(int x, int y, Dama* selected, Dama* rojas, Dama* negras, int* turno) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        if (selected->color == sf::Color::Red || selected->color == sf::Color::Black && selected->isCrowned) {
            if (x == selected->x - 1 && y == selected->y - 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 1 && y == selected->y - 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x - 2 && y == selected->y - 2) {
                if (!Find(x, y, rojas, rojas) && Find(x + 1, y + 1, rojas, negras) != NULL && Find(x + 1, y + 1, rojas, negras)->color != selected->color) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    Kill(x + 1, y + 1, rojas, negras, turno);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 2 && y == selected->y - 2) {
                if (!Find(x, y, rojas, negras) && Find(x - 1, y + 1, negras, negras) != NULL && Find(x - 1, y + 1, rojas, negras)->color != selected->color) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    Kill(x - 1, y + 1, rojas, negras, turno);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
        }
        if (selected->color == sf::Color::Black || selected->color == sf::Color::Red && selected->isCrowned) {
            if (x == selected->x - 1 && y == selected->y + 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x + 1 && y == selected->y + 1) {
                if (!Find(x, y, rojas, negras)) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    return 1;
                }
            }
            if (x == selected->x - 2 && y == selected->y + 2) {
                if (!Find(x, y, rojas, negras) && Find(x + 1, y - 1, rojas, negras) != NULL && Find(x + 1, y - 1, rojas, negras)->color != selected->color) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    Kill(x + 1, y - 1, rojas, negras, turno);
                    return 1;
                }
            }
            if (x == selected->x + 2 && y == selected->y + 2) {
                if (!Find(x, y, rojas, negras) && Find(x - 1, y - 1, rojas, negras) != NULL && Find(x - 1, y - 1, rojas, negras)->color != selected->color) {
                    *turno = ((*turno == 1) ? 2 : 1);
                    selected->x = x;
                    selected->y = y;
                    Kill(x - 1, y - 1, rojas, negras, turno);
                    return 1;
                }
            }
        }
    }

    return 0;
}

class Node
{
public:
    Node* enlaces[96];
    Tablero _tablero;
    Dama _Rojas[12];
    Dama _Negras[12];
    bool inicializado = 0;

    Node(Tablero tablero, Dama Rojas[12], Dama Negras[12]) //copiamos el tablero actual y las posiciones de las damas
    {
        _tablero = tablero;
        inicializado = 1;

        for (int x = 0; x < 12; x++)
        {
            _Rojas[x] = Rojas[x];
            _Negras[x] = Negras[x];
        }
        for (int x = 0; x < 48; x++)
        {
            enlaces[x] = 0;
        }
    }
    void print()
    {
        std::cout << "Las fichas rojas son: ";
        for (int x = 0; x < 12; x++)
        {
            if (_Rojas[x].isAlive == 0)
            {
                std::cout << _Rojas[x].x << _Rojas[x].y << " vive:" << _Rojas[x].isAlive << "--";
            }
            else
            {
                std::cout << _Rojas[x].x << _Rojas[x].y << "--";
            }

        }
        std::cout << std::endl;

        std::cout << "Las fichas negras son: ";
        for (int x = 0; x < 12; x++)
        {
            if (_Negras[x].isAlive == 0)
            {
                std::cout << _Negras[x].x << _Negras[x].y << " vive:" << _Negras[x].isAlive << "--";
            }
            else
            {
                std::cout << _Negras[x].x << _Negras[x].y << "--";
            }
        }
        std::cout << std::endl;

    }
};

class Arbol
{
public:
    Node* _root;
    Dama Rojas_finales[12];
    Dama Negras_finales[12];
    int _nivel = 4;
    int movimientos_x_n[4] = { -1,1,-2,2 };
    int movimientos_y_n[4] = { 1,1,2,2 };

    int movimientos_x_coronada[8] = { -1,1,-2,2,-1,1,-2,2 };
    int movimientos_y_coronada[8] = { 1,1,2,2,-1,-1,-2,-2 };

    int movimientos_x_r[4] = { -1,1,-2,2 };
    int movimientos_y_r[4] = { -1,-1,-2,-2 };



    int cambios = 0;

    Arbol(Tablero tablero, Dama Rojas[12], Dama Negras[12])
    {
        _root = new Node(tablero, Rojas, Negras); //inicializamos la raiz con el tablero y las damas actuales
    }
    int funcion(Node* Actual)
    {

        int rojas_vivas = 0;
        int negras_vivas = 0;
        for (int x = 0; x < 12; x++)
        {
            if (Actual->_Rojas[x].isAlive)
            {
                rojas_vivas++;
            }
            if (Actual->_Negras[x].isAlive)
            {
                negras_vivas++;
            }

        }
        return negras_vivas - rojas_vivas;
    }
    void print()
    {
        std::cout << "Las fichas rojas son: ";
        for (int x = 0; x < 12; x++)
        {
            if (_root->_Rojas[x].isAlive == 0)
            {
                std::cout << _root->_Rojas[x].x << _root->_Rojas[x].y << " vive:" << _root->_Rojas[x].isAlive << "--";
            }
            else
            {
                std::cout << _root->_Rojas[x].x << _root->_Rojas[x].y << "--";
            }

        }
        std::cout << std::endl;

        std::cout << "Las fichas negras son: ";
        for (int x = 0; x < 12; x++)
        {
            if (_root->_Negras[x].isAlive == 0)
            {
                std::cout << _root->_Negras[x].x << _root->_Negras[x].y << " vive:" << _root->_Negras[x].isAlive << "--";
            }
            else
            {
                std::cout << _root->_Negras[x].x << _root->_Negras[x].y << "--";
            }
        }
        std::cout << std::endl;

    }
    void simular_movimiento(int nivel, Node* Actual)
    {
        Dama aux_rojas[12];
        Dama aux_negro[12];
        for (int x = 0; x < 12; x++)
        {
            aux_rojas[x] = Actual->_Rojas[x];
            aux_negro[x] = Actual->_Negras[x];
        }
        //std::cout << "nivel: " << nivel<<std::endl;;
        //print();
        Dama* SelectedPiece;
        int turno = 0;
        int turno_ini = 0;
        int posx = 0;
        int posy = 0;
        int i = 0;
        //SelectedPiece=Find(0, 5, _root->_Rojas, _root->_Negras);
        //Move(1, 4, SelectedPiece, _root->_Rojas, _root->_Negras, &turno);
        if (nivel == _nivel)
        {
            //std::cout << "se retorna " << std::endl;
            return;
        }
        for (int x = 0; x < 12; x++)
        {
            if (nivel % 2 == 1) //si es el turno de las rojas asignamos la posicion a la ficha roja x
            {
                turno = 1;
                turno_ini = 1;
                posx = Actual->_Rojas[x].x;
                posy = Actual->_Rojas[x].y;
                SelectedPiece = Find(posx, posy, Actual->_Rojas, Actual->_Negras);
            }
            else //caso contrario lo hacemos con las negras
            {
                turno = 2;
                turno_ini = 2;
                posx = Actual->_Negras[x].x;
                posy = Actual->_Negras[x].y;
                SelectedPiece = Find(posx, posy, Actual->_Rojas, Actual->_Negras);
            }
            if (SelectedPiece != NULL) //si hemos encontrado una ficha en esa posicion (si no esta muerta)
            {
                if (SelectedPiece->isCrowned)
                {
                    for (int y = 0; y < 8; y++)
                    {
                        if (turno_ini == 2) //vemos si existe un movimento posible como ficha negra y si existe lo hacemos y luego reseteamos los valores
                        {
                            if (Move(posx + movimientos_x_coronada[y], posy + movimientos_y_coronada[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_n[y] << " " << posy + movimientos_y_n[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                        else if (turno_ini == 1) //lo mismo para las rojas
                        {
                            if (Move(posx + movimientos_x_coronada[y], posy + movimientos_y_coronada[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_r[y] << " " << posy + movimientos_y_r[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int y = 0; y < 4; y++)
                    {
                        if (turno_ini == 2) //vemos si existe un movimento posible como ficha negra y si existe lo hacemos y luego reseteamos los valores
                        {
                            if (Move(posx + movimientos_x_n[y], posy + movimientos_y_n[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_n[y] << " " << posy + movimientos_y_n[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }
                        else if (turno_ini == 1) //lo mismo para las rojas
                        {
                            if (Move(posx + movimientos_x_r[y], posy + movimientos_y_r[y], SelectedPiece, Actual->_Rojas, Actual->_Negras, &turno) == 1)
                            {
                                cambios++;
                                nivel++;
                                //std::cout << "Nivel antes del cambio: " << nivel << std::endl;
                                //std::cout << "cambio nro " << cambios << " cambio desde: " << posx << " " << posy << " hasta " << posx + movimientos_x_r[y] << " " << posy + movimientos_y_r[y] << " en el turno " << turno_ini << std::endl;
                                Actual->enlaces[i] = new Node(Actual->_tablero, Actual->_Rojas, Actual->_Negras);
                                simular_movimiento(nivel, Actual->enlaces[i]);
                                i++;
                                nivel--;
                                for (int x = 0; x < 12; x++)
                                {
                                    Actual->_Rojas[x] = aux_rojas[x];
                                    Actual->_Negras[x] = aux_negro[x];
                                }
                            }
                        }


                    }
                }

            }


        }
    }
    int min_max(int nivel, Node* Actual, int alfa, int beta)
    {
        //int alfa = -100000;
        //int beta = 1000000;
        //si alfa es mayor o igual que beta 
        if (Actual)
        {
            if (nivel == _nivel - 1)
            {
                if (Actual->inicializado)
                {
                    return funcion(Actual);
                }
                else
                {
                    if (nivel % 2 == 1)
                    {
                        return 0;
                    }
                    else
                    {
                        return 10000;
                    }
                }

            }
            if (nivel == 1)
            {
                int maximo = 0;
                int minimo = 10000;
                int aux;
                for (int x = 0; x < 48; x++)
                {
                    if (alfa >= beta)
                    {
                        break;
                    }
                    nivel++;
                    aux = min_max(nivel, Actual->enlaces[x], alfa, beta);
                    nivel--;
                    if (nivel % 2 == 1)
                    {
                        if (aux < minimo)
                        {
                            for (int x = 0; x < 12; x++)
                            {
                                Rojas_finales[x] = Actual->_Rojas[x];
                                Negras_finales[x] = Actual->_Negras[x];
                            }
                            minimo = aux;
                            beta = minimo;
                        }
                    }
                    else
                    {
                        if (aux > maximo)
                        {
                            for (int x = 0; x < 12; x++)
                            {
                                Rojas_finales[x] = Actual->enlaces[x]->_Rojas[x];
                                Negras_finales[x] = Actual->enlaces[x]->_Negras[x];
                            }
                            maximo = aux;
                            alfa = maximo;
                        }
                    }

                }
                if (nivel % 2 == 1)
                {
                    return minimo;
                }
                else
                {
                    return maximo;
                }
            }
            else
            {
                int maximo = 0;
                int minimo = 10000;
                int aux;

                for (int x = 0; x < 48; x++)
                {
                    if (alfa >= beta)
                    {
                        break;
                    }
                    nivel++;
                    aux = min_max(nivel, Actual->enlaces[x], alfa, beta);
                    nivel--;
                    if (nivel % 2 == 1)
                    {
                        if (aux < minimo)
                        {
                            minimo = aux;
                            beta = minimo;
                        }
                    }
                    else
                    {
                        if (aux > maximo)
                        {
                            maximo = aux;
                            alfa = maximo;
                        }
                    }

                }
                if (nivel % 2 == 1)
                {
                    return minimo;
                }
                else
                {
                    return maximo;
                }
            }

        }

    }

};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16.0;
    sf::RenderWindow ventana(sf::VideoMode(600, 600), "Checkers", sf::Style::Default, settings);
    sf::Event event;
    Tablero tablero;
    Dama Rojas[12];
    Dama Negras[12];
    bool selected = false;
    Dama* SelectedPiece = NULL;
    int turno = 1;
    bool click = false;
    sf::RectangleShape fondo;
    fondo.setSize(sf::Vector2f(250, 200));
    fondo.setFillColor(sf::Color::White);
    fondo.setPosition(sf::Vector2f(0, 0));
    sf::RectangleShape rojo;
    rojo.setSize(sf::Vector2f(125, 200));
    rojo.setFillColor(sf::Color::Red);
    rojo.setPosition(sf::Vector2f(0, 0));
    sf::RectangleShape negro;
    negro.setSize(sf::Vector2f(125, 200));
    negro.setFillColor(sf::Color::Black);
    negro.setPosition(sf::Vector2f(125, 0));



    sf::RenderWindow turn_select(sf::VideoMode(250, 200), "Turn Selection", sf::Style::Default, settings);
    while (turn_select.isOpen()) {
        while (turn_select.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                turn_select.close();
            }
        }
        turn_select.clear();
        turn_select.draw(fondo);
        turn_select.draw(rojo);
        turn_select.draw(negro);
        if (event.mouseButton.button == sf::Mouse::Left) {
            int x = sf::Mouse::getPosition(turn_select).x;
            click = true;
            if (click) {
                if (x < 125) {
                    turno = 1;
                    turn_select.close();

                }
                else {
                    turno = 2;
                    turn_select.close();
                }
            }
        }
        turn_select.display();
    }

    for (int i = 0; i < 12; i++) {
        Negras[i].color = sf::Color::Black;
        Rojas[i].color = sf::Color::Red;
    }


    Init(ventana, Rojas, Negras);


    //Arbol Minmax(tablero, Rojas, Negras);
    //Minmax.simular_movimiento(0,Minmax._root);
    //Minmax._root->enlaces[0]->print();
    //std::cout << std::endl;
    //Minmax._root->enlaces[0]->enlaces[0]->print();
    //std::cout << std::endl;
    //Minmax._root->enlaces[0]->enlaces[0]->enlaces[0]->print();
    //std::cout << std::endl;
    //Minmax._root->enlaces[0]->enlaces[0]->enlaces[1]->print();
    //std::cout << std::endl;
    //Minmax._root->enlaces[0]->enlaces[1]->print();
    //std::cout << std::endl;
    //Minmax._root->enlaces[1]->print();
    //std::cout << Minmax.min_max(0, Minmax._root) << std::endl;

    while (ventana.isOpen()) {
        /*
        for (int x = 0; x < 12; x++)
        {
            Rojas[x] = Minmax.Rojas_finales[x];
            Negras[x] = Minmax.Negras_finales[x];
        }
        */

        int ndead = 0, rdead = 0;

        for (int i = 0; i < 12; i++) {
            if (!(Negras[i].isAlive)) {
                ndead++;
            }
        }
        for (int i = 0; i < 12; i++) {
            if (!(Rojas[i].isAlive)) {
                rdead++;
            }
        }
        if (rdead == 12) {
            std::cout << "La IA ha ganado";
            ventana.close();
        }
        if (ndead == 12) {
            std::cout << "El jugador ha ganado";
            ventana.close();
        }

        if (turno == 1)
        {
            while (ventana.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    ventana.close();
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        selected = !selected;
                    }
                }
            }

            ventana.clear();

            tablero.Draw(ventana);

            if (SelectedPiece != NULL) {
                tablero.Select(ventana, SelectedPiece->x, SelectedPiece->y);
            }

            for (int i = 0; i < 12; i++) {
                Negras[i].Draw(ventana);
                Rojas[i].Draw(ventana);
            }

            if (selected) {
                int x = sf::Mouse::getPosition(ventana).x / 75;
                int y = sf::Mouse::getPosition(ventana).y / 75;
                if (Find(x, y, Rojas, Negras) && (Find(x, y, Rojas, Negras)->color == sf::Color::Red && turno == 1 || Find(x, y, Rojas, Negras)->color == sf::Color::Black && turno == 2)) {//si seleccionas una ficha de tu color en tu turno
                    if (Find(x, y, Rojas, Negras) == SelectedPiece) {
                        SelectedPiece = NULL;
                    }
                    else {
                        SelectedPiece = Find(x, y, Rojas, Negras);
                    }

                    selected = false;
                }
                else if (SelectedPiece != NULL && Move(x, y, SelectedPiece, Rojas, Negras, &turno)) {
                    selected = false;
                    SelectedPiece = NULL;
                }
                selected = false;
            }
            for (int i = 0; i < 12; i++) {
                if (Rojas[i].y == 0) {
                    Rojas[i].isCrowned = true;
                }
                if (Negras[i].y == 7) {
                    Negras[i].isCrowned = true;
                }
            }


            ventana.display();
        }
        else
        {
            Arbol Minmax(tablero, Rojas, Negras);
            Minmax.simular_movimiento(0, Minmax._root);
            //Minmax.min_max(0, Minmax._root);
            Minmax.min_max(0, Minmax._root, -1000000, 1000000);
            for (int x = 0; x < 12; x++)
            {
                Rojas[x] = Minmax.Rojas_finales[x];
                Negras[x] = Minmax.Negras_finales[x];
            }
            turno = 1;

        }
    }
    return 0;
}