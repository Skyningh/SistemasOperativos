// Enviar las variables env como argumentos!!! (Para este proceso externo,y conteo dde palabras.) LISTO 
/*
Este programa hace una llamada al proceso externo "conteo de palabras", por cada elemento de array threads. LISTO
Cada una de estas llamadas debe realizarse "REPETICIONES" veces. LISTO
Los datos de threads y tiempos de ejecución deben ser analizados en "DATOS" con el proceso externo ANALIZADOR. FALTA
También faltan validaciones, y incluir el txt en el env*/ 
#include <iostream>
#include <vector>
#include <sstream>
#include <getopt.h>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <fstream>

#include "funciones.h"

using namespace std;

vector<int> procesarVector(const char* input) {
    vector<int> numeros;
    stringstream ss(input);
    string temp;
    
    // Usamos getline para dividir el string en partes separadas por comas
    while (getline(ss, temp, ',')) {
        numeros.push_back(stoi(temp));  // Convertimos cada parte en entero y lo añadimos al vector
    }
    
    return numeros;
}

int main (int argc, char* argv[]){
    char* array_threads;
    int repeticiones;    
    // Manejar los argumentos de línea de comandos usando getopt
    int opt;
    while ((opt = getopt(argc, argv, "t:r:")) != -1) {
        switch (opt) {
            case 't':
                array_threads = optarg;  // Archivo de stop words
                break;
            case 'r':
                repeticiones = atoi(optarg);  // Cantidad de hilos
                break;
            default:
                cerr << "Uso: " << argv[0] << " -t <array_threads> -r <repeticiones>" << endl;
                return EXIT_FAILURE;
        }
    }
    envLoad();
    char* stopWordsFile = getenv("stop_word");
    char* cantidad_threads1 = getenv("cantidad_thread");
    int cantidad_threads = atoi(cantidad_threads1);
    char* pathIn = getenv("pathIn");
    char* pathOut = getenv("pathOut");
    char* datos = getenv("DATOS");
    
    vector<int> vec = procesarVector(array_threads);

    ofstream datosFile(datos);
    if (!datosFile.is_open()) {
        cerr << "Error al abrir el archivo datos.txt" << endl;
        return EXIT_FAILURE;
    }


    cout<<"\033[32m"<<"Ejecutando con éxito para el vector de threads: "<< array_threads<< ", y con " <<repeticiones<<" repeticiones."<<"\033[0m"<<endl<<endl;
    for (int num : vec) {
        for (int i = 1; i<repeticiones+1 ;i++){
            cout<<"\033[32m"<< "Comienzo Ejecución\nNúmero de threads: "<<num<< " \nNúmero de repetición: "<<i<<"\033[0m"<<endl;
            auto start = chrono::high_resolution_clock::now();  // Inicio del tiempo

            string comando = "./conteo -s " +  string(stopWordsFile) +  " -t " + to_string(num) +  " -i " + string(pathIn) + " -o " + string(pathOut);
            int status = system(comando.c_str());
            auto end = chrono::high_resolution_clock::now();    // Fin del tiempo
            chrono::duration<double> duration = end - start;

            double tiempo_ejecucion = duration.count();
            datosFile << num << " " << tiempo_ejecucion << endl;

            cout << "Tiempo de ejecución para " << num << " threads, repetición " << i << ": " << tiempo_ejecucion << " segundos." << endl;

        }
    }
    datosFile.close();

    cout << endl;

    cout << "\033[32m" << "Ejecuciones finalizadas. Ejecutando el proceso ANALIZADOR..." << "\033[0m" << endl;

    string analizador_cmd = "python3 analizador.py";
    int status_analizador = system(analizador_cmd.c_str());


    if (status_analizador == 0) {
        cout << "\033[32m" << "Gráfico generado con éxito en " << getenv("GRAFICO") << "\033[0m" << endl;
    } else {
        cerr << "\033[31m" << "Error al ejecutar el proceso ANALIZADOR" << "\033[0m" << endl;
    }

    return 0;
}