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
#include <queue>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex mtx; //Mutex -> Exclusión mutua (Sirve para bloquear un recurso para que solo un thread puede acceder a este en un momento dado.)
condition_variable cv; //Variable de condición para sincronización
queue<pair<string, string>> tareas; //Cola de tareas

//Convierte las palabras en solo minúsculas
string convertirMinusculas(string palabra) {
    string palabra_minus = palabra;
    transform(palabra_minus.begin(), palabra_minus.end(), palabra_minus.begin(), ::tolower);
    return palabra_minus;
}

//Elimina caracteres de una palabra
string eliminarCaracteres(string palabra) {
    palabra.erase(remove_if(palabra.begin(), palabra.end(), [](char c) {
        return !isalpha(c);
    }), palabra.end());
    return palabra;
}

//Función que procesa los archivos y cuenta las palabras
void contarPalabras(string ruta_archivo, string pathOut) {
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada: " << ruta_archivo << endl;
        return;
    }

    //Mapa que almacena la cantidad de cada palabra
    map<string, int> contador_palabras;
    string linea, palabra;

    //Lectura de archivo
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        //Extrae cada palabra de la línea
        while (ss >> palabra) {
            palabra = eliminarCaracteres(palabra);
            palabra = convertirMinusculas(palabra);
            if (!palabra.empty()) {
                contador_palabras[palabra]++;
            }
        }
    }
    archivo.close();

    //Aquí bloqueamos el mutex
    mtx.lock();
    ofstream archivo_salida(pathOut);
    if (!archivo_salida.is_open()) {
        cerr << "No se pudo crear el archivo de salida: " << pathOut << endl;
        mtx.unlock();
        return;
    }

    // Escribe resultado en output
    for (auto [palabra, cantidad] : contador_palabras) {
        archivo_salida << palabra << " ; " << cantidad << endl;
    }
    archivo_salida.close();
    mtx.unlock();

    //Imprime path del archivo y la cantidad de palabras distintas (también protegido)
    mtx.lock();
    cout<< "Libro "<< ruta_archivo<<"procesado con éxito!"<<endl;
    cout << "Archivo de salida: " << pathOut << endl;
    cout << "Cantidad de palabras distintas: " << contador_palabras.size() << endl<<endl;
    mtx.unlock();
}

void worker() { //Esta funcion gestiona la cola de trabajo, junto a los hilos
    while (true) {
        pair<string, string> tarea;
        
        {
            // Bloquear hasta que haya tareas disponibles
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] { return !tareas.empty(); });

            // Obtener la tarea de la cola
            tarea = tareas.front();
            tareas.pop();
        }

        // Procesar la tarea
        contarPalabras(tarea.first, tarea.second);
    }
}

// Función que se encargará de manejar los threads y controlar el número de threads activos
int procesarConThreads(string ext, string pathIn, string pathOut, int cantidad_threads) {
    vector<thread> threads;

    // Crear hilos del pool
    for (int i = 0; i < cantidad_threads; ++i) {
        threads.emplace_back(worker);
    }

    // Iterar sobre los archivos en el directorio
    for (auto& entry : filesystem::directory_iterator(pathIn)) {
        string nombre_archivo = entry.path().filename().string();
        string pathArchivoSalida = pathOut + "/" + nombre_archivo;

        // Agregar la tarea a la cola
        {
            lock_guard<mutex> lock(mtx);
            tareas.emplace(entry.path().string(), pathArchivoSalida);
        }
        cv.notify_one(); // Notificar a un hilo que hay una nueva tarea
    }

    // Terminar el trabajo cuando no haya más tareas
    for (int i = 0; i < cantidad_threads; ++i) {
        {
            lock_guard<mutex> lock(mtx);
            tareas.emplace("", ""); // Se añade una señal de terminación
        }
        cv.notify_one();
    }

    // Unirse a todos los hilos
    for (auto& t : threads) {
        t.join();
    }
    return 1;
}

int main() {
    int opcion3;
    string pathIn = "/home/esperanza/Escritorio/UACh/Sem6/SistemasOperativos/Trabajo/SistemasOperativos/libros" ;
    string pathOut= "/home/esperanza/Escritorio/UACh/Sem6/SistemasOperativos/Trabajo/SistemasOperativos/prueba";
    string extension;
    bool check1 = false, check2 = false, check3 = false;
 
    // Leer la cantidad de threads permitidos desde la variable de entorno
    int cantidad_threads = 4; // Por defecto, 4 threads

    pid_t pid = getpid();
    cout << "\nPrograma contador de palabras" << endl;
    cout << "PID: " << pid << endl;
    
    do { 
        cout << "Para procesar presione 1";
        cin >> opcion3;

        switch (opcion3) {
            case 1: {
                cout << "Procesando con " << cantidad_threads << " threads..." << endl;
                if (procesarConThreads(extension, pathIn, pathOut, cantidad_threads)==1)
                    cout<< "Conteo hecho con éxito"<<endl;
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