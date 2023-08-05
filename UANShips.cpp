#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void menuPrincipal();
void iniciarPartida();
void menuEstadisticas();
void actualizarMapaVisible();
void pintarMapaVisible();
void pintarMapaInvisible();
void introducirCoordenadas();
void partidaGanada();
void eliminarMargenBarcoMapaInvisible();
void posicionarBarcos();
void matarBarcos();
void mostrarEstadoBarco();
void resetearJuego();
void actualizarBD();
void leerBD();
void pintarTiro(int fila, int columna, bool acertado);
void validarBarcoHundido(int numeroBarco);
void pintarBarcoInvisible(int filas, int columnas, int numeroBarco, bool horizontal);
void pintarMargen(int filas, int columnas, int posicionX, int posicionY, bool horizontal);

int numeroAleatorio(int max);
int asignarEjeX(int maximoX);
int asignarEjeY(int maximoY);

bool validacionPosicionBarco(int filas, int columnas, int posicionX, int posicionY, bool horizontal);

string validarEstadoBarco(int barco);

int mapaInvisible[20][20];
char mapaVisible[14][14];

int partidasGanadas, partidasCanceladas;
int top[5] = {};
int celdasAcertadas = 0, celdasErradas = 0;
int puntuacionPartida = 0;

string partidasGanadasBD, partidasCanceladasBD, top1BD, top2BD, top3BD, top4BD, top5BD;
string jugadorBD[] = {"None", "None", "None", "None", "None"};

int filasPortaAviones = 2, columnasPortaAviones = 5;
int filasAcorazado = 1, columnasAcorazado = 6;
int filasBuque = 1, columnasBuque = 5;
int filasDestructor = 1, columnasDestructor = 4;
int filasFragata = 1, columnasFragata = 4;
int filasLanchaRapida = 1, columnasLanchaRapida = 3;

int filasBarcos[6] = {0, 0, 0, 0, 0, 0};
int columnasBarcos[6] = {0, 0, 0, 0, 0, 0};

int tamanoBarcos[6][2] = {
    {filasPortaAviones, columnasPortaAviones},
    {filasAcorazado, columnasAcorazado},
    {filasBuque, columnasBuque},
    {filasDestructor, columnasDestructor},
    {filasFragata, columnasFragata},
    {filasLanchaRapida, columnasLanchaRapida}};

int portaAvionesAtacado = 0, acorazadoAtacado = 0, buqueAtacado = 0, destructorAtacado = 0, fragataAtacado = 0, lanchaRapidaAtacado = 0;

string codigoMostrarMapaInvisible = "codigo";
bool estadoCodigoMostrarMapaInvisible = false;
string codigoMatarBarcos = "ganar";
bool estadoCodigoMatarBarcos = false;

string opcionDef = "**Escoja una opción que esté dentro del menú**";

char opcionUsuario;
string codigoUsuario;

struct Puntajes
{
    string nombre;
    string puntaje;
};

int main()
{
    srand(time(NULL));
    std::system("clear");
    leerBD();
    menuPrincipal();
    return 0;
}

void leerBD()
{

    vector<vector<string>> contenidoBD;
    vector<string> fila;
    string linea, dato;

    ifstream baseDatos;
    baseDatos.open("baseDatos.csv", ios::in);

    if (baseDatos.fail())
    {
        actualizarBD();
    }

    if (baseDatos.is_open())
    {
        while (getline(baseDatos, linea))
        {
            fila.clear();

            stringstream lineaStringStream(linea);

            while (getline(lineaStringStream, dato, ','))
            {
                fila.push_back(dato);
            }

            contenidoBD.push_back(fila);
        }
    }
    else
    {
        menuPrincipal();
    }

    partidasGanadasBD = contenidoBD[0][1];
    partidasGanadas = stoi(partidasGanadasBD);

    partidasCanceladasBD = contenidoBD[1][1];
    partidasCanceladas = stoi(partidasCanceladasBD);

    Puntajes puntajes[5] = {top1BD, top2BD, top3BD, top4BD, top5BD};

    puntajes[0].puntaje = contenidoBD[2][1];
    top[0] = stoi(puntajes[0].puntaje);
    puntajes[1].puntaje = contenidoBD[2][2];
    top[1] = stoi(puntajes[1].puntaje);
    puntajes[2].puntaje = contenidoBD[2][3];
    top[2] = stoi(puntajes[2].puntaje);
    puntajes[3].puntaje = contenidoBD[2][4];
    top[3] = stoi(puntajes[3].puntaje);
    puntajes[4].puntaje = contenidoBD[2][5];
    top[4] = stoi(puntajes[4].puntaje);

    jugadorBD[0] = contenidoBD[3][1];
    jugadorBD[1] = contenidoBD[3][2];
    jugadorBD[2] = contenidoBD[3][3];
    jugadorBD[3] = contenidoBD[3][4];
    jugadorBD[4] = contenidoBD[3][5];

    baseDatos.close();
}

void actualizarBD()
{
    ofstream baseDatos;
    baseDatos.open("baseDatos.csv", ios::out);

    baseDatos << "partidasGanadas," << partidasGanadas << endl;
    baseDatos << "partidasCanceladas," << partidasCanceladas << endl;
    baseDatos << "puntajes," << top[0] << "," << top[1] << "," << top[2] << "," << top[3] << "," << top[4] << endl;
    baseDatos << "jugadores," << jugadorBD[0] << "," << jugadorBD[1] << "," << jugadorBD[2] << "," << jugadorBD[3] << "," << jugadorBD[4] << endl;

    baseDatos.close();
}

void menuPrincipal()
{

    do
    {
        std::cout << "\n#####################################################################################################\n";
        std::cout << "\n*****************************************************************************************************\n";
        std::cout << "\n*\t--------------------------------- U A N S H I P S --------------------------------          *\n";
        std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
        std::cout << "\n*\t                                 Escoja una opcion:  \t\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t        1. Iniciar Partida\t\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t    2. Estadisticas del juego\t\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t\t     0. Salir\t\t\t\t\t\t    *\n";
        std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
        std::cout << "\n*****************************************************************************************************\n";
        std::cout << "\n#####################################################################################################\n";
        std::cout << "\t\t\t\t\t       ";
        std::cin >> opcionUsuario;

        switch (opcionUsuario)
        {
        case '1':
            std::system("clear");
            posicionarBarcos();
            iniciarPartida();
            break;

        case '2':
            menuEstadisticas();
            break;

        case '0':
            std::cout << "Cerrando Juego..." << endl;
            actualizarBD();
            exit(1);

        default:
            std::system("clear");
            std::cout << endl
                      << opcionDef << endl
                      << endl;
            menuPrincipal();
            break;
        }
    } while (opcionUsuario != '0');
}

void iniciarPartida()
{
    if (estadoCodigoMatarBarcos)
    {
        matarBarcos();
        std::system("clear");
    }

    std::cout << "\n#####################################################################################################\n";
    std::cout << "\n*****************************************************************************************************\n";
    std::cout << "\n*\t--------------------------------- U A N S H I P S --------------------------------          *\n";
    std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";

    if (estadoCodigoMostrarMapaInvisible)
    {
        pintarMapaInvisible();
    }

    actualizarMapaVisible();
    mostrarEstadoBarco();
    pintarMapaVisible();

    std::cout << "\n\t\t\t\t       Escoja una opcion:" << endl;
    std::cout << "\t\t\t\t    1. Introducir coordenada" << endl;
    std::cout << "\t\t\t\t      0. Cancelar partida" << endl;
    std::cout << "\t\t\t\t\t       ";
    std::cin >> opcionUsuario;

    switch (opcionUsuario)
    {
    case '1':
        introducirCoordenadas();
        break;

    case '0':
        std::cout << "\t\t\t  ¿Está seguro de querer cancelar la partida?" << endl;
        std::cout << "\t\t\t\t\t     1. Si" << endl;
        std::cout << "\t\t\t\t\t     0. No" << endl;
        std::cout << "\t\t\t\t\t       ";
        std::cin >> opcionUsuario;

        switch (opcionUsuario)
        {
        case '1':
            std::system("clear");
            partidasCanceladas++;

            resetearJuego();
            menuPrincipal();
            break;

        case '0':
            iniciarPartida();
            break;

        default:
            std::cout << endl
                      << opcionDef << endl
                      << endl;
            iniciarPartida();
            break;
        }
        break;

    case 'c':
        std::cout << "\t\t\t\t            ";
        std::cin >> codigoUsuario;
        std::system("clear");

        if (codigoUsuario == codigoMostrarMapaInvisible)
        {
            estadoCodigoMostrarMapaInvisible = true;
        }
        if (codigoUsuario == codigoMatarBarcos)
        {
            estadoCodigoMatarBarcos = true;
        }

        iniciarPartida();
        break;

    default:
        std::system("clear");
        std::cout << endl
                  << opcionDef << endl
                  << endl;
        iniciarPartida();
        break;
    }
}

void mostrarEstadoBarco()
{
    std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
    std::cout << "*\t1. Porta Aviones =" << validarEstadoBarco(1) << "                                       \t\t\t    *" << endl;
    std::cout << "*\t2. Acorazado =" << validarEstadoBarco(2) << "                                           \t\t\t    *" << endl;
    std::cout << "*\t3. Buque =" << validarEstadoBarco(3) << "                                               \t\t\t    *" << endl;
    std::cout << "*\t4. Destructor =" << validarEstadoBarco(4) << "                                          \t\t\t    *" << endl;
    std::cout << "*\t5. Fragata =" << validarEstadoBarco(5) << "                                             \t\t\t    *" << endl;
    std::cout << "*\t6. Lancha Rapida =" << validarEstadoBarco(6) << "                                       \t\t\t    *" << endl;
    std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
}

void menuEstadisticas()
{
    std::system("clear");
    do
    {
        std::cout << "\n#####################################################################################################\n";
        std::cout << "\n*****************************************************************************************************\n";
        std::cout << "\n*\t***********************************************************************************\t    *\n";
        std::cout << "\n*\t                               ESTADISTICAS DEL JUEGO  \t\t\t\t\t    *\n";
        std::cout << "\n*\t***********************************************************************************\t    *\n";
        std::cout << "\n*\t-----------------------------------------------------------------------------------\t    *\n";
        std::cout << "\n*\t                                 Escoja una opcion:  \t\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t  1. Número de partidas ganadas\t\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t 2. Número de partidas canceladas\t\t\t\t    *\n";
        std::cout << "\n*\t\t\t\t    3. Top 5 mejores puntajes\t\t\t\t            *\n";
        std::cout << "\n*\t\t\t\t  0. Regresar al menú principal\t\t\t\t            *\n";
        std::cout << "\n*\t-----------------------------------------------------------------------------------\t    *\n";
        std::cout << "\n*****************************************************************************************************\n";
        std::cout << "\n#####################################################################################################\n";
        std::cout << "\t\t\t\t\t       ";
        std::cin >> opcionUsuario;
        std::system("clear");

        switch (opcionUsuario)
        {
        case '1':
            std::cout << "\n#####################################################################################################\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n*\t--------------------------------- U A N S H I P S --------------------------------          *\n";
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*\t                                 PARTIDAS GANADAS                                           *\n";
            std::cout << "\n*\t\t\t\tEl número de partidas ganadas es: " << partidasGanadas << "\t\t\t\t    *" << endl;
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n#####################################################################################################\n";
            std::cout << endl;
            break;
        case '2':
            std::cout << "\n#####################################################################################################\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n*\t--------------------------------- U A N S H I P S --------------------------------          *\n";
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*\t                                 PARTIDAS CANCELADAS\t\t\t\t\t    *\n";
            std::cout << "\n*\t\t\t       El número de partidas canceladas es: " << partidasCanceladas << "\t\t\t\t    *" << endl;
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n#####################################################################################################\n";
            std::cout << endl;
            break;
        case '3':
            std::cout << "\n#####################################################################################################\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n*\t--------------------------------- U A N S H I P S --------------------------------          *\n";
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*\t                                 TOP 5 PUNTAJES \n";
            std::cout << "\n*\t\t\t\t  Los 5 mejores puntajes son: " << endl;
            std::cout << "\n\t\t\t\t\t     1. " << jugadorBD[0] << ": " << top[0] << endl;
            std::cout << "\n\t\t\t\t\t     2. " << jugadorBD[1] << ": " << top[1] << endl;
            std::cout << "\n\t\t\t\t\t     3. " << jugadorBD[2] << ": " << top[2] << endl;
            std::cout << "\n\t\t\t\t\t     4. " << jugadorBD[3] << ": " << top[3] << endl;
            std::cout << "\n\t\t\t\t\t     5. " << jugadorBD[4] << ": " << top[4] << endl;
            std::cout << "\n*\t----------------------------------------------------------------------------------          *\n";
            std::cout << "\n*****************************************************************************************************\n";
            std::cout << "\n#####################################################################################################\n";
            std::cout << endl;
            break;
        case '0':
            menuPrincipal();
            break;
        default:
            std::cout << endl
                      << opcionDef << endl
                      << endl;
            break;
        }
    } while (opcionUsuario != '0');
}

void matarBarcos()
{
    int numeroBarco = 0;
    for (int i = 3; i < 17; i++)
    {
        for (int j = 3; j < 17; j++)
        {
            if (mapaInvisible[i][j] != 6 && mapaInvisible[i][j] != 9 && mapaInvisible[i][j] != 0)
            {
                mapaVisible[i - 3][j - 3] = 'X';
                numeroBarco = mapaInvisible[i][j];
                validarBarcoHundido(numeroBarco);
            }
        }
    }

    celdasAcertadas = 29;
    estadoCodigoMatarBarcos = false;
}

void pintarMapaVisible()
{
    if (estadoCodigoMostrarMapaInvisible)
    {
        std::cout << "*    MAPA DE JUEGO " << endl;
    }

    int filasMapaVisible = 0;
    std::cout << "*\t\t     0   1   2   3   4   5   6   7   8   9  10  11  12  13" << endl;
    for (int i = 0; i < 14; i++)
    {
        std::cout << "*\t\t   ---------------------------------------------------------\t\t\t    *" << endl;
        for (int j = 0; j < 14; j++)
        {
            if (j == 0)
            {
                std::cout << "*\t\t" << filasMapaVisible;
                if (filasMapaVisible < 10)
                {
                    std::cout << " ";
                }

                filasMapaVisible++;
            }
            std::cout << " | ";
            std::cout << mapaVisible[i][j];
        }
        std::cout << " |\t\t\t    *" << endl;
    }
    std::cout << "*\t\t   ---------------------------------------------------------" << endl;
}

void pintarMapaInvisible()
{
    int filasMapaInvisible = 0;
    std::cout << "*    MAPA CON LA POSICION DE LOS BARCOS" << endl;
    std::cout << "*\t\t     0   1   2   3   4   5   6   7   8   9  10  11  12  13" << endl;
    for (int i = 3; i < 17; i++)
    {
        std::cout << "*\t\t   ---------------------------------------------------------\t\t\t    *" << endl;
        for (int j = 3; j < 17; j++)
        {
            if (j == 3)
            {
                std::cout << "*\t\t" << filasMapaInvisible;
                if (filasMapaInvisible < 10)
                {
                    std::cout << " ";
                }

                filasMapaInvisible++;
            }
            std::cout << " | ";
            if (i > 2 && j > 2)
            {
                std::cout << mapaInvisible[i][j];
            }
        }
        std::cout << " |\t\t\t    *" << endl;
    }
    std::cout << "*\t\t   --------------------------------------------------------- \n\n"
              << endl;
}

void actualizarMapaVisible()
{
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            if (mapaVisible[i][j] != 'X' && mapaVisible[i][j] != 'O')
            {
                mapaVisible[i][j] = '~';
            }
        }
    }
}

void introducirCoordenadas()
{
    int filaUsuario = 0, columnaUsuario = 0;
    int filaInvisible = 0, columnaInvisible = 0;
    int numeroBarco;

    bool acertado;
    std::cout << endl
              << "\t\t\t\t     Digite las coordenadas" << endl;

    std::cout << "\t\t\t\t\t   Fila: ";
    std::cin >> filaUsuario;

    if (filaUsuario < 0 || filaUsuario > 13)
    {
        std::cout << "La fila introducida se encuentra fuera del rango del mapa, intente nuevamente" << endl;
        introducirCoordenadas();
    }

    std::cout << "\t\t\t\t\t Columna: ";
    std::cin >> columnaUsuario;
    if (columnaUsuario < 0 || columnaUsuario > 13)
    {
        std::cout << "La columna introducida se encuentra fuera del rango del mapa, intente nuevamente" << endl;
        introducirCoordenadas();
    }

    filaInvisible = filaUsuario;
    columnaInvisible = columnaUsuario;

    filaInvisible += 3;
    columnaInvisible += 3;

    if (mapaInvisible[filaInvisible][columnaInvisible] != 0 && mapaInvisible[filaInvisible][columnaInvisible] != 9)
    {
        if (mapaVisible[filaUsuario][columnaUsuario] != '~')
        {
            std::system("clear");
            std::cout << "\t\t\t  -*-*-*--*-*-*-Esta posicion ya fue atacada, intente de nuevo-*-*-*--*-*-*-" << endl;
            iniciarPartida();
        }
        else
        {
            std::system("clear");
            std::cout << "\t\t\t    -*-*-*--*-*-*-¡Le ha dado a un barco!-*-*-*--*-*-*-" << endl;
            numeroBarco = mapaInvisible[filaInvisible][columnaInvisible];

            validarBarcoHundido(numeroBarco);

            acertado = true;
            pintarTiro(filaUsuario, columnaUsuario, acertado);
            celdasAcertadas++;

            if (celdasAcertadas == 32)
            {
                partidaGanada();
            }
        }
    }
    else
    {
        std::system("clear");
        std::cout << "\t\t\t    -*-*-*--*-*-*-¡Tiro fallido!-*-*-*--*-*-*-" << endl;
        acertado = false;
        pintarTiro(filaUsuario, columnaUsuario, acertado);

        celdasErradas++;
    }

    iniciarPartida();
}

void partidaGanada()
{
    std::system("clear");

    string nombreGanador;

    puntuacionPartida = 196 - celdasErradas;
    estadoCodigoMostrarMapaInvisible = false;
    estadoCodigoMatarBarcos = false;
    partidasGanadas++;

    std::cout << "¡FELICITACIOINES, HA GANADO LA PARTIDA! Su puntuacion es: " << puntuacionPartida << endl
              << endl;

    std::cout << "Digite su nombre: ";
    std::cin >> nombreGanador;

    int guardar;
    string guardarNombre;
    string meterNombre;
    int meter;
    int limite = 5;
    int indice;

    for (int i = 0; i < 5; i++)
    {
        if (top[i] < puntuacionPartida)
        {
            guardar = top[i];
            guardarNombre = jugadorBD[i];
            top[i] = puntuacionPartida;
            jugadorBD[i] = nombreGanador;
            indice = i + 1;
            break;
        }
        limite--;
    }

    for (int i = indice; i <= limite; i++)
    {

        meter = guardar;
        meterNombre = guardarNombre;
        guardar = top[i];
        guardarNombre = jugadorBD[i];
        top[i] = meter;
        jugadorBD[i] = meterNombre;
    }

    /*
    if (top1 < puntuacionPartida)
    {
        player2BD = player1BD;
        top2 = top1;
        std::cin >> player1BD;
        top1 = puntuacionPartida;
    }
    else if (top2 < puntuacionPartida)
    {
        player3BD = player2BD;
        top3 = top2;
        std::cin >> player2BD;
        top2 = puntuacionPartida;
    }
    else if (top3 < puntuacionPartida)
    {
        player4BD = player3BD;
        top4 = top3;
        std::cin >> player3BD;
        top3 = puntuacionPartida;
    }
    else if (top4 < puntuacionPartida)
    {

        player5BD = player4BD;
        top5 = top4;

        std::cin >> player4BD;
        top4 = puntuacionPartida;
    }
    else if (top5 < puntuacionPartida)
    {
        std::cin >> player5BD;
        top5 = puntuacionPartida;
    }*/

    std::system("clear");

    resetearJuego();
    menuPrincipal();
}

void pintarTiro(int fila, int columna, bool acertado)
{
    if (acertado)
    {
        mapaVisible[fila][columna] = 'X';
    }
    else
    {
        mapaVisible[fila][columna] = 'O';
    }
}

void validarBarcoHundido(int numeroBarco)
{
    switch (numeroBarco)
    {
    case 1:
        portaAvionesAtacado++;
        if (portaAvionesAtacado == 10)
        {
            std::cout << "¡Ha hundido el Porta Aviones!" << endl;
        }
        break;

    case 2:
        acorazadoAtacado++;
        if (acorazadoAtacado == 6)
        {
            std::cout << "¡Ha hundido el Acorazado!" << endl;
        }
        break;

    case 3:
        buqueAtacado++;
        if (buqueAtacado == 5)
        {
            std::cout << "¡Ha hundido el Buque!" << endl;
        }
        break;

    case 4:
        destructorAtacado++;
        if (destructorAtacado == 4)
        {
            std::cout << "¡Ha hundido el Destructor!" << endl;
        }
        break;

    case 5:
        fragataAtacado++;
        if (fragataAtacado == 4)
        {
            std::cout << "¡Ha hundido la Fragata!" << endl;
        }
        break;

    case 6:
        lanchaRapidaAtacado++;
        if (lanchaRapidaAtacado == 3)
        {
            std::cout << "¡Ha hundido la Lancha Rapida!" << endl;
        }
        break;

    default:
        break;
        validarEstadoBarco(numeroBarco);
    }
}

string validarEstadoBarco(int numeroBarco)
{
    string estado = " Barco hundido";

    switch (numeroBarco)
    {
    case 1:
        if (portaAvionesAtacado < 10)
        {
            estado = " Barco activo";
        }
        break;

    case 2:
        if (acorazadoAtacado < 6)
        {
            estado = " Barco activo";
        }

        break;

    case 3:
        if (buqueAtacado < 5)
        {
            estado = " Barco activo";
        }
        break;

    case 4:
        if (destructorAtacado < 4)
        {
            estado = " Barco activo";
        }

        break;

    case 5:
        if (fragataAtacado < 4)
        {
            estado = " Barco activo";
        }
        break;

    case 6:
        if (lanchaRapidaAtacado < 3)
        {
            estado = " Barco activo";
        }
        break;

    default:
        return 0;
        break;
    }
    return estado;
}

void posicionarBarcos()
{
    bool horizontal;
    int numeroBarco = 1;

    do
    {
        horizontal = numeroAleatorio(2);
        if (horizontal == true)
        {
            filasBarcos[numeroBarco - 1] = tamanoBarcos[numeroBarco - 1][0];
            columnasBarcos[numeroBarco - 1] = tamanoBarcos[numeroBarco - 1][1];
        }
        else
        {
            filasBarcos[numeroBarco - 1] = tamanoBarcos[numeroBarco - 1][1];
            columnasBarcos[numeroBarco - 1] = tamanoBarcos[numeroBarco - 1][0];
        }

        pintarBarcoInvisible(filasBarcos[numeroBarco - 1], columnasBarcos[numeroBarco - 1], numeroBarco, horizontal);
        numeroBarco++;
    } while (numeroBarco <= 6);
}

void pintarBarcoInvisible(int filas, int columnas, int numeroBarco, bool horizontal)
{
    int numeroAleatorioX = 0, numeroAleatorioY = 0, controlX, controlY;

    do
    {
        numeroAleatorioX = asignarEjeX(17 - filas);
        numeroAleatorioY = asignarEjeY(17 - columnas);
    } while (validacionPosicionBarco(filas, columnas, numeroAleatorioX, numeroAleatorioY, horizontal));

    controlX = numeroAleatorioX;

    pintarMargen(filas, columnas, numeroAleatorioX, numeroAleatorioY, horizontal);
    for (int i = 0; i < filas; i++)
    {
        controlY = numeroAleatorioY;
        for (int j = 0; j < columnas; j++)
        {
            mapaInvisible[controlX][controlY] = numeroBarco;

            controlY++;
        }
        controlX++;
    }
    eliminarMargenBarcoMapaInvisible();
}

int numeroAleatorio(int max)
{
    int numero = rand() % max;

    return numero;
}

bool validacionPosicionBarco(int filas, int columnas, int posicionX, int posicionY, bool horizontal)
{
    bool validacion = false;

    if (horizontal == true)
    {
        columnas += 2;
        posicionY--;
    }
    else
    {
        filas += 2;
        posicionX--;
    }

    int controlX = posicionX, controlY = posicionY;

    for (int i = 0; i < filas; i++)
    {
        posicionY = controlY;
        for (int j = 0; j < columnas; j++)
        {
            if (mapaInvisible[posicionX][posicionY] != 0)
            {
                validacion = true;
            }

            if (horizontal == true)
            {
                if (mapaInvisible[posicionX + 1][posicionY] != 0 || mapaInvisible[posicionX - 1][posicionY] != 0)
                {
                    validacion = true;
                }
            }
            else
            {
                if (mapaInvisible[posicionX][posicionY + 1] != 0 || mapaInvisible[posicionX][posicionY - 1] != 0)
                {
                    validacion = true;
                }
            }
            if (validacion == true)
            {
                break;
            }
            else
            {
                posicionY++;
            }
        }
        if (validacion == true)
        {
            break;
        }
        else
        {
            posicionX++;
        }
    }

    return validacion;
}

int asignarEjeX(int maximoX)
{
    int x = 0;

    do
    {
        x = numeroAleatorio(20);
    } while (x < 3 || x > maximoX);

    return x;
}

int asignarEjeY(int maximoY)
{
    int y = 0;
    do
    {
        y = numeroAleatorio(20);
    } while (y < 3 || y > maximoY);

    return y;
}

void pintarMargen(int filas, int columnas, int posicionX, int posicionY, bool horizontal)
{
    if (horizontal == true)
    {
        columnas += 2;
        posicionY--;
    }
    else
    {
        filas += 2;
        posicionX--;
    }

    int controlX = posicionX, controlY = posicionY;

    for (int i = 0; i < filas; i++)
    {
        posicionY = controlY;
        for (int j = 0; j < columnas; j++)
        {
            if (horizontal == true)
            {
                mapaInvisible[posicionX - 1][posicionY] = 9;
                mapaInvisible[posicionX][posicionY] = 9;
                mapaInvisible[posicionX + 1][posicionY] = 9;
            }
            else
            {
                mapaInvisible[posicionX][posicionY - 1] = 9;
                mapaInvisible[posicionX][posicionY] = 9;
                mapaInvisible[posicionX][posicionY + 1] = 9;
            }
            posicionY++;
        }
        posicionX++;
    }
}

void eliminarMargenBarcoMapaInvisible()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (mapaInvisible[i][j] == 9)
            {
                mapaInvisible[i][j] = 0;
            }
        }
    }
}

void resetearJuego()
{
    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            mapaVisible[i][j] = '~';
            mapaInvisible[i + 3][j + 3] = 0;
        }
    }

    portaAvionesAtacado = 0;
    acorazadoAtacado = 0;
    buqueAtacado = 0;
    destructorAtacado = 0;
    fragataAtacado = 0;
    lanchaRapidaAtacado = 0;
    celdasAcertadas = 0;
    celdasErradas = 0;
}
