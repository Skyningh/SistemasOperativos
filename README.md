# Trabajo 1
## Descripcion
Este programa es una interfaz que realiza diversas funciones desde los parámetros ingresados en consola. Puede además, administrar los usuarios registrados en la base de datos, realizar múltiples conteos simultáneos de palabras en archivos de texto grandes, con el uso de threads, y genera estadísticas de los tiempos de ejecución. Y por último, se usa como una simulación de como trabajan los core en el computador, con un planificador que organiza los procesos, un distribuidor que decodifica los mensajes y escribe la respuesta, y los core que hacen las operaciones.


## Requisitos
- Un ordenador con Linux, macOS o Windows
- El compilador g++ instalado
- Lirerias standard C++


## Instalacion / Compilacion
No se debe instalar nada. Para compilar, sólo se debe escribir "make" en el terminal en la carpeta donde están los archivos.

## Ejecucion
Después de compilar, se debe ejecutar como ./trabajo1 -u usuario -p contraseña -t texto entre comillas -v vector de la forma "1,2,3,4" (entre comillas) -n un número entero.

| Argumento | Descripcion |
| -------- | ----------- |
| u | Especifica el usuario |
| p | Especifica la contraseña del usuario |
| t | Un texto |
| v | Un vector de números enteros |
| n | Un número entero |

## Variables de entorno
Se utilizan variables de entorno a lo largo del proyecto para poder facilitar el uso en cada computador personal

PATHBD=./complementos/users_bd.txt
pathIn=./libros
pathOut=./conteo_output
stop_word=./complementos/stop_word.txt
cantidad_thread=5
mapa_archivos=./complementos/mapa.txt -- El archivo donde se guarda el mapa de los libros, con su id respectivo
inverted_index=./complementos/indiceinv.txt -- El archivo donde se guarda el indice invertido
PROCESOS=./complementos/procesosS.txt -- El archivos de proceso de tamaño pequeño
PROCESOSM=./complementos/procesosM.txt -- El archivo de procesos de tamaño mediano
PROCESOSL=./complementos/procesosL.txt -- El archivo de procesos grande
CANTIDAD_CORES=3 -- Son la cantidad de cores que se simularan en la opcion 13
RESULTADOS=./complementos/resultados.txt -- Es el archivo donde se guardaran los resultados de la opcion 13
ESTADO_CORES=./complementos/cores -- Es la carpeta donde estan los archivos de el estado de cada core
ARRAY_THREADS=1,2,4,8,16 -- El arreglo de los threads a utilizar para el conteo de palabras en paralelo
REPETICIONES=2 -- Cantidad de repeticiones de cada iteración de conteo en cada valor del arreglo thread
DATOS=./complementos/datos.txt -- Archivo de salida para los tiempos de ejecución de conteo thread
GRAFICO=./complementos/grafico.png -- Imagen png del gráfico tiempo vs cantidad de threads

## Funciones

### Lógica general del programa (`funciones.cpp`)

Estas funciones gestionan la lógica principal del programa:

- **`Usuario procesarUsuario(string usuario, string password, string file)`**  
  Crea un usuario con los parámetros ingresados y realiza las verificaciones necesarias.

- **`string palindromo(string a)`**  
  Verifica si el texto ingresado es un palíndromo.

- **`int vocales(string a)`**  
  Retorna el número de vocales en el texto ingresado.

- **`int letras(string a)`**  
  Cuenta la cantidad de letras en el texto ingresado.

- **`int sum(vector<int> vec)`**  
  Retorna la suma de los números en el vector ingresado.

- **`float prom(vector<int> vec)`**  
  Calcula y retorna el promedio de los números en el vector ingresado.

- **`vector<int> procesaVector(string a)`**  
  Convierte una cadena de texto en un vector de enteros.

- **`tuple<string, string, string> leerEnv()`**  
  Lee las variables de entorno del archivo `.env`.

- **`void eliminarUsuarios(string usuario, string contraseña, string file)`**  
  Elimina usuarios de la base de datos.

- **`void crearUsuario(string username, string password, string tipo, string file)`**  
  Crea un nuevo usuario y lo guarda en la base de datos.

- **`void listarUsuarios(string file)`**  
  Lista todos los usuarios almacenados en la base de datos.

- **`int verificarUser(string username, string password, string tipo, string file)`**  
  Verifica si un usuario cumple con las condiciones para ser creado.

### Interfaz del programa (`interfaz.cpp`)

Estas funciones manejan la interfaz del usuario:

- **`int contarmenu()`**  
  Menú para la función de contar palabras.

- **`int subMenu()`**  
  Submenú para volver atrás.

- **`int menu(Usuario usuario, string frase, vector<int> vec, float num, string path)`**  
  Menú principal del programa.

### Procesamiento de archivos de texto (`procesar.cpp`)

Estas funciones procesan los archivos de texto:

- **`string convertirMinusculas(string palabra)`**  
  Convierte las palabras a minúsculas.

- **`string eliminarPuntuacion(string palabra)`**  
  Elimina la puntuación de una palabra.

- **`void contarPalabras(string ruta_archivo, string pathOut)`**  
  Procesa los archivos y cuenta las palabras.

- **`int procesar(string ext, string pathIn, string pathOut)`**  
  Aplica la función de contar palabras a todos los archivos en la carpeta de entrada.

### Conteo paralelo con threads (`conteo_thread.cpp`)

Estas funciones manejan el conteo paralelo con threads:

- **`string convertirMinusculas(string palabra)`**  
  Convierte las palabras a minisculas.

- **`string eliminarCaracteres(string palabra)`**
  Elimina caracteres no alfabéticos de una palabra.

- **`unordered_set<string> cargarStopWords(const string& stopWordsFile)`**
  Carga las stop words desde un archivo.

- **`string borrarStopWords(const string& inputFile)`**
  Elimina las stop words de un archivo.

- **`void contarPalabras(const string& ruta_archivo, const string& pathOut, int thread_id)`**
  Cuenta las palabras de los archivos

- **`vector<string> lee_archivos(const string& ruta_input, const string& file_type)`**
  Lee los archivos de un directorio con una extensión especifica

- **`void mapeoArchivos()`**
  Hace un archivo map que tiene nombrelibro,id

- **`int procesarConThreads(const string& ruta_input, const string& ruta_output, const string& file_type, int num_threads)`**
  Maneja la creación de hilos y la asignación de tareas.

### Creación del inverted index (`inverted_index.cpp`)

Estas funciones crean el inverted index:

- **`void procesarArchivo(int id, string archivo)`**
  Procesa cada archivo y extrae sus palabras y cantidad de veces que se ocupan.

- **`void creariIndex(string iIndex, string pathOut)`**
  Crea el indice invertido a partir de los archivos en pathOut

### Planificador (`planificador.cpp`)

  En el planificador se realiza la organizacion antes de ejecutar los cores con estas funciones:

- **`vector<string> split2(string str, char separador)`**
  Splitea un str a un vector, especifico para la situacion de planificador

- **`int selectCore(vector<int> Cores)`**
  Selecciona el core libre a ejecutar

### Distribuidor (`distribuidor.cpp`)

  En el distribuidor solo se interpreta el mensaje del planificador y se espera el resultado del core, solo se utiliza un main.

### Core (`core.cpp`)

  En el core se simula un core de un procesador, tiene estas funciones a ejecutar dependiendo de la operacion:

- **`float suma(float n1, float n2)`**
  Se suma el n1 y el n2

- **`float resta(float n1, float n2)`**
  Se resta el n1 por el n2

- **`float multiplicacion(float n1, float n2)`**
  Se multiplica el n1 con el n2

- **`float division(float n1, float n2)`**
  Verifica si el divisor es 0, si es asi devuelve 0, sino se divide normal

### Ejecución del programa de conteo threads y medición de tiempos (`ejecutador.cpp`)

Este archivo maneja la ejecución del conteo de palabras en paralelo y registra el tiempo de ejecución para distintas configuraciones de threads. Las funciones principales incluyen:

- **`vector<int> procesarVector(const char* input)`**  
  Convierte una cadena de caracteres en un vector de enteros, separando los valores por comas para representar la lista de threads a probar.

- **`int main(int argc, char* argv[])`**  
  Configura el programa tomando como argumentos la lista de threads a utilizar y la cantidad de repeticiones deseadas. Ejecuta el conteo de palabras para cada configuración de threads, registra el tiempo de cada ejecución en el archivo especificado por la variable de entorno `DATOS`, y luego llama al script `analizador.py` para la generación del gráfico de rendimiento.

### Análisis y generación de gráficos (`analizador.py`)

Este script en Python lee los datos de tiempos de ejecución y crea un gráfico que muestra cómo varía el rendimiento con el número de threads. Las funciones principales incluyen:

- **`leer_datos(archivo)`**  
  Lee y organiza los datos desde el archivo `datos.txt`, ordenando los tiempos de ejecución por cantidad de threads.

- **`graficar(datos, repeticiones, grafico_path)`**  
  Genera un gráfico de líneas que representa el tiempo de ejecución en función del número de threads. Cada repetición se representa en una línea distinta, y el gráfico resultante se guarda en la ruta especificada en la variable de entorno `GRAFICO`. 

Estos archivos permiten un análisis detallado de la eficiencia del programa en función de los threads, identificando configuraciones óptimas para maximizar el rendimiento.






