import matplotlib.pyplot as plt
import os

def leer_datos(archivo):
    datos = {}
    with open(archivo, 'r') as file:
        for linea in file:
            thread, tiempo = map(float, linea.split())
            if thread not in datos:
                datos[thread] = []
            datos[thread].append(tiempo)
    return datos

#Función para generar el gráfico
def graficar(datos, repeticiones, grafico_path):
    threads = sorted(datos.keys())  # Ordenar los threads por número
    for i in range(repeticiones):
        tiempos = [datos[thread][i] for thread in threads]
        plt.plot(threads, tiempos, label=f"Repetición {i+1}")

    plt.title('Tiempo de ejecución vs. Número de Threads')
    plt.xlabel('Número de Threads')
    plt.ylabel('Tiempo de Ejecución (segundos)')
    plt.legend()
    
    #Guardar el gráfico
    plt.savefig(grafico_path)

if __name__ == "__main__":
    #Leer las variables de entorno
    repeticiones = int(os.environ.get('REPETICIONES'))
    grafico_path = os.environ.get('GRAFICO')
    datos = leer_datos(os.environ.get('DATOS'))

    #Verificaciones
    if repeticiones is None:
        raise ValueError("La variable de entorno 'REPETICIONES' no está definida.")
    if grafico_path is None:
        raise ValueError("La variable de entorno 'GRAFICO' no está definida.")
    if datos_path is None:
        raise ValueError("La variable de entorno 'DATOS' no está definida.")
    if not os.path.isfile(datos_path):
        raise FileNotFoundError(f"El archivo de datos '{datos_path}' no existe.")
    
    #Validamos la ruta del gráfico
    grafico_dir = os.path.dirname(grafico_path)
    if grafico_dir and not os.path.exists(grafico_dir):
        raise FileNotFoundError(f"La carpeta para guardar el gráfico '{grafico_dir}' no existe.")

    #Generamos el gráfico
    graficar(datos, repeticiones, grafico_path)
