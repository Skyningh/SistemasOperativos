#include <iostream>
#include <vector>
#include <vector>
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

    vector<int> Cores = {1,1,1};
    cout << "Cantidad de cores: " << cantidadCores << endl;
    int core = selectCore(Cores);
    while (core == -1){
        this_thread::sleep_for(chrono::milliseconds(10));
        selectCore(Cores);
    }


    return 0;
}