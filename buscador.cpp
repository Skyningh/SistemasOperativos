#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
using namespace std;

int main (){
    string palabra;
    cout << "Ingrese una palabra a buscar: ";
    cin >> palabra;

    //Aquí definimos la estructura en la que almacenaremos las ocurrencias de palabras accedidas recientemente. 
    //Es una map donde la key es la palabra (un string) y el value, una lista de tuplas, que son los libros y su cantidad de ocurrencias
    map <string, vector<tuple <int, int>>> cache;

    //Verificamos si es que la palabra buscada está en el caché.
    auto it = cache.find(palabra);
    if (it != cache.end()) {
        cout << "Palabra: "<< it->first << " encontrada en el caché, con ocurrencia en: " << it->second << endl;
    } else {
        cout << "Clave no encontrada, llamando a MOTOR DE BÚSQUEDA" << std::endl;

    //Comienza el proceso de búsqueda de la palabra
    return 0;
}