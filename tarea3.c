#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxescenarios 17

typedef struct 
{
  char *nombre;
  int valor;
  int peso;
} item;


typedef struct
{
  int arriba;
  int abajo;
  int izquierda;
  int derecha;
} direcciones;

typedef struct 
{
  char *nombre;
  int id;
  char *descripcion;
  List *items;
  direcciones enlaces;
  int es_final;
} escenario;

escenario escenarios[maxescenarios];

typedef struct
{
  float tiempo_restante;
  int puntajeTotal;
  int pesoTotal;
  List *items;
  escenario *escenario_actual;
} player;

escenario* escenariosFull[maxescenarios];

/*Esta funcion se encarga de recibir el nombre, valor y peso del  item para luego asignarle memoria y posteriormente asignarlo a su estructura correspondiente y retornarlo.*/

item *crearItem(char *nombre, int valor, int peso)
{
  item *nuevo_item = malloc(sizeof(item));
  nuevo_item->nombre = strdup(nombre);
  nuevo_item->valor = valor;
  nuevo_item->peso = peso;
  return nuevo_item;

}

/* Lo que realiza esta funcion es abrir el archivo CSV con todos sus campos para luego asignarle a las estructuras cada informacion de los escenarios, es decir su id, nombre, descripcion,
items y si es el final. Le asignamos memoria a traves de un Malloc a cada escenario. Ademas utilizamos la funcion crear item para encargarnos de que cada item tenga su nombre, valor y peso.
Ademas asignamos los movimientos a cada nuevo escenario.*/

void leer_escenarios() 
{
  
  FILE *archivo = fopen("graphquest.csv", "r");
  if (archivo == NULL) 
  {
    perror(
        "Error al abrir el archivo"); 
    return;
  }

  char **campos;
  fgets((char[256]){0}, 256, archivo); 
  
  while ((campos = leer_linea_csv(archivo, ',')) != NULL)
  {
    int id = atoi(campos[0]);
    escenario *nuevo_escenario = malloc(sizeof(escenario));
    nuevo_escenario->id = id;
    nuevo_escenario->nombre = strdup(campos[1]);
    nuevo_escenario->descripcion = strdup(campos[2]);
    nuevo_escenario->items = list_create();
    nuevo_escenario->es_final = (strcmp(campos[8], "Si") == 0) ? 1 : 0; 

    List *items = split_string(campos[3], ";");
    for (char *item_str = list_first(items); item_str != NULL; item_str = list_next(items))
    {
      List* valores = split_string(item_str, ",");
      char *nombre_item = strdup(list_first(valores));
      int valor = atoi(list_next(valores));
      int peso = atoi(list_next(valores));
      item *nuevo_item = crearItem(nombre_item, valor, peso);
      list_pushBack(nuevo_escenario->items, nuevo_item);
      list_clean(valores);
      free(valores);
      free(nombre_item);
  
    }
    list_clean(items);
    free(items);
    nuevo_escenario->enlaces.arriba = atoi(campos[4]);
    nuevo_escenario->enlaces.abajo = atoi(campos[5]);
    nuevo_escenario->enlaces.izquierda = atoi(campos[6]);
    nuevo_escenario->enlaces.derecha = atoi(campos[7]);
    escenariosFull[id] = nuevo_escenario;
    free(campos);
  }
  fclose(archivo);
  printf("Escenarios leidos correctamente.\n");
}


/*Esta funcion se encarga de mostrar el estado actual despues de cada accion o movimiento que realice el jugador. Nos muestra el escenario en el cual estamos, la descripcion de este,
Los items disponibles en este escenario, nuestra lista de items y el tiempo restante que tenemos, ademas de los movimientos que tenemos disponibles para realizar.*/
void mostrarEstadoActual(player *jugador)
{
  escenario *ubicacion = jugador->escenario_actual;
  if (ubicacion->es_final)
  {
    printf("Has llegado al escenario final: %s\n", ubicacion->nombre);
    return;
  }
  if (jugador->tiempo_restante <= 0)
  {
    printf("Se ha acabado el tiempo!\n");
    return;
  }
  
  printf("====================================\n");
  printf("Escenario actual: %s\n", ubicacion->nombre);
  printf("Descripcion: %s\n", ubicacion->descripcion);

  printf("Items disponibles:\n");
  for (item *i = list_first(ubicacion->items); i != NULL; i = list_next(ubicacion->items))
  {
    printf("Nombre: %s, Valor: %d, Peso: %d\n", i->nombre, i->valor, i->peso);
  }
  printf("\n");
  printf("Tiempo restante: %.2f\n", jugador->tiempo_restante);

  printf("Inventario actual : (peso total: %d kg, puntaje total: %d)\n", jugador->pesoTotal, jugador->puntajeTotal);

  if (jugador->items != NULL)
  {
    for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items))
    {
      printf("Nombre: %s, Valor: %d, Peso: %d\n", i->nombre, i->valor, i->peso);
    }
  }
  else
  {
    printf("No hay items en el inventario.\n");
  }
  printf("\n");


  printf("Movimientos disponibles:\n");
  if (ubicacion->enlaces.arriba != -1)
    printf("Arriba: %s\n", escenariosFull[ubicacion->enlaces.arriba]->nombre);
  if (ubicacion->enlaces.abajo != -1)
    printf("Abajo: %s\n", escenariosFull[ubicacion->enlaces.abajo]->nombre);
  if (ubicacion->enlaces.izquierda != -1)
    printf("Izquierda: %s\n", escenariosFull[ubicacion->enlaces.izquierda]->nombre);
  if (ubicacion->enlaces.derecha != -1)
    printf("Derecha: %s\n", escenariosFull[ubicacion->enlaces.derecha]->nombre);
  printf("\n");
}


/*Esta funcion se encarga de mostrarnos los items disponibles para recoger, si es que no llega a haber nos muestra un mensaje avisandonos, luego nos solicita elegir el item que querramos 
recoger, para luego que este se agregue a nuestro inventario. Ademas nos advertira si excedemos el peso limite del jugador y no recogera el item.*/
void recogerItems(player *jugador) {
  escenario *ubicacionActual = jugador->escenario_actual;
  if (ubicacionActual->items == NULL || list_size(ubicacionActual->items) == 0) {
    printf("No hay items disponibles en este escenario.\n");
    return;
  }

  int opcion = -1;
  do {
    printf("\nItems disponibles para recoger:\n");
    int index = 1;
    List *auxiliar = list_create();  

    for (item *i = list_first(ubicacionActual->items); i != NULL; i = list_next(ubicacionActual->items)) {
      printf("%d. Nombre: %s, Valor: %d, Peso: %d\n", index, i->nombre, i->valor, i->peso);
      list_pushBack(auxiliar, i);
      index++;
    }

    printf("\nSeleccione el numero del item que desea recoger (0 para salir): ");
    scanf("%d", &opcion);

    if (opcion > 0 && opcion <= list_size(auxiliar)) {
      item *itemRecogido = NULL;
      int pos = 1;
      for (item *i = list_first(auxiliar); i != NULL; i = list_next(auxiliar)) {
        if (pos == opcion) {
          itemRecogido = i;
          break;
        }
        pos++;
      }

      if (itemRecogido != NULL) {
        if (jugador->pesoTotal + itemRecogido->peso <= 10) {
          jugador->pesoTotal += itemRecogido->peso;
          jugador->puntajeTotal += itemRecogido->valor;
          jugador->tiempo_restante -= 1.0;

          
          list_pushBack(jugador->items, itemRecogido);

          
          List *nuevaLista = list_create();
          for (item *i = list_first(ubicacionActual->items); i != NULL; i = list_next(ubicacionActual->items)) {
            if (i != itemRecogido)
              list_pushBack(nuevaLista, i);
          }
          list_clean(ubicacionActual->items);
          free(ubicacionActual->items);
          ubicacionActual->items = nuevaLista;

          printf("Item '%s' recogido exitosamente.\n", itemRecogido->nombre);
        } else {
          printf("No puedes recoger el ítem '%s'. Peso total excede el limite.\n", itemRecogido->nombre);
        }
      }
    } else if (opcion != 0) {
      printf("Opcion invalida. Intente nuevamente.\n");
    }
    system("cls");
    list_clean(auxiliar);
    free(auxiliar);

  } while (opcion != 0);
}


/*Esta funcion se encarga de eliminar items de nuestro inventario, la funcion nos advierte que al descartar el item no podremos volver a recogerlo, nos preguntara cual de todos los items
que tendremos en el inventario queremos descartar, luego de seleccionar el item que eliminaremos, esta funcion lo descartara de nuestro inventario, reduciendonos el puntaje y peso
correspondieste de este, ademas de disminuirnos el tiempo por realizar la accion.*/
void descartarItems(player *jugador) {
  if (jugador->items == NULL || list_size(jugador->items) == 0) {
    printf("No hay items en el inventario.\n");
    return;
  }

  puts("ADVERTENCIA: Al descartar un item, no podras volver a recogerlo.");

  int opcion = -1;
  do {
    printf("\nItems en el inventario:\n");
    int index = 1;
    List *referencias = list_create();

    for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items)) {
      printf("%d. Nombre: %s, Valor: %d, Peso: %d\n", index, i->nombre, i->valor, i->peso);
      list_pushBack(referencias, i);
      index++;
    }

    printf("\nSeleccione el numero del item que desea descartar (0 para salir): ");
    scanf("%d", &opcion);

    if (opcion > 0 && opcion <= list_size(referencias)) {
      item *itemDescartado = NULL;
      int pos = 1;
      for (item *i = list_first(referencias); i != NULL; i = list_next(referencias)) {
        if (pos == opcion) {
          itemDescartado = i;
          break;
        }
        pos++;
      }

      if (itemDescartado != NULL) {
        jugador->pesoTotal -= itemDescartado->peso;
        jugador->puntajeTotal -= itemDescartado->valor;
        jugador->tiempo_restante -= 1.0;

        
        List *nuevaLista = list_create();
        for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items)) {
          if (i != itemDescartado)
            list_pushBack(nuevaLista, i);
          else {
            free(i->nombre);
            free(i);
          }
        }
        list_clean(jugador->items);
        free(jugador->items);
        jugador->items = nuevaLista;
        
        printf("Item descartado correctamente.\n");
      }
    } else if (opcion != 0) 
    {
      
      printf("Opcion invalida. Intente nuevamente.\n");
    }
    system("cls");
    list_clean(referencias);
    free(referencias);

  } while (opcion != 0);
}



/*Esta funcion se encarga de mostrarnos en que direcciones podremos desplazarnos en base al escenario que estamos con nuestro jugador, ademas nos informara hacia que escenario
nos desplazamos ademas de informarnos el tiempo utilizado y el tiempo restante que nos queda.*/
void moverJugador(player *jugador)
{
  escenario *ubicacionActual = jugador->escenario_actual;
  
  printf("Seleccione la direccion a la que desea moverse:\n");

  if (ubicacionActual->enlaces.arriba != -1)
    printf("1. Arriba: %s\n", escenariosFull[ubicacionActual->enlaces.arriba]->nombre);
  if (ubicacionActual->enlaces.abajo != -1)
    printf("2. Abajo: %s\n", escenariosFull[ubicacionActual->enlaces.abajo]->nombre);
  if (ubicacionActual->enlaces.izquierda != -1)
    printf("3. Izquierda: %s\n", escenariosFull[ubicacionActual->enlaces.izquierda]->nombre);
  if (ubicacionActual->enlaces.derecha != -1)
    printf("4. Derecha: %s\n", escenariosFull[ubicacionActual->enlaces.derecha]->nombre);

  printf("0. Salir\n");

  int opcion;
  printf("Ingrese la direccion la cual desea moverse: ");
  scanf("%d", &opcion);

  int nueva_posicion = -1;
  if (opcion == 1 && ubicacionActual->enlaces.arriba != -1)
    nueva_posicion = ubicacionActual->enlaces.arriba;
  else if (opcion == 2 && ubicacionActual->enlaces.abajo != -1)
    nueva_posicion = ubicacionActual->enlaces.abajo;
  else if (opcion == 3 && ubicacionActual->enlaces.izquierda != -1)
    nueva_posicion = ubicacionActual->enlaces.izquierda;
  else if (opcion == 4 && ubicacionActual->enlaces.derecha != -1)
    nueva_posicion = ubicacionActual->enlaces.derecha;
  else if (opcion == 0)
    return;
  else
  {
    printf("OPCION INVALIDA\n");
    return;
  }

  if (nueva_posicion != -1)
  {
    float tiempoUtilizado = (jugador->pesoTotal + 1.0) / 10.0;
    jugador->tiempo_restante -= tiempoUtilizado;
    jugador->escenario_actual = escenariosFull[nueva_posicion];
    system("cls");
    printf("Te has movido a %s\n", escenariosFull[nueva_posicion]->nombre);
    printf("Tiempo Utilizado: %.2f\n", tiempoUtilizado);
    printf("Tiempo Restante: %.2f\n", jugador->tiempo_restante);
  }
}


/*Esta funcion se encarga de reiniciar la partida, es decir poner en 0 nuestro puntaje y peso total, ademas de reinciarnos el tiempo y colocarnos en el escenario inicial*/
void reinciarPartida(player *jugador)
{
  for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items))
  {
    free(i->nombre);
    free(i);
  }
  list_clean(jugador->items);

  jugador->tiempo_restante = 10;
  jugador->puntajeTotal = 0;
  jugador->pesoTotal = 0;
  jugador->escenario_actual = escenariosFull[1];

  printf("Partida Reiniciada con exito!\n");
  printf("Estas en el escenario: %s\n", jugador->escenario_actual->nombre);
}


/*Esta fybcuib se ebcarga de iniciar la partida colocando todos los valores a su posicion inicial, pero antes nos pregunta si nos quedamos sin tiempo, o llegamos a la salida
para mostrarnos el mensaje correspondiente. Si esto no sucede nos muestra el menu principal de acciones que podemos realizar con el jugador ademas de mostrarnos nuestro estado actual. 
Luego de seleccionar esta se encarga de llamar a la funcion correspondiente. Al finalizar el juego se encarga de liberar la memoria y ademas de vaciar las listas.*/
void iniciarPartida()
{
  player *jugador = malloc(sizeof(player));
  jugador->tiempo_restante = 10;
  jugador->puntajeTotal = 0;
  jugador->pesoTotal = 0;
  jugador->items = list_create();
  jugador->escenario_actual = escenariosFull[1];

  printf("Partida iniciada!\n");

  int opcion;
  do
  {
    
    if (jugador->escenario_actual->es_final)
    {
      system("cls");
      printf("\nFelicidades! Has llegado a la salida.\n");
      printf("Puntaje final: %d\n", jugador->puntajeTotal);
      printf("Inventario final:\n");
      for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items)) {
        printf("Nombre: %s, Valor: %d, Peso: %d\n", i->nombre, i->valor, i->peso);
      }
      printf("\nGracias por jugar GraphQuest!\n");
      printf("\n");
      printf("\n");
      break;
    }

    if (jugador->tiempo_restante <= 0)
    {
      system("cls");
      printf("\nSe ha acabado el tiempo!\n");
      printf("Puntaje final: %d\n", jugador->puntajeTotal);
      printf("\n");
      printf("\n");
      break;
    }
    mostrarEstadoActual(jugador);
    printf("\n");
    printf("=========================\n");
    printf("Seleccione una opcion:\n");
    printf("1. Recoger items\n");
    printf("2. Descartar items\n");
    printf("3. Moverse\n");
    printf("4. Reiniciar partida\n");
    printf("5. Volver al Menu Principal\n");
    scanf("%d", &opcion);

    switch (opcion)
    {
      case 1:
        system("cls");
        recogerItems(jugador);
        break;
      case 2:
        system("cls");
        descartarItems(jugador);
        break;
      case 3:
        system("cls");
        moverJugador(jugador);
        break;
      case 4:
        system("cls");
        reinciarPartida(jugador);
        break;
      case 5:
        system("cls");
        printf("Saliendo del juego...\n");
        break;
      default:
        system("cls");
        printf("Opcion invalida. Intente nuevamente.\n");
        break;
    }

    
  } while (opcion != 5);


  for (item *i = list_first(jugador->items); i != NULL; i = list_next(jugador->items)) 
  {
    free(i->nombre);
    free(i);
  }
  list_clean(jugador->items);
  free(jugador->items);
  free(jugador);
}


/*El main se encarga de leer los escenarios y con un Do While se encarga de mostrarnos un Pre-Menu donde nos pregunta si queremos iniciar la partida o salir del juego.*/
int main() 
{
  leer_escenarios();
  int opcion;
  
  do {
    
    printf("=========================\n");
    printf("Bienvenido a GraphQuest!\n");
    printf("1. Iniciar Partida \n");
    printf("2. Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) 
    {
      case 1:
        
        printf("Iniciando partida...\n");
        system("cls");
        iniciarPartida();
        break;
      case 2:
        system("cls");
        printf("Saliendo del juego...\n");
        break;
      default:
        system("cls");
        printf("Opcion invalida. Intente nuevamente.\n");
        break;
    }
  } while (opcion != 2);
  return 0;
}
