#include "plutonio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define LINTERNA 'L'


/*
PRE:-
POST: Imprime por pantalla la presentacion del juego.
*/
void presentar_juego(){
  printf("Bienvenido al juego 'Desastre Nuclear'.\n");
  printf("En este deberas controlar a Homero y encontrar las 10 barras de plutonio distribuidas por el mapa.\n");
  printf("Pero cuidado! estas a oscuras, solo podes ver los interruptores y ademas hay obstaculos en el camino\n");
  printf("Por cada movimiento perdes un punto de energia, si chocas con un barril perdes 15 puntos.\n");
  printf("Contas con 5 linternas las cuales duran 5 turnos, si pisas una rata perdes una linterna.\n");
  printf("Por suerte tambien hay donas que al comerlas obtenes 10 puntos de energia.\n");
  printf("Tambien hay propulsores que te desplazan por el mapa sin gastar energia extra.\n");
  printf("El juego termina cuando te quedas sin energia o cuando coleccionas las 10 barras.\n");
  printf("Empieza el juego. Para moverse WASD y para activar la linterna L\n");
}

/*
PRE: El juego debe estar inicializado.
POST: Pide al jugador la accion e imprime el tablero.
*/
void pedir_accion(juego_t *juego, char *movimiento){
  mostrar_juego(*juego);
  printf("Ingrese el movimiento\n");
  scanf(" %c", movimiento);
while(*movimiento != ARRIBA && *movimiento != ABAJO && *movimiento != IZQUIERDA && *movimiento != DERECHA && *movimiento != LINTERNA){
  printf("Movimiento invalido\n");
  scanf(" %c", movimiento);
}
realizar_jugada(juego, *movimiento);
system("clear");

}


int main(){
   srand((unsigned)time(NULL));
   int estado = 0;
   juego_t juego;
   inicializar_juego(&juego);
   char movimiento;
   system("clear");
   presentar_juego();
   do{
     pedir_accion(&juego, &movimiento); 
     estado = estado_juego(juego);    
   }while(estado == 0);
   if(estado == 1){
     printf("Ganaste\n");
   }else{
     printf("Perdiste\n");
   }
   return 0;
}
