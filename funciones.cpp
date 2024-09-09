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