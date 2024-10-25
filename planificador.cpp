#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <getopt.h>
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

vector<string> split2(string str, char separador){
    vector<string> vectorMensaje;
    string token;
    stringstream ss(str);

    while(getline(ss, token, separador)){
        vectorMensaje.push_back(token);
    }
    return vectorMensaje;
}

int main(int argc, char* argv[]){
    int cantidadCores;
    string pathIn;
    string pathOut;
    int opt;

    while ((opt = getopt(argc, argv, "c:i:o:")) != -1) {
        switch (opt) {
            case 'c':
                cantidadCores = atoi(optarg);
                break;
            case 'i':
                pathIn = optarg;
                break;
            case 'o':
                pathOut = optarg;
                break;
            default:
                cerr<<"\033[31m" << "Error en el paso de parametros." <<"\033[0m"<<endl;
        }
    }

    string resultados = "complementos/resultados.txt";
    ofstream borrar(resultados, ios::trunc);
    if(!borrar.is_open()){
        cerr << "no se pudo abrir el archivo de resultados" << endl;
    }
    borrar.close();

    for(int i=0;i <cantidadCores; i++){
        string c = to_string(i);
        ofstream archivo("complementos/cores/core"+c+".txt");
        if(archivo.is_open()){
            archivo << "1";
        }
        else{
            cerr << "El archivo de core status no se pudo crear" << endl;
            return EXIT_FAILURE;
        }

    }

    int id = 0;
    vector<int> Cores;
    ifstream procesos(pathIn);
    
    if(!procesos.is_open()){
        cerr << "El archivo "+ pathIn + " no se puedo abrir" << endl;
        return EXIT_FAILURE;
    }
    string lineaProceso;
    while(getline(procesos,lineaProceso)){
        vector<string> vectorMensaje = split2(lineaProceso,';');
        int id = atoi(vectorMensaje[0].c_str());
        string operacion = vectorMensaje[1];
        string numeros = vectorMensaje[2];
        vector<string> aux = split2(numeros, ',');
        
        for(int i=0;i<cantidadCores; i++){
            ifstream file("complementos/cores/core"+to_string(i)+".txt");

            if(!file.is_open()){
                cerr << "El archivo complementos/cores/core"+to_string(i)+".txt"+" no se pudo abrir" << endl;
                return EXIT_FAILURE;
            }
            Cores.push_back(0);
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

        int core = selectCore(Cores);
        while (core == -1){
            this_thread::sleep_for(chrono::milliseconds(50));
            core = selectCore(Cores);
        }
        string mensaje = to_string(core)+","+to_string(id)+","+operacion+","+aux[0]+","+aux[1];
        string execdistribuidor = "./distribuidor -m '" + mensaje + "'";      
        int status = system(execdistribuidor.c_str());
        if (status == -1){
            cerr << "Falló la ejecución del programa hijo" << endl;
        }
    }

    cout << "Ruta de resultados: " << pathOut << endl;
    

    return 0;
}