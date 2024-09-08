#include "funciones.h"
#include <iostream>
#include <string>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace std;

string palindromo(string a){

    if (a.length() < 2) 
        return "es un palíndromo.";
    
    if (a[0] == a[a.length()-1])
        return palindromo(a.substr(1, a.length() - 2));
    
    else
        return "no es un palíndromo.";
    
}

int vocales(string a){
    vector<char> vocales = {'a', 'e', 'i', 'o', 'u'};

    int count = 0;

    for (char c : a) {
    auto it = find(vocales.begin(), vocales.end(), c);

    if (it != vocales.end()) 
        count++;
    }

    return count;
}

int letras(string a){
    int count = 0;
    for (char c : a) {
        if (isalpha(c)) 
            count++;
    }
    return count;
}
 
int sum(vector<int> vec){
    int sum = 0;
    for (int num:vec){
        sum = sum + num;
    }
    return sum;
}

float prom(vector<int> vec){
    float promedio = float(sum(vec))/float(vec.size());
    return promedio;
}

vector <int> procesaVector(string a){
    stringstream ss(a);
    string token;
    vector <int> vec;
    
    while (getline(ss, token, ',')) 
        vec.push_back(stoi(token));  // Convertir cada token a entero y añadirlo al vector
    
    return vec;
}

int subMenu() {
    int opcion2;
    do {
        cout << "¿Qué desea hacer?" << endl;
        cout << "1. Volver " << endl;
        cout << "0. Salir " << endl;
        cout <<"Escriba aquí: ";
        cin >> opcion2;
        
        if (opcion2 != 0 && opcion2 != 1) {
            cout << endl << "Error, ingrese la opción otra vez!!!!!!!!" << endl;
        }
    } while (opcion2 != 0 && opcion2 != 1);
    if (opcion2 == 1)
        cout<<endl<< "Volviendo..."<<endl<<endl;
    else
        cout<<endl<< "Saliendo..."<<endl<<endl;
    return opcion2;
}

tuple<string, string> leerEnv() {
    string usernameEnv;
    string passwordEnv;

    ifstream envFile("holi.env");
    if (!envFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo .env" << endl;
        return make_tuple(usernameEnv, passwordEnv);
    }

    string line;
    while (getline(envFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            if (key == "USERNAME") {
                usernameEnv = value;
            } else if (key == "PASSWORD") {
                passwordEnv = value;
            }
        }
    }

    envFile.close();
    return make_tuple(usernameEnv, passwordEnv);
}

int verificacion(string username, string password, string frase, vector <int> vec, int num){
    if (username.length() < 3) {
    cout<< "Error, nombre de usuario debe tener al menos 3 caracteres"<<endl;
        return 1;  // Nombre de usuario demasiado corto
    }
    for (char c : username) {
        if (!isalpha(c)) {
            cout<< "Error, nombre de usuario solo debe contener letras."<<endl;
            return 1;  // Nombre de usuario contiene caracteres no alfabéticos
        }
    }

    // Verificar password: debe tener más de 6 caracteres y ser alfanumérico
    if (password.length() <= 6) {
        cout<< "Error, la contraseña debe tener al menos 6 caracteres."<<endl;
        return 1;  // Contraseña demasiado corta
    }
    for (char c : password) {
        if (!isalnum(c)) {
             cout<< "Error, la contraseña solo debe contener letras y números."<<endl;
            return 1;  // Contraseña contiene caracteres no alfanuméricos
        }
    }

    // Verificar num: no puede ser 0
    if (num == 0) {
        cout<< "Error, debe ingresar un numero que sea distinto de cero."<<endl;
        return 1;  // Número inválido
    }

    // Si todas las verificaciones pasan
    return 0;
}

int contarmenu(){
    int opcion3;
    do{

        pid_t pid = getpid();
        cout << "\nPrograma contador de palabras" << endl;
        cout << "PID: " << pid << endl;
        cout << "seleccione la opcion: " << endl;
        cout << "\n0) Salir" << endl;
        cout << "1) Extensión de archivos a procesar(ej:txt)" << endl;
        cout << "2) Path de carpeta a procesar(eje: /home/usuario/in)" << endl;
        cout << "3) Path de carpeta que contendrá la respuesta del proceso(eje:/home/usuario/out)" << endl;
        cout << "4) Procesar" << endl;
        cout << "Escriba aquí: ";

        cin >> opcion3;

        switch (opcion3) {
                case 0: {
                    cout << "Saliendo..." << endl;
                    break;
                }

                case 1: {
                    cout << "Opción 1: Extensión de archivos a procesar" << endl;
                    if (subMenu() == 0) return 0;
                    break;
                }
                case 2: {
                    cout << "Opción 2: Path de carpeta a procesar" << endl;
                    if (subMenu() == 0) return 0;
                    break;
                }
                case 3: {
                    cout << "Opción 3: Path de carpeta que contendrá la respuesta del proceso" << endl;
                    if (subMenu() == 0) return 0;
                    break;
                }
                case 4: {
                    cout << "Opción 4: Procesar" << endl;
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