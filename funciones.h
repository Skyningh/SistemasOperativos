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

Usuario procesarUsuario(string usuario, string password, string file);
string palindromo(string a);

int vocales(string a);

int letras(string a);

int sum(vector<int> vec);

float prom(vector<int> vec);

vector<int> procesaVector(string a);

vector<string> split(string str, char separador);

tuple<string, string, string> leerEnv();

void eliminarUsuarios(string usuario, string contraseña, string file);

void crearUsuario(string username, string password, string tipo, string file);

void listarUsuarios(string file);

int verificarUser(string username, string password, string tipo, string file);

void envLoad();
#endif // FUNCTIONS_H