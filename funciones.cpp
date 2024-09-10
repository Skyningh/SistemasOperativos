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


Usuario procesarUsuario(string archivodb,  string username,  string password) {
    ifstream archivo("users_bd.txt");
    string linea;
    Usuario usuario = {"", "", ""}; // Inicializa un Usuario vacío
    //Verifica si el archivo se ha abierto correctamente
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return usuario; // Retorna un Usuario vacío en caso de error
    }
    //Lee el archivo línea por línea
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string user, pass, rol;

        //Extrae los campos separados por ','
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, rol, ',');
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
    
    while (getline(ss, token, ',')) 
        vec.push_back(stoi(token));  // Convertir cada token a entero y añadirlo al vector
    
    return vec;
}

int verificacion(string frase, vector <int> vec, int num){
    // Verificar num: no puede ser 0
    if (num == 0) {
        cout<< "Error, debe ingresar un numero que sea distinto de cero."<<endl;
        return 1;  // Número inválido
    }

    // Si todas las verificaciones pasan
    return 0;
}

tuple<string, string, string> leerEnv() {
    string usernameEnv;
    string passwordEnv;
    string path;
    
    ifstream envFile("holi.env");
    if (!envFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo .env" << endl;
        return make_tuple(usernameEnv, passwordEnv, path);
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
            } else if (key == "PATHBD"){
                path = value;
            }
        }
    }

    envFile.close();
    return make_tuple(usernameEnv, passwordEnv, path);
}