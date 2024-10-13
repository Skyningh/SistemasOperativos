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

mutex mtx; // Mutex para la exclusión mutua
mutex cout_mutex; // Mutex para la salida en consola
condition_variable cv; // Variable de condición para la sincronización
queue<string> work_queue; // Cola de trabajo para las rutas de archivos
bool terminado = false; // Bandera para indicar que no hay más tareas

// Convierte las palabras a minúsculas
string convertirMinusculas(string palabra) {
    string palabra_minus = palabra;
    transform(palabra_minus.begin(), palabra_minus.end(), palabra_minus.begin(), ::tolower);
    return palabra_minus;
}

// Elimina caracteres de una palabra
string eliminarCaracteres(string palabra) {
    palabra.erase(remove_if(palabra.begin(), palabra.end(), [](char c) {
        return !isalpha(c);
    }), palabra.end());
    return palabra;
}

// Función que cuenta las palabras
void contarPalabras(const string& ruta_archivo, const string& pathOut, int thread_id) {
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada: " << ruta_archivo << endl;
        return;
    }

    map<string, int> contador_palabras;
    string linea, palabra;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        while (ss >> palabra) {
            palabra = eliminarCaracteres(palabra);
            palabra = convertirMinusculas(palabra);
            if (!palabra.empty()) {
                contador_palabras[palabra]++;
            }
        }
    }
    archivo.close();

    {
        lock_guard<mutex> lock(mtx);
        ofstream archivo_salida(pathOut);
        if (!archivo_salida.is_open()) {
            cerr << "No se pudo crear el archivo de salida: " << pathOut << endl;
            return;
        }

        for (auto [palabra, cantidad] : contador_palabras) {
            archivo_salida << palabra << " ; " << cantidad << endl;
        }
    }

    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Thread " << thread_id << ": Libro " << ruta_archivo << " procesado con éxito!" << endl;
        cout << "Archivo de salida: " << pathOut << endl;
        cout << "Cantidad de palabras distintas: " << contador_palabras.size() << endl << endl;
    }
}

// Función que lee los archivos de un directorio con una extensión específica
vector<string> lee_archivos(const string& ruta_input, const string& file_type) {
    vector<string> files;
    for (const auto& entry : filesystem::directory_iterator(ruta_input)) {
        if (entry.path().extension() == file_type) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// Función que maneja la creación de hilos y la asignación de tareas
int procesarConThreads(const string& ruta_input, const string& ruta_output, const string& file_type, int num_threads) {
    vector<string> files = lee_archivos(ruta_input, file_type);
    vector<thread> threads;

    // Agregar los archivos a la cola de trabajo
    for (const string& file : files) {
        work_queue.push(file);
    }

    // Crear hilos
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, i]() {
            while (true) {
                string file;
                {
                    lock_guard<mutex> lock(mtx);
                    if (!work_queue.empty()) {
                        file = work_queue.front();
                        work_queue.pop();
                        cout_mutex.lock();
                        cout << "Thread " << i << ": Desocupado y procesando el archivo " << file << endl;
                        cout_mutex.unlock();
                    } else {
                        cout_mutex.lock();
                        cout << "Thread " << i << ": Esperando por nuevos archivos..." << endl;
                        cout_mutex.unlock();
                        break;  // No hay más archivos para procesar.
                    }
                }
                // Procesar el archivo
                contarPalabras(file, ruta_output + "/" + filesystem::path(file).filename().string(), i);
            }
        });
    }

    // Unirse a todos los hilos
    for (auto& thread : threads) {
        thread.join();
    }

    return EXIT_SUCCESS;
}

int main() {
    int opcion3;
    //string pathIn = "/home/esperanza/Escritorio/UACh/Sem6/SistemasOperativos/Trabajo/SistemasOperativos/libros";
    //string pathOut = "/home/esperanza/Escritorio/UACh/Sem6/SistemasOperativos/Trabajo/SistemasOperativos/prueba";
    string pathIn = "/home/rudy/2024/SO/SistemasOperativos/libros";
    string pathOut = "/home/rudy/2024/SO/SistemasOperativos/prueba";
    string extension = ".txt"; // Puedes modificar la extensión según tus archivos
    int cantidad_threads = 4; // Por defecto, 4 threads

    pid_t pid = getpid();
    cout << "\nPrograma contador de palabras" << endl;
    cout << "PID: " << pid << endl;

    do { 
        cout << "Para procesar presione 1: ";
        cin >> opcion3;

        switch (opcion3) {
            case 1: {
                cout << "Procesando con " << cantidad_threads << " threads..." << endl;
                procesarConThreads(pathIn, pathOut, extension, cantidad_threads);
                cout << "Conteo hecho con éxito" << endl;
                break;
            }
            default: {
                system("clear");
                cout << "\033[31mOpción inválida. Intente de nuevo.\033[0m" << endl;
                break;
            }
        }
    } while (opcion3 != 1);
    
    return 0;
}
