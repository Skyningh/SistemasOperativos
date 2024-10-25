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

bool check10 = false;


int subMenu() {
    int opcion2;
    do {
        cout << "¿Qué desea hacer?" << endl;
        cout << "1. Volver " << endl;
        cout << "0. Salir " << endl;
        cout <<"Escriba aquí: ";
        cin >> opcion2;
        
        if (opcion2 != 0 && opcion2 != 1) {
            system("clear");
            cout << endl<<"\033[31m"<< "Error, ingrese la opción otra vez!!!!!!!!"<<"\033[0m"<< endl;
        }
    } while (opcion2 != 0 && opcion2 != 1);
    if (opcion2 == 1){
        system("clear");
        cout<<"\033[32m"<<endl<< "Volviendo..."<<"\033[0m"<<endl<<endl;
    }
    else{
        system("clear");
        cout<<"\033[32m"<<endl<< "Saliendo..."<<"\033[30m"<<endl<<endl;
    }
    return opcion2;
}

int contarmenu(){
    system("clear");
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
                    system("clear");
                    cout<<"\033[32m" << "Saliendo..."<<"\033[0m" << endl;
                    break;
                }

                case 1: {
                    system("clear");
                    cout << "Opción 1: Extensión de archivos a procesar" << endl;
                    check1 = true;
                    break;
                }
                case 2: {
                    system("clear");
                    cout << "Opción 2: Path de carpeta a procesar" << endl;
                    check2 = true;
                    break;
                }
                case 3: {
                    system("clear");
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
                    system("clear");
                    if (subMenu() == 0) return 0;
                    break;

                default:
                    system("clear");
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
    cout<<"Rol: "<<usuario.rol<<endl<<endl;
    cout << "########################"<<endl<<endl;
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
        cout << "10. Conteo paralelo con threads" << endl;
        cout << "11. Crear índice invertido" << endl;
        cout << "12. Analisis de performance" << endl;
        cout << "13. Planificador" << endl;
    }
    cout << "0. Salir" << endl;

    cout << "Escriba aquí: ";
    cin >> opcion;

    switch (opcion) {

        case 1: {
            system("clear");
            cout << "########################"<<endl<<endl;
            cout <<"Opción 1: Palíndromo"<<endl;
            cout <<"La frase '" <<frase <<"' "<< palindromo(frase)<<endl<<endl  ;
            cout << "########################"<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 2: {
            system("clear");
            cout << "########################"<<endl<<endl;
            cout <<"Opción 2: Número de vocales"<<endl;
            cout <<"La frase " <<frase << " tiene " << vocales(frase)<<" vocales."<<endl<<endl;
            cout << "########################"<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 3: {
            system("clear");
            cout << "########################"<<endl<<endl;
            cout <<"Opción 3: Número de letras"<<endl;
            cout << "La frase: "<<frase<<" tiene " <<letras(frase)<<" letras." <<endl<<endl;
            cout << "########################"<<endl<<endl;

            if (subMenu() == 0) return 6;
            break;
        }
        case 4: {
            system("clear");
            cout << "########################"<<endl<<endl;
            cout <<"Opción 4: Suma y promedio del vector: "<<endl;
            cout << "Vector: ";
            for (int i=0; i < size(vec); i++){
                cout<< vec[i] <<" ";
            }
            cout<<"\nLa suma es: " <<sum(vec)<<endl;
            cout<<"Y el promedio: "<<prom(vec)<<endl<<endl;
            cout << "########################"<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 5: {
            system("clear");
            cout << "########################"<<endl<<endl;
            cout << "Opción 5: 5x²+(1/x)  [Con num = "<<num<<"]"<<endl;
            cout << "Resultado: "<<5*num*num+(1/num) << endl<<endl;
            cout << "########################"<<endl<<endl;
            if (subMenu() == 0) return 6;
            break;
        }
        case 0:{
            cout << "Saliendo..." << endl;
            return 6;
        } 
        case 6: {
            system("clear");
            //Ejecuta programa externo
            int status = system("./procesar");
            if (status == -1){
                cerr << "Falló la ejecución del programa hijo" << endl;
            } else{
                cout << "El programa hijo termino con estado: " << WEXITSTATUS(status) << endl;
            }
            cout << "Continua el proceso padre" << endl;
            menu(usuario, frase, vec, num, PATHDB);
            
            return 6; //Si retorna 6, tira la opción "6", que es salir del programa QUE ENREDO ESTOS RETURN DIOSSSS
        }
        
        // Opciones adicionales solo para administradores
        case 7:{//Ingresar nuevos usuarios
            if (usuario.rol == "admin") {
                system("clear");
                cout << "########################"<<endl<<endl;
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
                system("clear");
                cout << "########################"<<endl<<endl;
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
                system("clear");
                cout << "########################"<<endl<<endl;
                cout << "Opción 9: Lista de usuarios." << endl;
                listarUsuarios(PATHDB);
                if (subMenu() == 0) return 6;

            } else {
                cout<<"\033[31m"<< "Opción inválida, intente de nuevo!!" <<"\033[0m"<<endl;
            }
            break;
        }
        case 10:{ // HACER QUE LA 11 REVISE SI SE EJECUTO LA 10
            system("clear");

            envLoad();
            char* pathBD = getenv("PATHBD");
            char* stopWordsFile = getenv("stop_word");
            char* cantidad_threads1 = getenv("cantidad_thread");
            int cantidad_threads = atoi(cantidad_threads1);
            char* pathIn = getenv("pathIn");
            char* pathOut = getenv("pathOut");

            string comando = "./conteo -s " +  string(stopWordsFile) +  " -t " + to_string(cantidad_threads) +  " -i " + string(pathIn) + " -o " + string(pathOut);
            int status = system(comando.c_str());
            if (status == -1){
                cerr << "Falló la ejecución del programa hijo" << endl;
            } else{
                cout << "El programa hijo termino con estado: " << WEXITSTATUS(status) << endl;
                check10 = true;
            }
            cout << "Continua el proceso padre" << endl;
            menu(usuario, frase, vec, num, PATHDB);
            
            return 6; //Si retorna 6, tira la opción "6", que es salir del programa QUE ENREDO ESTOS RETURN DIOSSSS

            if (usuario.rol == "admin"){
                system("clear");
                cout << "Opción 10" << endl;
                if (subMenu() == 0) return 6;
            }
            else {
                cout<<"\033[31m"<< "Opción inválida, intente de nuevo!!" <<"\033[0m"<<endl;
            }
            break;
        }
        case 11:{
            if (check10 == false){
                system("clear");
                cout << "\033[31m"<< "Debes ejecutar la opción 10 primero!!" <<"\033[0m"<<endl;
            } else{
                if (usuario.rol == "admin"){
                    system("clear");
                    int status1 = system("./ii");
                    if (status1 == -1){
                        cerr << "Falló la ejecución del programa hijo" << endl;
                    } else{
                        cout << "El programa hijo termino con estado: " << WEXITSTATUS(status1) << endl;
                    }
                    cout << "Continua el proceso padre" << endl;
                    menu(usuario, frase, vec, num, PATHDB);

                    return 6;
                }
                else{
                    cout<<"\033[31m"<< "Opción inválida, intente de nuevo!!" <<"\033[0m"<<endl;
                }

            }
            break;
        }
        case 12:{
            system("clear");
            if (usuario.rol == "admin"){
                cout << "Opcion 12: Análisis de Performance." << endl;
                envLoad();
                char* array_threads = getenv("ARRAY_THREADS");
                char* nrepeticiones = getenv("REPETICIONES");
                int repeticiones = atoi(nrepeticiones);
                if (repeticiones < 2){
                    cout<< "Error, el número de repeticiones debe ser mayor o igual a 2."<<endl<<endl;
                    return 6;
                }
                string comando = "./ejecutador -t " +  string(array_threads) +  " -r " + to_string(repeticiones);
                int status = system(comando.c_str());

            }
            menu(usuario, frase, vec, num, PATHDB);
            break;


        }
        case 13:{
            system("clear");
            if(usuario.rol == "admin"){
                envLoad();
                char* cantidadCoresenv = getenv("CANTIDAD_CORES");
                char* pathInenv = getenv("PROCESOSM");
                char* pathOutenv = getenv("RESULTADOS");
                string cantidadCores(cantidadCoresenv);
                string pathIn(pathInenv);
                string pathOut(pathOutenv); 
                cout << "Opcion 13" << endl;
                string ejecutar = "./planificador -c "+cantidadCores+" -i "+pathIn+" -o "+pathOut;
                int status = system(ejecutar.c_str());
                if (status == -1){
                cerr << "Falló la ejecución del programa hijo" << endl;
                } 
                else{
                cout << "El programa hijo termino con estado: " << WEXITSTATUS(status) << endl;
                }

            }
            if (subMenu() == 0) return 6;
        }
        default:
            cout<<endl<<"\033[31m"<< "Opción inválida. Intente de nuevo.!!!!"<<"\033[0m"<<endl;
        
    }
    return 0;
}