#include <iostream>
#include <string>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <getopt.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>  // Para fork() y exec()
#include <sys/wait.h> // Para waitpid()

#include "funciones.h"
#include "interfaz.h"

using namespace std;

void iniciarServicios();

int main (int argc, char* argv[]){
    if (argc != 11) {
        system("clear");
        cout<<"\033[31m"<< "ERROR!!! Número incorrecto de argumentos."<<"\033[0m"<<endl;
        cout<<"Deben ser de la forma './trabajo1 -u user -p password -t texto -v vector -n numero'." << endl;
        return 1;
    }


    int opt;
    string username = "";
    string password = "";
    string frase = " ";
    vector <int> vec = {}; 
    float num;
    envLoad();
    char* pathBD = getenv("PATHBD");

    //Ciclo while para capturar los parámetros de la línea de comandos
    while ((opt = getopt(argc, argv, "u:p:t:v:n:")) != -1) {
        switch (opt) {
            case 'u':
                username = optarg;
                break;
            case 'p':
                password = optarg;
                break;
            case 't':
                frase = optarg;
                break;
            case 'v':
                vec = procesaVector(optarg);
                break;
            case 'n':
                num = atoi(optarg);
                break;
            default:
                cerr<<"\033[31m" << "Uso incorrecto de argumentos." <<"\033[0m"<<endl;
                return 1;
        }
    }

    Usuario usuario = procesarUsuario(username, password, pathBD);
    if (usuario.username == "" || usuario.password ==  ""){
        system("clear");
        cerr<< "Error: Usuario o contraseña incorrectos."<<endl;
        return 1;
    } 
    //Verificación de parámetros
    if (num == 0) {
        system("clear");
        cout<<"\033[31m"<< "Error, debe ingresar un numero que sea distinto de cero."<<"\033[0m"<<endl;
        return 1;  // Número inválido
    }
    iniciarServicios();
    sleep(1); //Espera 1 segundo para mostrar el menú, así aseguramos la respuesta de los socket 
    int opcion;
    do {
        opcion = menu(usuario, frase, vec, num, pathBD);
    }
    while (opcion != 6 && opcion != 7);

    return 0;
}

void iniciarServicios() {
    pid_t pidCache, pidMotor;

    // Lanzar el proceso CACHE
    pidCache = fork();
    if (pidCache == 0) { // Proceso hijo para CACHE
        execl("./c", "CACHE", NULL);  // Ejecuta el archivo CACHE
        perror("Error al ejecutar CACHE"); // En caso de error
        exit(EXIT_FAILURE); // Termina el proceso hijo en caso de fallo
    }

    // Lanzar el proceso MOTOR DE BÚSQUEDA
    pidMotor = fork();
    if (pidMotor == 0) { // Proceso hijo para MOTOR DE BÚSQUEDA
        execl("./m", "MOTOR_DE_BUSQUEDA", NULL);  // Ejecuta el archivo MOTOR
        perror("Error al ejecutar MOTOR DE BÚSQUEDA"); // En caso de error
        exit(EXIT_FAILURE); // Termina el proceso hijo en caso de fallo
    }

    // En el proceso principal (padre), los servicios ahora están ejecutándose en segundo plano
    cout << "Servicios CACHE y MOTOR DE BÚSQUEDA iniciados." <<endl<<endl;
}
