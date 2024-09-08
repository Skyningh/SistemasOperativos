using namespace std;
#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <vector>
#include <tuple>

string palindromo(string a);
int vocales(string a);
int letras(string a);
int sum(vector<int> vec);
float prom(vector<int> vec);
vector<int> procesaVector(string a);
int subMenu();
tuple<string, string> leerEnv();
int verificacion(string username, string password, string frase, vector<int> vec, int num);
int contarmenu();

#endif // FUNCTIONS_H