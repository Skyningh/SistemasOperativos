#include <iostream>
#include <string>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <getopt.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "funciones.h"
#include "interfaz.h"

using namespace std;

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
    
    int opcion;
    do {
        opcion = menu(usuario, frase, vec, num, pathBD);
    }
    while (opcion != 6 && opcion != 7);

    return 0;
}