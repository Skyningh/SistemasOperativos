#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include "funciones.h"
#include "funciones.cpp"

using namespace std;

int selectCore(vector<int> Cores){
    for(int i = 0; i < Cores.size(); i++){
        if (Cores[i] == 1){
            return i;
        }
    }
return -1;
}

int main(){
    envLoad();
    int cantidadCores = atoi(getenv("CANTIDAD_CORES"));
    char* pathInenv = getenv("PROCESOS");
    char* pathOutenv = getenv("RESULTADOS");
    string pathIn(pathInenv);
    string pathOut(pathOutenv);
    int id = 0;
    vector<int> Cores = {0,0,0};
    for(int i=0;i<cantidadCores; i++){
        ifstream file("complementos/cores/core"+to_string(i)+".txt");

        if(!file.is_open()){
            cerr << "El archivo no se pudo abrir" << endl;
            return EXIT_FAILURE;
        }

        string linea;
        if(getline(file, linea)){
            if(linea == "1"){
                Cores[i] = 1;
            }
            else if (linea == "0"){
                Cores[i] = 0;
            }
            else{
                cerr << "El archivo no tiene formato correcto" << endl;
            }
        }
        file.close();

    }
    for(int i=0;i<cantidadCores;i++){
        cout << Cores[i] << endl;
    }


    cout << "Cantidad de cores: " << cantidadCores << endl;
    int core = selectCore(Cores);
    while (core == -1){
        this_thread::sleep_for(chrono::milliseconds(10));
        core = selectCore(Cores);
    }
    


    return 0;
}