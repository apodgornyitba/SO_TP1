# Sistemas Operativos - TP1: Inter Process Communication
## Autores
    ● 60299 - Juan Pablo Arias
    ● 60570 - Andrés Podgorny
    ● 61278 - Sol Victoria Anselmo
## Introducción
El trabajo práctico consiste en aprender a utilizar los distintos tipos de IPCs presentes en un sistema POSIX. Para ello se implementará un sistema que distribuirá tareas de SAT solving entre varios procesos. Desarrollar un sistema en C que resuelva múltiples fórmulas proposicionales en CNF de forma distribuida. Para ello cuenta con procesos aplicación, vista, y esclavos
## Entorno de desarrollo y ejecución
Es un requisito obligatorio tanto para el desarrollo y compilación como para la ejecución del sistema, utilizar la imagen provista por la cátedra:

    docker pull agodio/itba-so:1.0
          
## Instrucciones de Compilación
Ubicado dentro del directorio donde se descargaron los archivos, ejecutar el comando:

    $ make all
    
Esto generará todos los archivos ejecutables necesarios.
## Instrucciones de Ejecución
El programa master guardará todos los resultados en un archivo, sin necesidad de otro proceso, para ello, ejecute:

    $ ./app ./files/* 

donde ./files/* representa un file path. Adicionalmente, se puede ejecutar el proceso view, que se encargará de imprimir por salida estándar los resultados, de la siguiente manera:

    $ ./view <memorySize>
 
donde el valor <memorySize> será indicado por salida estándar al ejecutar el proceso master. Para ello, se cuenta con 2 segundos de espera. Para iniciar ambos procesos a la vez, ejecutar lo siguiente:

    $ ./app ./files/* | ./view

## Instrucciones de Limpieza
Ubicado dentro del directorio donde se descargaron los archivos, para borrar todos los archivos ejecutables generados y el archivo resultado ejecutar el comando:
    
    $ make clean
    
