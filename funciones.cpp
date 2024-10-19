#include "funciones.h"
#include <iostream>
#include <string>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <cctype>

using namespace std;

Usuario procesarUsuario(string username,  string password, string file) {
    ifstream archivo(file);
    string linea;
    Usuario usuario = {"", "", ""}; // Inicializa un Usuario vacío
    //Verifica si el archivo se ha abierto correctamente
    if (!archivo.is_open()) {
        cerr<<"\033[31m"<< "No se pudo abrir el archivo."<<"\033[0m" << endl;
        return usuario; // Retorna un Usuario vacío en caso de error
    }
    //Lee el archivo línea por línea
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string user, pass, rol;

        //Extrae los campos separados por ','
        getline(ss, user, ';');
        getline(ss, pass, ';');
        getline(ss, rol, ';');
        //Compara el usuario y la contraseña ingresados con los del archivo
        if (user == username && pass == password) {
            usuario.username = user;
            usuario.password = pass;
            usuario.rol = rol;
            return usuario; // Coincidencia encontrada
        }
    }

    //No se encontró coincidencia
    return usuario; // Retorna un Usuario vacío si no se encontró
}

string palindromo(string texto){
    texto.erase(remove(texto.begin(), texto.end(), ' '), texto.end());
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);

    if (texto.length() < 2) 
        return "es un palíndromo.";
    
    if (texto[0] == texto[texto.length()-1])
        return palindromo(texto.substr(1, texto.length() - 2));
    
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
    
    while (getline(ss, token, ';')) 
        vec.push_back(stoi(token));  // Convertir cada token a entero y añadirlo al vector
    
    cout<<endl;
    return vec;

}

void eliminarUsuarios(string usuario, string password, string file){
    ifstream archivo(file);
    ofstream archivoTemporal("temp.txt");
    string linea;
    bool usuarioEncontrado = false;
    bool primeraLinea = true;


    if (!archivo.is_open()) {
        cerr <<"\033[31m"<< "No se pudo abrir el archivo " << file <<"\033[0m"<< endl;
        return;
    }

    if (!archivoTemporal.is_open()) {
        cerr << "No se pudo crear el archivo temporal." << endl;
        archivo.close();
        return;
    }

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string user, pass, tipo;

        getline(ss, user, ';');
        getline(ss, pass, ';');
        getline(ss, tipo, ';');
        
        if (user == usuario) {
            usuarioEncontrado = true;

            // Verificar si el password es incorrecto
            if (pass != password) {
                cout << "\033[31m" << "Error: La contraseña es incorrecta para el usuario '" << usuario << "'." << "\033[0m" << endl;
                remove("temp.txt");
                archivo.close();
                archivoTemporal.close();
                return;
            }

            // Verificar si el tipo no es "user"
            if (tipo != "user") {
                cout << "\033[31m" << "Error: El usuario '" << usuario << "' no es de tipo 'user'"<<"\033[0m" << endl;
                remove("temp.txt");
                archivo.close();
                archivoTemporal.close();
                return;
            }
            // Si el usuario y contraseña coinciden y es "user", no escribimos la línea para eliminarlo
            continue;
        }

            if (!primeraLinea) {
                archivoTemporal << endl;  //Agrega nueva línea solo si no es la primera
            }

            archivoTemporal << linea;
            primeraLinea = false;
    }
    archivo.close();
    archivoTemporal.close();

    if (!usuarioEncontrado) {
        cout<<"\033[31m"<< "Error: El usuario '" << usuario << "' no fue encontrado."<<"\033[0m"<< endl;
        remove("temp.txt");
    } else {
        if (remove(file.c_str()) != 0) {
            cerr <<"\033[31m"<< "Error al eliminar el archivo original." <<"\033[0m"<< endl;
            return;
        }
        if (rename("temp.txt", file.c_str()) != 0) {
            cerr<<"\033[31m" << "Error al renombrar el archivo temporal." <<"\033[0m"<< endl;
            return;
        }
        cout <<"\033[32m"<< "Usuario '" << usuario << "' borrado correctamente." <<"\033[0m"<< endl;
    }
}

void crearUsuario(string username, string password, string tipo, string file){
    //abrir el archivo con modo append (ios::app)
    ofstream archivo(file, ios::app);
    
    if (archivo.is_open()) {
        archivo <<endl<<username << ";" << password << ";" << tipo;
        archivo.close();
        cout <<"\033[32m"<< "El usuario fue creado correctamente." <<"\033[0m"<< endl<<endl;
    } else {
        cerr << "Ocurrió un error al abrir el archivo para crear el usuario." << endl;
    }
}

void listarUsuarios(string file){
    ifstream archivo(file);
    string linea;
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
    }
    int count = 1;
    //Lee el archivo línea por línea
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string user, pass, rol;

        //Extrae los campos separados por ','
        getline(ss, user, ';');
        getline(ss, pass, ';');
        getline(ss, rol, ';');

    cout<<count<<".- "<<"Usuario: "<<user<<endl;
    cout<<"Contraseña: "<<pass<<endl;
    cout<<"Rol: "<<rol<<endl<<endl;
    count++;
    }
}

int verificarUser(string username, string password, string tipo, string file){
    //verificamos username
    if (username.length() < 3) {
        cout << "Error, el nombre de usuario debe tener al menos 3 caracteres." << endl;
        return 0;
    }
    for (char c: username)
        if (!isalpha(c)) {
            cout << "Error, el nombre de usuario solo puede contener letras." << endl;
            return 0;
        }
    //verificamos password
    if (password.length() < 6) {
        cout << "Error, la contraseña debe tener al menos 6 caracteres." << endl;
        return 0;
    }
    for (char c: password)
        if (!isalnum(c)) {
            cout << "Error, la contraseña solo puede contener letras o números." << endl;
            return 0;
        }

    //verificamos el tipo, que sea admin o user
    if (tipo != "admin" && tipo != "user") {
        cout << "Error, el tipo debe ser 'admin' o 'user'." << endl;
        return 0;
    }

    //verificamos si el usuario ya existe
    ifstream archivo(file);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para la verificación." << endl;
        return 0;
    }

    string linea, user;
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        getline(ss, user, ';');
        if (user == username) {
            cout << "Error, el usuario '" << username << "' ya existe." << endl;
            archivo.close();
            return 0;
        }
    }

    archivo.close();

    return 1;
}

void envLoad() {
    const char* envFile = "var.env";
    ifstream file(envFile);
    if (!file) {
        cerr << "No se pudo abrir el archivo .env" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        size_t equalsPos = line.find('=');
        if (equalsPos != string::npos) {
            string key = line.substr(0, equalsPos);
            string value = line.substr(equalsPos + 1);
            setenv(key.c_str(), value.c_str(), 1);
        }
    }

    file.close();
}

vector<string> split(string str, char separador){
    vector<string> vectorMensaje;
    string token;
    stringstream ss(str);

    while(getline(ss, token, separador)){
        vectorMensaje.push_back(token);
    }
    return vectorMensaje;
}
