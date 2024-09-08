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

using namespace std;

int main (int argc, char* argv[]){
    if (argc != 11) {
        cout << "ERROR!!! Número incorrecto de argumentos."<<endl<<"Deben ser de la forma './trabajo1 -u user -p password -t texto -v vector -n numero'." << endl;
        return 1;
    }

    auto [usernameEnv, passwordEnv] = leerEnv();

    int opt;
    string username = "";
    string password = "";
    string frase = " ";
    vector <int> vec = {}; 
    int num;

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
                cerr << "Uso incorrecto de argumentos." << endl;
                return 1;
        }
    }

    //Verificación de parámetros
    if (verificacion(username, password, frase, vec, num) == 1){
        cout<< "Cancelando..."<<endl;
        return 1;
    }
    
    if (username != usernameEnv || password != passwordEnv) {
        cerr << "Error: Usuario o contraseña incorrectos." << endl;
        return 1;
    }

    int opcion;
    do {
        //Interfaz, con un do while
        cout<< "----------------------"<<endl;
        cout << "Elija una de las siguientes opciones:" <<endl;
        cout << "0. Contar palabras." << endl;
        cout << "1. Verificar si la frase es un palíndromo." << endl;
        cout << "2. Contar el número de vocales de la frase." << endl;
        cout << "3. Contar número de letras de la frase" << endl;
        cout << "4. Calcular el promedio y sumatoria del vector" << endl;
        cout << "5. Calcular la función f(x) = 5x²+1/x" << endl;
        cout << "6. Salir" << endl;
        cout << "7. Contar Palabras" << endl;
        cout << "Escriba aquí: ";
        
        cin >> opcion;
    
        switch (opcion) {


            case 1: {
                cout << "----------------------"<<endl<<endl;
                cout <<"Opción 1: Palíndromo"<<endl;
                cout <<"La frase '" <<frase <<"' "<< palindromo(frase)<<endl<<endl;
                if (subMenu() == 0) return 0;
                break;
            }
            case 2: {
                cout << "----------------------"<<endl<<endl;
                cout <<"Opción 2: Número de vocales"<<endl;
                cout <<"La frase " <<frase << " tiene " << vocales(frase)<<" vocales."<<endl<<endl;
                if (subMenu() == 0) return 0;
                break;
            }
            case 3: {
                cout << "----------------------"<<endl<<endl;
                cout <<"Opción 3: Número de letras"<<endl;
                cout << "La frase: "<<frase<<" tiene " <<letras(frase)<<" letras." <<endl<<endl;
                if (subMenu() == 0) return 0;
                break;
            }
            case 4: {
                cout << "----------------------"<<endl<<endl;
                cout <<"Opción 4: Suma y promedio del vector"<<endl;
                cout<<"La suma es: " <<sum(vec)<<endl;
                cout<<"Y el promedio: "<<prom(vec)<<endl<<endl;
                if (subMenu() == 0) return 0;
                break;
            }
            case 5: {
                cout << "----------------------"<<endl<<endl;
                cout << "5x²+(1/x) ="<< 5*num*num+(1/num) << endl<<endl;
                if (subMenu() == 0) return 0;
                break;
            }
            case 6:{
                cout << "Saliendo..." << endl;
                break;
            } 
            case 7: {
                contarmenu();
                cout << "opcion 7" << endl;
                break;
            }
            default:
                cout<<endl << "\033[31m" << "Opción inválida. Intente de nuevo.!!!!"<<"\033[0m" << endl;
            
        }
    }
    while (opcion != 6 && opcion != 7);

    return 0;
}