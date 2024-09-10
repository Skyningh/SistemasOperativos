using namespace std;
#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <vector>
#include <tuple>


struct Usuario {
    string username;
    string password;
    string rol;
};

Usuario procesarUsuario(string archivo, string usuario, string password);

string palindromo(string a);

int vocales(string a);

int letras(string a);

int sum(vector<int> vec);

float prom(vector<int> vec);

vector<int> procesaVector(string a);

tuple<string, string, string> leerEnv();

int verificacion(string frase, vector<int> vec, int num);

int process();

#endif // FUNCTIONS_H