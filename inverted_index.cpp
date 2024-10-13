#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>

using namespace std;

map<string,string> indice_inv;

void procesarArchivo(int id, string archivo){
    ifstream Input(archivo);
    if (!Input){
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }
    string linea;
    while(getline(Input, linea)){
        string palabra;
        int cantidad;
        stringstream ss(linea);
        getline(ss, palabra, ';');
        ss >> cantidad;

        palabra.erase(remove_if(palabra.begin(),palabra.end(), ::isspace), palabra.end());
        if(indice_inv.find(palabra) != indice_inv.end()){
            indice_inv[palabra] += ";(" + to_string(id) + "," + to_string(cantidad) + ")";
        }
        else{
            indice_inv[palabra] = "(" + to_string(id) + "," + to_string(cantidad) + ")";
        }

    }
    Input.close();
}

void creariIndex(string iIndex, string pathOut){
    int id = 0;
    for(auto entry : filesystem::directory_iterator(pathOut)){
        if(entry.path().extension() == ".txt"){
            procesarArchivo(id, entry.path().string());
            id++;
        }
    }
    ofstream Output(iIndex);
    if (!Output){
        cerr << "Error al crear el archivo de indice invertido" << endl;
        return;
    }
    
    for(auto [palabra, info] : indice_inv){
        Output << palabra << ";" << info << endl;
    }

    cout << "Indice invertido creado exitosamente, path: " << iIndex << endl; 
    Output.close();
}

int main(){
    string pathOut = "/home/rudy/2024/SO/SistemasOperativos/prueba";
    string iIndex = "/home/rudy/2024/SO/SistemasOperativos/complementos/indiceinv.txt";

    if(pathOut.empty() || iIndex.empty()){
        cerr << "Alguna de las carpetas estan vacias" << endl;
        return EXIT_FAILURE;
    }

    creariIndex(iIndex, pathOut);

    return 0;
}