#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h>

using namespace std;

int main(){
    string mapa_archivos = "/home/rudy/2024/SO/SistemasOperativos/complementos/mapa.txt";
    string libros = "/home/rudy/2024/SO/SistemasOperativos/libros";
    int id = 0;
    ofstream mapa(mapa_archivos);

    if (!mapa){
        cerr << "El archivo no se pudo abrir" << endl;
    }

    for (auto entrada : filesystem::directory_iterator(libros)){
        if(entrada.is_regular_file()){
            mapa << entrada.path().stem().string() << ", " << id << endl;
            id++;
        }
    }
    mapa.close();

    

    return 0;
}
