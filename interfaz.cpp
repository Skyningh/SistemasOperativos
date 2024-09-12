#include <iostream>
#include <string>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <getopt.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include "funciones.h"

using namespace std;


int subMenu() {
    int opcion2;
    do {
        cout << "¿Qué desea hacer?" << endl;
        cout << "1. Volver " << endl;
        cout << "0. Salir " << endl;
        cout <<"Escriba aquí: ";
        cin >> opcion2;
        
        if (opcion2 != 0 && opcion2 != 1) {
            cout << endl<<"\033[31m"<< "Error, ingrese la opción otra vez!!!!!!!!"<<"\033[0m"<< endl;
        }
    } while (opcion2 != 0 && opcion2 != 1);
    if (opcion2 == 1)
        cout<<"\033[32m"<<endl<< "Volviendo..."<<"\033[0m"<<endl<<endl;
    else
        cout<<"\033[32m"<<endl<< "Saliendo..."<<"\033[30m"<<endl<<endl;
    return opcion2;
}

int contarmenu(){
    int opcion3;
    bool check1 = false, check2 = false, check3 = false;
    do{

        pid_t pid = getpid();
        cout<< "----------------------"<<endl;
        cout << "\nPROGRAMA CONTADOR DE PALABRAS" << endl;
        cout << "PID: " << pid << endl;
        cout << "seleccione la opción: " << endl;
        cout << "\n0) Salir" << endl;
        cout << "1) Extensión de archivos a procesar(ej:txt)" << endl;
        cout << "2) Path de carpeta a procesar(eje: /home/usuario/in)" << endl;
        cout << "3) Path de carpeta que contendrá la respuesta del proceso(eje:/home/usuario/out)" << endl;
        cout << "4) Procesar" << endl;
        cout << "Escriba aquí: ";

        cin >> opcion3;

        switch (opcion3) {
                case 0: {
                    cout<<"\033[32m" << "Saliendo..."<<"\033[0m" << endl;
                    break;
                }

                case 1: {
                    cout << "Opción 1: Extensión de archivos a procesar" << endl;
                    check1 = true;
                    break;
                }
                case 2: {
                    cout << "Opción 2: Path de carpeta a procesar" << endl;
                    check2 = true;
                    break;
                }
                case 3: {
                    cout << "Opción 3: Path de carpeta que contendrá la respuesta del proceso" << endl;
                    check3 = true;
                    break;
                }
                case 4: {
                    cout << "Opción 4: Procesar" << endl;
                    if (check1 == true && check2 == true && check3 == true){
                        //process();
                        cout <<"\033[32m"<< "Procesando..."<<"\033[0m" << endl;
                    }
                    else{
                        cout<<"\033[3m" << "Tienes que completar las opciones 1 2 y 3."<<"\033[0m" << endl;
                    }
                    if (subMenu() == 0) return 0;
                    break;

                default:
                    cout<<endl << "\033[31m" << "Opción inválida. Intente de nuevo.!!!!"<<"\033[0m" << endl;
                
            }
        }
    }
    while(opcion3 != 0);
    return 0;
}




int menu(Usuario usuario,string frase, vector <int> vec, float num, string PATHDB){

    int opcion;
    pid_t pid = getpid();
    cout<<"SISTEMA "<<endl;
    cout<<"PID = "<<pid<<endl;
    cout<<"Nombre de Usuario: "<<usuario.username<<endl;
    cout<<"Rol: "<<usuario.rol<<endl;
    cout<< "----------------------"<<endl;
    cout << "Elija una de las siguientes opciones:" <<endl;
    cout << "1. Verificar si la frase es un palíndromo." << endl;
    cout << "2. Contar el número de vocales de la frase." << endl;
    cout << "3. Contar número de letras de la frase" << endl;
    cout << "4. Calcular el promedio y sumatoria del vector" << endl;
    cout << "5. Calcular la función f(x) = 5x²+1/x" << endl;
    cout << "6. Contar Palabras" << endl;
    if (usuario.rol == "admin") {
        cout << "7. Agregar un nuevo usuario" << endl;
        cout << "8. Eliminar usuarios" << endl;
        cout << "9. Lista de usuarios" << endl;

    }
    cout << "0. Salir" << endl;

    cout << "Escriba aquí: ";
    cin >> opcion;

    switch (opcion) {

        case 1: {
            cout << "----------------------"<<endl<<endl;
            cout <<"Opción 1: Palíndromo"<<endl;
            cout <<"La frase '" <<frase <<"' "<< palindromo(frase)<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 2: {
            cout << "----------------------"<<endl<<endl;
            cout <<"Opción 2: Número de vocales"<<endl;
            cout <<"La frase " <<frase << " tiene " << vocales(frase)<<" vocales."<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 3: {
            cout << "----------------------"<<endl<<endl;
            cout <<"Opción 3: Número de letras"<<endl;
            cout << "La frase: "<<frase<<" tiene " <<letras(frase)<<" letras." <<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 4: {
            cout << "----------------------"<<endl<<endl;
            cout <<"Opción 4: Suma y promedio del vector"<<endl;
            cout<<"La suma es: " <<sum(vec)<<endl;
            cout<<"Y el promedio: "<<prom(vec)<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 5: {
            cout << "----------------------"<<endl<<endl;
            cout << "Opción 5: 5x²+(1/x) ="<< 5*num*num+(1/num) << endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 0:{
            cout << "Saliendo..." << endl;
            return 6;
        } 
        case 6: {
            pid_t pidc = fork();
            if(pidc == -1){
                cerr << "Falló el fork" << endl;
                return 0;
            } else if (pidc == 0){
                execl("/home/rudy/2024/SO/SistemasOperativos/procesar", "procesar", NULL);
                cerr << "Error ejecutando programa hijo" << endl;
            } else{
                int status;
                waitpid(pidc, &status, 0);
                if (WIFEXITED(status)){
                    cout << "Terminó el proceso hijo" << endl;
                }
            }
            return 6; //Si retorna 6, tira la opción "6", que es salir del programa QUE ENREDO ESTOS RETURN DIOSSSS
        }
        
        // Opciones adicionales solo para administradores
        case 7:{//Ingresar nuevos usuarios
            if (usuario.rol == "admin") {
                cout << "----------------------"<<endl<<endl;
                cout << "Opción 7: Agregar un nuevo usuario" << endl;
                string username, password, tipo;
                bool usercorrecto = false;
                do{
                    cout << "Ingrese el nombre de usuario: ";
                    cin >> username;
                    cout << "Ingrese la contraseña: ";
                    cin >> password;
                    cout << "Ingrese el tipo de usuario (admin/user): ";
                    cin >> tipo;

                    if (verificarUser(username, password, tipo, PATHDB) == 1) 
                        usercorrecto = true;
                    else 
                        cout<<"\033[31m"<< "Por favor, intente de nuevo." <<"\033[0m"<< endl<<endl;
                }while (!usercorrecto);
                
                crearUsuario(username, password, tipo, PATHDB);
                if (subMenu() == 0) return 6;

            } else {
                cout<< "\033[31m" << "Opción inválida, intente de nuevo!!" <<"\033[0m" << endl;
            }
            break;
        }
        case 8:{
            if (usuario.rol == "admin") {
                cout << "----------------------"<<endl<<endl;
                cout << "Opción 8: Eliminar usuarios" << endl;
                string username;
                string password;
                cout << "Debe ingresar un nombre de usuario para eliminar: ";
                cin>>username;
                username.erase(remove(username.begin(), username.end(), ' '), username.end());
                cout<< "Ingrese la contraseña para "<<username<<": ";
                cin>> password;
                eliminarUsuarios(username ,password, PATHDB);
                if (subMenu() == 0) return 6;

            } else {
                cout<< "\033[31m" << "Opción inválida, intente de nuevo!!" <<"\033[0m" << endl;
            }
            break;
        }
        case 9:{
            if (usuario.rol == "admin") {
                cout << "----------------------"<<endl<<endl;
                cout << "Opción 9: Lista de usuarios." << endl;
                listarUsuarios(PATHDB);
                if (subMenu() == 0) return 6;

            } else {
                cout<<"\033[31m"<< "Opción inválida, intente de nuevo!!" <<"\033[0m"<<endl;
            }
            break;
        }
        default:
            cout<<endl<<"\033[31m"<< "Opción inválida. Intente de nuevo.!!!!"<<"\033[0m"<<endl;
        
    }
    return 0;
}