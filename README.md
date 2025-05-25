# GraphQuest

# Descripcion del Juego
GraphQuest es un juego en el cual consiste en recorrer un laberinto con distintos escenarios con un tiempo de 10. Cada escenario tendra sus propias caracteristicas es decir. Nombre, descripción e ítems que podamos agarrar.
Cada ítem tendrá su propio peso y valor. Nuestro jugador tendrá un peso límite de 10kg. Mientras mas peso tengamos mas tiempo nos costará movernos por los distintos escenarios. Por lo tanto nosotros tendremos que intentar completar 
el laberinto evitando que nos quedemos sin tiempo y con el mayor puntaje posible respetando nuestro peso maximo. 

# Como ejecutar para utilizar el juego
Este programa ha sido desarrollado en el lenguaje C y puede ejecutarse utilizando *Visual Studio Code* junto con una extensión para C/C++, como la *Extension pack de Microsoft* que se descarga del mismo Visual Studio Code
Para poder ejecutarlo siga los siguientes pasos.

# Requisitos Previos
1. Tener instalado Visual Studio Code
2. Tener la extención C/C++
3. Tener instalado un compilador de C, como por ejemplo *MinGW*


# Pasos para ejecutar

1. Descarga el archivo .zip y descomprime este en un directorio a elección
2. Abre el código en Visual Studio Code
Para esto:
- Abre la aplicacion
- Archivo -> Abrir Carpeta
- Y seleccionamos la carpeta donde descomprimimos el programa

3. Para compilar el codigo
- Crea una nueva terminal, en Terminal -> Nueva Terminal
- En la terminal coloca este comando: **gcc -o graphquest tarea3.c tdas/list.c tdas/extra.c**

(Esto lo que hará será crear un Archivo.Exe llamado graphquest para ejecutar el programa)
Luego ingresa a la carpeta donde hayas descomprimido el programa y ejecuta el .Exe

# Funcionalidades
- Recoger items
- Descartar items
- Movernos por distintos escenarios
- Reiniciar Partida

# Problemas Frecuentes
- Si al programa se le ingresan letras o carácteres especiales este falla.

# Posibles Mejoras
- Actualizar este para que no falle el programa cuando se le ingresen cáracteres donde no se deba
- Poder regresar al menú principal sin perder nuestro progreso. Añadiendo un menú para poder continuar partida.

# Ejemplo de Uso

![image](https://github.com/user-attachments/assets/f274347d-aa99-4977-a8ea-b1f4e03f4008)

Este sera el menú inicial, ingresando **SOLO VALORES NUMERICOS**, decediremos iniciar la partida o salirnos de esta y cerrar el juego.

# Al iniciar la Partida

![image](https://github.com/user-attachments/assets/6f3f7e3e-49c1-48eb-a794-d3e944741fc3)

Nos aparecera un menú como este, además de mostrarnos el estado inicial del jugador.

**Opcion 1. Recoger Items**

![image](https://github.com/user-attachments/assets/4d6cbc90-da55-4c29-8458-0214d8f7960c)

Nos aparecerá un menú como ese, el cual ingresando los **valores numericos** que nos aparecen podremos agregar el item a nuestro inventario.
Al recoger un ítem se descontará 1 de nuestro tiempo acutal.

**Opcion 2. Descartar Items**

![image](https://github.com/user-attachments/assets/27dfcc69-60c7-4af6-8a50-cb78a5e00582)

Al momento de descartar ítems el programa nos advertirá que perderemos el objeto de nuestro inventario completamente y no podremos recuperarlo.
Ingresaremos el **valor numerico** correspondiente al objeto que querramos descartar.
Al descartar un ítem se descontará 1 de nuestro tiempo acutal.

**Opcion 3. Moverse**

![image](https://github.com/user-attachments/assets/85a5c3dc-c525-4424-9d97-bbffe881d2d5)

Nos aparecera un menú como este donde deberemos ingresar un **valor numerico** correspondiente al movimiento que querramos realizar, este menu se irá actualizando en base a los escenarios en los que estemos
y los movimientos disponibles a realizar que tengamos.

**Opcion 4. Reiniciar Partida**

Al seleccionar esta opción el juego se reiniciará por completo es decir, restableciendo nuestro tiempo a 10, además de eliminar las cosas de nuestro inventario y puntaje, y regresaremos al escenario inicial.


**Opción 5. Volver al menú principal** 

Si seleccionamos esta opción se finalizará el juego perdiendo todo nuestro avance. Regresaremos al menú mostrado al inicio donde podremos elegir si iniciar nuevamente partida o salirnos completamente del programa.
