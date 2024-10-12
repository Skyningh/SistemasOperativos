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
namespace fs = filesystem;

int main(){
    string mapa_archivos = "/home/rudy/2024/SO/SistemasOperativos/mapa.txt";
    string libros = "/home/rudy/2024/SO/SistemasOperativos/libros";
    int id = 0;
    ofstream mapa(mapa_archivos, ios::app);
    for (auto entrada : fs::directory_iterator(libros)){
        if(entrada.is_regular_file()){
            mapa << entrada.path().stem().string() << ", " << id << endl;
            id++;
        }
    }
    mapa.close();

    

    return 0;
}