#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <poppler-document.h>
#include <poppler-page.h>

using namespace std;

// Función para leer las "stop words" desde un archivo y guardarlas en un diccionario
unordered_set<string> cargarStopWords(const string& stopWordsFile) {
    unordered_set<string> stopWords;
    ifstream file(stopWordsFile);
    string palabra;

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de stop words." << endl;
        return stopWords;
    }

    while (file >> palabra) {
        stopWords.insert(palabra);
    }

    file.close();
    return stopWords;
}

// Función para leer texto desde un archivo PDF
string leerPDF(const string& pdfFile) {
    string texto;
    poppler::document* doc = poppler::document::load_from_file(pdfFile);
    if (!doc) {
        cerr << "Error: No se pudo abrir el archivo PDF." << endl;
        return texto;
    }

    for (int i = 0; i < doc->pages(); i++) {
        poppler::page* pagina = doc->page(i);
        if (pagina) {
            texto += pagina->text().to_utf8() + "\n"; // Agregar el texto de cada página
        }
    }

    return texto;
}

// Función para procesar el texto, eliminando las "stop words"
void borrarStopWords(const string& texto, const string& outputFile, const unordered_set<string>& stopWords) {
    ofstream outFile(outputFile);
    
    if (!outFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de salida." << endl;
        return;
    }

    stringstream ss(texto);
    string palabra;

    // Filtramos las palabras, eliminando las stop words
    while (ss >> palabra) {
        if (stopWords.find(palabra) == stopWords.end()) { // Si no es una stop word
            outFile << palabra << " "; // Escribimos la palabra en el archivo de salida
        }
    }

    outFile.close();
}

int main() {
    // Define tus archivos de entrada y salida
    string stopWordsFile = "stop_word.txt"; // Archivo que contiene las stop words
    string inputFile = "libros/dracula.pdf"; // Archivo de entrada (PDF)
    string outputFile = "libros/output.txt"; // Archivo de salida

    // Cargar las stop words
    unordered_set<string> stopWords = cargarStopWords(stopWordsFile);

    // Leer el contenido del archivo PDF
    string texto = leerPDF(inputFile);

    // Procesar el texto y eliminar las stop words
    borrarStopWords(texto, outputFile, stopWords);

    cout << "Proceso completado. Archivo procesado guardado como: " << outputFile << endl;

    return 0;
}
