#include "plutonio.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> 

const int TOTAL_LINTERNAS = 5;
const int MOV_LINTERNA = 0;
const int LIMITE_MOV_LINTERNA = 5;
const int CANT_HERRAMIENTAS = 9;
const int CANT_OBSTACULOS = 20;
const int CANT_PROPULSORES = 6;
const int CANT_BARRAS_INICIAL = 0;
const int CANT_BARRAS_JUEGO = 10;
const char POSICION_VACIA = ' ';
const char RATA_RADIOACTIVA = 'R';
const char BARRIL = 'A';
const char DONA = 'D';
const char CHARCO = 'C';
const char INTERRUPTOR = 'I';
const char ESCALERA = 'E';
const char HOMERO = 'H';
const char BARRA = 'B';
#define LONGITUD_MATRIZ 20
#define CANT_COORDENADAS 100
#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define LINTERNA 'L'


/*
PRE: Las coordenadas deben tener valores dentro del rango establecido.
POST: devuelve la distancia manhattan entre los puntos dados.
*/
int calculo_distancia_manhattan(coordenada_t coordenada_1, coordenada_t coordenada_2){
  int distancia_filas = coordenada_1.fil - coordenada_2.fil;
  int distancia_columnas = coordenada_1.col - coordenada_2.col;
  int distancia_final = 0;
  if(distancia_filas < 0){
    distancia_filas = distancia_filas * (-1);
  }
  if(distancia_columnas < 0){
    distancia_columnas = distancia_columnas * (-1);
  }
  distancia_final = distancia_filas + distancia_columnas;
  return distancia_final;
}

/*
PRE:-
POST: Devuelve una posicion aleatoria entre 0 y 19 tanto para la fila como para la columna.
*/
coordenada_t inicializar_posicion_aleatoria(int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  int fila = 0;
  int columna = 0;
  bool repetida = false;
  do{
      fila = rand() % 20;      
      columna = rand() % 20;
      repetida = false;
      for (int i = 0; i < *coordenadas_usadas_tope; i++) {
        if ((coordenadas_usadas[i].fil == fila && coordenadas_usadas[i].col == columna) || (fila > 19 || fila < 0 || columna > 19 || columna < 0)) {
             repetida = true;
          }
      }
   }while(repetida);
  coordenada_t posicion = {fila,columna};
  coordenadas_usadas[*coordenadas_usadas_tope] = posicion;
  (*coordenadas_usadas_tope)++;
  return posicion;
}

/*
PRE: Necesita que la matriz este declarada.
POST:rellena la matriz con espacion vacios.
*/
void rellenar_tablero(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ]){
  for(int i = 0; i < LONGITUD_MATRIZ; i++){
     for(int j = 0; j < LONGITUD_MATRIZ; j++){
        tablero[i][j] = POSICION_VACIA;
     }
  }
}


/*
PRE:-
POST: recibe al personaje a controlar para inicializarlo.
*/
void inicializar_homero(personaje_t *homero, int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  homero->cantidad_barras = CANT_BARRAS_INICIAL;
  homero->energia = 400;
  homero->cantidad_linternas = TOTAL_LINTERNAS;
  homero->linterna_activada = false;
  homero->mov_linterna = MOV_LINTERNA;
  homero->posicion = inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas);
}

/*
PRE:-
POST:inicializa las herramientas y la cantidad de herramientas del juego.
*/
void inicializar_herramientas(objeto_t herramientas[], int *cantidad_herramientas, int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  *cantidad_herramientas = CANT_HERRAMIENTAS; 
  for(int i = 0; i < *cantidad_herramientas; i++){
    if(i < 5){
      objeto_t dona = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),DONA,false};
      herramientas[i] = dona;
    }
    else if(i>4){
      objeto_t interruptor = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),INTERRUPTOR,true};
      herramientas[i] = interruptor;
    }
   }
}


/*
PRE:- 
POST:inicializa los obstaculos y la cantidad de obstaculos del juego.
*/
void inicializar_obstaculos(objeto_t obstaculos[], int *cantidad_obstaculos, int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  *cantidad_obstaculos = CANT_OBSTACULOS;  
  for(int i = 0; i < *cantidad_obstaculos; i++){
      if(i < 5){
        objeto_t rata_radioactiva = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),RATA_RADIOACTIVA,false};
        obstaculos[i] = rata_radioactiva;
      }
      else if(i > 4){
        objeto_t barril = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),BARRIL,false};
        obstaculos[i] = barril;
      }
    }
}

/*
PRE:-
POST:inicializa los propulsores y la cantidad de propulsores del juego.
*/
void inicializar_propulsores(objeto_t propulsores[],int *cantidad_propulsores, int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  *cantidad_propulsores = CANT_PROPULSORES;
  for(int i = 0; i < *cantidad_propulsores; i++){
     if(i < 3){
      objeto_t escalera = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),ESCALERA,false};
        propulsores[i] = escalera;
     }
     else if(i > 2){
      objeto_t charco = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),CHARCO,false};
      propulsores[i] = charco;
     }
  }
}

/*
PRE:- 
POST:inicializa las barras y la cantidad de barras del juego. 
*/
void inicializar_barras(objeto_t barras[],int *cantidad_barras, int *coordenadas_usadas_tope, coordenada_t coordenadas_usadas[]){
  *cantidad_barras = CANT_BARRAS_JUEGO;
  for(int i = 0; i < *cantidad_barras; i++){
    objeto_t barra = {inicializar_posicion_aleatoria(coordenadas_usadas_tope, coordenadas_usadas),BARRA,false};
    barras[i] = barra;
  }
}

/*
PRE:el juego debe estar inicializado.
POST: introduce a Homero en la matriz.
*/
void ubicar_homero(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ], personaje_t homero){
   int fila = homero.posicion.fil;
   int columna = homero.posicion.col;
   tablero[fila][columna] = HOMERO;
}

/*
PRE: el juego debe estar inicializado.
POST: introduce las herramientas en la matriz si la linterna esta predida y esta hasta manhattan 2 o menos de distancia o si hay un interruptor activo. 
*/
void ubicar_herramientas(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ],objeto_t herramientas[], bool linterna_activa, int turnos_usados, coordenada_t coor_homero, int cantidad_herramientas){
  int distancia = 0;
  coordenada_t posicion_herramienta = {0,0}; 
  for(int i = 0; i < cantidad_herramientas; i++){
    posicion_herramienta = herramientas[i].posicion;
    distancia = calculo_distancia_manhattan(coor_homero, posicion_herramienta);
    if((linterna_activa && turnos_usados <= LIMITE_MOV_LINTERNA && distancia <= 2) || herramientas[i].visible){
     tablero[posicion_herramienta.fil][posicion_herramienta.col] = herramientas[i].tipo;
    }
    
  }
}

/*
PRE: el juego debe estar inicializado.
POST: introduce los obstaculos en la matriz si la linterna esta predida y esta hasta manhattan 2 o menos de distancia o si hay un interruptor activo. 
*/
void ubicar_obstaculos(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ],objeto_t obstaculos[], bool linterna_activa, int turnos_usados, coordenada_t coor_homero, int cantidad_obstaculos){
  int distancia = 0;
  coordenada_t posicion_obstaculo = {0,0};
  for(int i = 0; i < cantidad_obstaculos; i++){
    posicion_obstaculo = obstaculos[i].posicion;
    distancia = calculo_distancia_manhattan(coor_homero, posicion_obstaculo);
    if((linterna_activa && turnos_usados <= LIMITE_MOV_LINTERNA && distancia <= 2) || obstaculos[i].visible){
     tablero[posicion_obstaculo.fil][posicion_obstaculo.col] = obstaculos[i].tipo;
    }
  }
}

/*
PRE: el juego debe estar inicializado.
POST: introduce las barras en la matriz si la linterna esta predida y esta hasta manhattan 2 o menos de distancia o si hay un interruptor activo. 
*/
void ubicar_barras(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ],objeto_t barras[], bool linterna_activa, int turnos_usados, coordenada_t coor_homero, int cantidad_barras){
  int distancia = 0;
  coordenada_t posicion_barra = {0,0};
  for(int i = 0; i < cantidad_barras; i++){
    posicion_barra = barras[i].posicion;
    distancia = calculo_distancia_manhattan(coor_homero, posicion_barra);
    if((linterna_activa && turnos_usados <= LIMITE_MOV_LINTERNA && distancia <= 2) || barras[i].visible){
     tablero[posicion_barra.fil][posicion_barra.col] = barras[i].tipo;
    }
  }
}

/*
PRE: el juego debe estar inicializado.
POST: introduce los propulsores en la matriz si la linterna esta predida y esta hasta manhattan 2 o menos de distancia o si hay un interruptor activo. 
*/
void ubicar_propulsores(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ],objeto_t propulsores[], bool linterna_activa, int turnos_usados, coordenada_t coor_homero, int cantidad_propulsores){
  int distancia = 0;
  coordenada_t posicion_propulsor = {0,0};
  for(int i = 0; i < cantidad_propulsores; i++){
    posicion_propulsor = propulsores[i].posicion;
    distancia = calculo_distancia_manhattan(coor_homero, posicion_propulsor);
    if((linterna_activa && turnos_usados <= LIMITE_MOV_LINTERNA && distancia <= 2) || propulsores[i].visible){
     tablero[posicion_propulsor.fil][posicion_propulsor.col] = propulsores[i].tipo;
    }
  }
}


/*
PRE: el juego debe estar inicializado.
POST: activa o desactiva la visibilidad del objeto que reciba.
*/
void activar_o_descativar_visibilidad(objeto_t *objeto){
  objeto->visible = !(objeto->visible);
}

/*
PRE: el juego debe estar inicializado.
POST: activa la visibilidad de las donas.
*/
void activar_visibilidad_donas(int cantidad_herramientas, juego_t *juego){
   for(int i = 0; i < cantidad_herramientas; i++){
     if(juego->herramientas[i].tipo == DONA){
      activar_o_descativar_visibilidad(&(juego->herramientas[i]));
     }
   }
}

/*
PRE: el juego debe estar inicializado.
POST: desactiva visibilidad dona.
*/
void desactivar_visibilidad_donas(int cantidad_herramientas, juego_t *juego){
  for(int i = 0; i < cantidad_herramientas; i++){
    if(juego->herramientas[i].tipo == DONA){
     juego->herramientas[i].visible = false;
    }
  }
}


/*
PRE: el juego debe estar inicializado.
POST: activa visibilidad obstaculos.
*/
void activar_visibilidad_obstaculos(int cantidad_obstaculos, juego_t *juego){
  for(int i = 0; i < cantidad_obstaculos; i++){
     activar_o_descativar_visibilidad(&(juego->obstaculos[i]));
  }
}

/*
PRE: el juego debe estar inicializado.
POST: desactiva visibilidad obstaculo.
*/
void desactivar_visibilidad_obstaculos(int cantidad_obstaculos, juego_t *juego){
 for(int i = 0; i < cantidad_obstaculos; i++){
    juego->obstaculos[i].visible = false;
 }
}


/*
PRE: el juego debe estar inicializado.
POST: activa visibilidad propulsores.
*/
void activar_visibilidad_propulsores(int cantidad_propulsores, juego_t *juego){
  for(int i = 0; i < cantidad_propulsores; i++){
     activar_o_descativar_visibilidad(&(juego->propulsores[i]));
  }
}

/*
PRE: el juego debe estar inicializado.
POST: desactiva visibilidad propulsores.
*/
void desactivar_visibilidad_propulsores(int cantidad_propulsores, juego_t *juego){
 for(int i = 0; i < cantidad_propulsores; i++){
    juego->propulsores[i].visible = false;
 }
}


/*
PRE: el juego debe estar inicializado.
POST: activa visibilidad barras.
*/
void activar_visibilidad_barras(int cantidad_barras, juego_t *juego){
  for(int i = 0; i < cantidad_barras; i++){
     activar_o_descativar_visibilidad(&(juego->barras[i]));
  }
}

/*
PRE: el juego debe estar inicializado.
POST: desactiva visibilidad barras.
*/
void desactivar_visibilidad_barras(int cantidad_barras, juego_t *juego){
 for(int i = 0; i < cantidad_barras; i++){
    juego->barras[i].visible = false;
 }
}

/*
PRE: el juego debe estar inicializado.
POST: delvuelve true si la posicion es la misma que la de un propulsor.
*/
bool misma_posicion_propulsores(int fila, int columna, objeto_t propulsores[], int cantidad_propulsores){
   bool misma_posicion = false;
   for(int i = 0; i < cantidad_propulsores; i++){
      if(fila == propulsores[i].posicion.fil && columna == propulsores[i].posicion.col){
        misma_posicion = true;
      }
   }
   return misma_posicion;
}

/*
PRE: el juego debe estar inicializado.
POST: delvuelve true si la posicion es la misma que la de un obstaculo.
*/
bool misma_posicion_obstaculos(int fila, int columna, objeto_t obstaculos[], int cantidad_obstaculos){
  bool misma_posicion = false;
  for(int i = 0; i < cantidad_obstaculos; i++){
     if(fila == obstaculos[i].posicion.fil && columna == obstaculos[i].posicion.col){
       misma_posicion = true;
     }
  }
  return misma_posicion;
}

/*
PRE: el juego debe estar inicializado.
POST: delvuelve true si la posicion es la misma que la de una herramienta.
*/
bool misma_posicion_herramientas(int fila, int columna, objeto_t herramientas[], int cantidad_herramientas){
  bool misma_posicion = false;
  for(int i = 0; i < cantidad_herramientas; i++){
     if(fila == herramientas[i].posicion.fil && columna == herramientas[i].posicion.col){
       misma_posicion = true;
     }
  }
  return misma_posicion;
}

/*
PRE: el juego debe estar inicializado.
POST: delvuelve true si la posicion es la misma que la de una barra.
*/
bool misma_posicion_barras(int fila, int columna, objeto_t barras[], int cantidad_barras){
  bool misma_posicion = false;
  for(int i = 0; i < cantidad_barras; i++){
     if(fila == barras[i].posicion.fil && columna == barras[i].posicion.col){
       misma_posicion = true;
     }
  }
  return misma_posicion;
}

/*
PRE:el juego debe estar inicializado y la accion debe ser valida.
POST: Cambia la posicion de las ratas de forma aleatoria cuando se activa un interruptor o la linterna.
*/
void cambiar_posicion_ratas(juego_t *juego){
  int fila = 0;
  int columna = 0;
  bool iguales = false;
  for(int i = 0; i < juego->cantidad_obstaculos; i++){
     if(juego->obstaculos[i].tipo == RATA_RADIOACTIVA){
         do{
          iguales = false;
          fila = rand() % 20;      
          columna = rand() % 20;
           if(misma_posicion_barras(fila, columna, juego->barras, juego->cantidad_barras)){
               iguales = true;
           }
           else if(misma_posicion_herramientas(fila, columna, juego->herramientas, juego->cantidad_herramientas)){
            iguales = true;
           }
           else if(misma_posicion_propulsores(fila, columna, juego->propulsores, juego->cantidad_propulsores)){
            iguales = true;
           } 
           else if(misma_posicion_obstaculos(fila, columna, juego->obstaculos, juego->cantidad_obstaculos)){
            iguales = true;
           }         
         }while(iguales);
         juego->obstaculos[i].posicion.fil = fila;
         juego->obstaculos[i].posicion.col = columna;
     }
  }
}

/*
PRE: El juego debe estar inicializado.
POST: activa el interruptor y muestra todos los items.
*/
void activar_interruptor(juego_t *juego, int cantidad_herramientas, int cantidad_obstaculos, int cantidad_propulsores, int cantidad_barras, int i){
    activar_visibilidad_donas(cantidad_herramientas, juego);
    activar_visibilidad_barras(cantidad_barras, juego);
    activar_visibilidad_obstaculos(cantidad_obstaculos, juego);
    activar_visibilidad_propulsores(cantidad_propulsores, juego);
}

/*
PRE: El juego debe estar inicializado.
POST: recolecta dona, la elimina del vector y suma 10 puntos de energia a homero.
*/
void obtener_dona(juego_t *juego, int *cantidad_herramientas, int i){
       juego->homero.energia += 10;
       juego->herramientas[i] = juego->herramientas[*cantidad_herramientas-1];
       juego->herramientas[*cantidad_herramientas - 1].tipo = POSICION_VACIA;
       (*cantidad_herramientas)--;
}

/*
PRE: el juego debe estar inicializado.
POST: si homero se para sobre una dona la obtiene y si es un interruptor lo activa.
*/
void utilizar_herramienta(juego_t *juego){
  for(int i = 0; i < juego->cantidad_herramientas; i++){
    if(juego->homero.posicion.fil == juego->herramientas[i].posicion.fil && juego->homero.posicion.col == juego->herramientas[i].posicion.col){
      if(juego->herramientas[i].tipo == DONA){
       obtener_dona(juego, &(juego->cantidad_herramientas), i);
      }
      else if(juego->herramientas[i].tipo == INTERRUPTOR){
          activar_interruptor(juego, juego->cantidad_herramientas, juego->cantidad_obstaculos, juego->cantidad_propulsores, juego->cantidad_barras, i);
          cambiar_posicion_ratas(juego);
      }
    }
  }
}

/*
PRE: necesita que la matriz este llena y el juego inicializado.
POST: imprime la matriz.
*/
void imprimir_tablero(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ]){
  for(int i = 0; i < LONGITUD_MATRIZ; i++){
    for(int j = 0; j < LONGITUD_MATRIZ; j++){
          printf("[%c]",tablero[i][j]);
    }
    printf("\n");
 }
}

/*
PRE:requiere que el juego este inicializado y las matriz llena.
POST: Imprime la matriz con los todos los items y la informacion del juego.
*/
void imprimir_juego(char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ], personaje_t homero, juego_t juego){
   int energia = homero.energia;
   int cantidad_linternas = homero.cantidad_linternas;
   int cantidad_movimientos = homero.mov_linterna;
   int cantidad_barras = homero.cantidad_barras;
   bool linterna_activa = homero.linterna_activada;
   imprimir_tablero(tablero);
   printf("\n");
   printf("ENERGIA: %i\n",energia);
   if(homero.cantidad_linternas <= 0){
    printf("LINTERNAS DISPONIBLES: 0\n");
   }
   else{
   printf("LINTERNAS DISPONIBLES: %i\n",cantidad_linternas);
   }
   if(linterna_activa){
      printf("LINTERNA ACTIVA\n");
      printf("MOVIMIENTOS HECHOS: %i\n",cantidad_movimientos);
    }
   printf("CANTIDAD BARRAS: %i\n",cantidad_barras);
   if(misma_posicion_propulsores(homero.posicion.fil, homero.posicion.col, juego.propulsores, juego.cantidad_propulsores)){
      printf("Se activo un propulsor\n");
   }
   else if(misma_posicion_obstaculos(homero.posicion.fil, homero.posicion.col, juego.obstaculos, juego.cantidad_obstaculos)){
    printf("Se contacto un obstaculo\n");
   }
}
/*
PRE: el juego debe estar inicializado y la accion debe ser valida.
POST: suma 1 a los movimientos con linterna si esta activa y la apaga cuando pasen los turnos.
*/
void sumar_mov_linterna(bool *linterna_activa, int *mov_linterna){
   if(*linterna_activa && *mov_linterna < LIMITE_MOV_LINTERNA){
       (*mov_linterna)++;
   }
   else if(*linterna_activa && *mov_linterna == LIMITE_MOV_LINTERNA){
     *linterna_activa = false;
     *mov_linterna = MOV_LINTERNA;
   }
}

/*
PRE:el juego debe estar inicializado y la accion debe ser valida.
POST: verifica que homero este sobre una barra, si es el caso, elimina la barra del vector y le suma una barra a homero.
*/
void obtener_barra(coordenada_t *posicion_homero, int *cantidad_barras, objeto_t barras[], int *barras_homero, bool linterna_activa){
   for(int i = 0; i < *cantidad_barras; i++){
     if(posicion_homero->fil == barras[i].posicion.fil && posicion_homero->col == barras[i].posicion.col){
      barras[i] = barras[*cantidad_barras-1];
      barras[*cantidad_barras -1].tipo = POSICION_VACIA; 
       (*cantidad_barras)--;
       (*barras_homero)++;
      }
   }
}

/*
PRE: el juego debe estar inicializado y la accion debe ser valida.
POST: si homero chocha con un barril le resta puntos y si pisa una rata le reduce la cantidad de linternas.
*/
void reaccionar_obstaculo(juego_t *juego){
  for(int i = 0; i < juego->cantidad_obstaculos; i++){
    if((juego->homero.posicion.fil == juego->obstaculos[i].posicion.fil &&
      juego->homero.posicion.col == juego->obstaculos[i].posicion.col)){
        if(juego->obstaculos[i].tipo == BARRIL){
           juego->homero.energia -= 15;
        }else{
          if(juego->homero.cantidad_linternas > 0){
            juego->homero.cantidad_linternas--;
          }
        }
    }
  }
}

/*
PRE: El juego debe estar inicializado.
POST: desactiva la visibilidad de todos los objetos.
*/
void desactivar_visibilidad_todos_objetos(juego_t *juego){
  desactivar_visibilidad_donas(juego->cantidad_herramientas, juego);
  desactivar_visibilidad_propulsores(juego->cantidad_propulsores, juego);
  desactivar_visibilidad_obstaculos(juego->cantidad_obstaculos, juego);
  desactivar_visibilidad_barras(juego->cantidad_barras, juego);
}

/*
PRE: El juego debe estar inicializado
POST: Activa visibilidad si esta a 2 o menos de distancia y la linterna esta activa
*/
void activar_visibilidad_por_linterna_obstaculos(int cantidad_obstaculos, personaje_t homero, objeto_t obstaculos[], int turnos_usados){
  for(int i = 0; i < cantidad_obstaculos; i++){
    if(calculo_distancia_manhattan(homero.posicion, obstaculos[i].posicion) <= 2 && !obstaculos[i].visible && homero.linterna_activada && turnos_usados < LIMITE_MOV_LINTERNA){
      activar_o_descativar_visibilidad(&(obstaculos[i]));
  }
 }
}

/*
PRE: El juego debe estar inicializado
POST: Activa visibilidad si esta a 2 o menos de distancia y la linterna esta activa
*/
void activar_visibilidad_por_linterna_herramientas(int cantidad_herramientas, personaje_t homero, objeto_t herramientas[], int turnos_usados){
  for(int i = 0; i < cantidad_herramientas; i++){
    if(calculo_distancia_manhattan(homero.posicion, herramientas[i].posicion) <= 2 && !herramientas[i].visible && homero.linterna_activada && turnos_usados < LIMITE_MOV_LINTERNA){
      activar_o_descativar_visibilidad(&(herramientas[i]));
    }
  }
}

/*
PRE: El juego debe estar inicializado
POST: Activa visibilidad si esta a 2 o menos de distancia y la linterna esta activa
*/
void activar_visibilidad_por_linterna_propulsores(int cantidad_propulsores, personaje_t homero, objeto_t propulsores[], int turnos_usados){
  for(int i = 0; i < cantidad_propulsores; i++){
    if(calculo_distancia_manhattan(homero.posicion, propulsores[i].posicion) <= 2 && !propulsores[i].visible && homero.linterna_activada && turnos_usados < LIMITE_MOV_LINTERNA){
      activar_o_descativar_visibilidad(&(propulsores[i]));
     }
   }
}

/*
PRE: El juego debe estar inicializado
POST: Activa visibilidad si esta a 2 o menos de distancia y la linterna esta activa
*/
void activar_visibilidad_por_linterna_barras(int cantidad_barras, personaje_t homero, objeto_t barras[], int turnos_usados){
  for(int i = 0; i < cantidad_barras; i++){
    if(calculo_distancia_manhattan(homero.posicion, barras[i].posicion) <= 2 && !barras[i].visible && homero.linterna_activada && turnos_usados < LIMITE_MOV_LINTERNA){
        activar_o_descativar_visibilidad(&(barras[i]));
    }
  }
}

/*
PRE: El juego debe estar inicializado
POST: Activa visibilidad si esta a 2 o menos de distancia y la linterna esta activa
*/
void activar_visibilidad_por_linterna(juego_t *juego){
 activar_visibilidad_por_linterna_barras(juego->cantidad_barras, juego->homero,juego->barras,juego->homero.mov_linterna);
 activar_visibilidad_por_linterna_herramientas(juego->cantidad_herramientas, juego->homero,juego->herramientas,juego->homero.mov_linterna);
 activar_visibilidad_por_linterna_obstaculos(juego->cantidad_obstaculos, juego->homero,juego->obstaculos,juego->homero.mov_linterna);
 activar_visibilidad_por_linterna_propulsores(juego->cantidad_propulsores, juego->homero,juego->propulsores,juego->homero.mov_linterna);
}


/*
PRE: El juego debe estar inicializado.
POST: devuelve false si Homero no se movio, true si se pudo mover.
*/
bool se_movio_homero(int *fila, int *columna, char accion, juego_t *juego){
  coordenada_t pos_futura;
  switch (accion){
   case ARRIBA:
    pos_futura.fil = *fila - 1;
    pos_futura.col = *columna;
   break;
   case ABAJO:
    pos_futura.fil = *fila + 1;
    pos_futura.col = *columna;
   break;
   case DERECHA:
    pos_futura.fil = *fila;
    pos_futura.col = *columna + 1;
   break;
   case IZQUIERDA:
    pos_futura.fil = *fila;
    pos_futura.col = *columna - 1;
   break;
  }
  if(pos_futura.fil < 0 || pos_futura.col < 0 || pos_futura.fil > 19 || pos_futura.col > 19){
     return false;
   }
  *fila = pos_futura.fil;
  *columna = pos_futura.col;
  desactivar_visibilidad_todos_objetos(juego);
  obtener_barra(&(juego->homero.posicion), &(juego->cantidad_barras), juego->barras, &(juego->homero.cantidad_barras), juego->homero.linterna_activada);
  reaccionar_obstaculo(juego);
  utilizar_herramienta(juego);
  return true;
}

/*
PRE:el juego debe estar inicializado.
POST: desplaza a Homero si interactua con una escalera.
*/
void utilizar_escalera(juego_t *juego, char accion){
     switch (accion){
      case ARRIBA:
      case ABAJO:
       se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ARRIBA, juego);
      break;
      case DERECHA:
       if(juego->homero.posicion.fil > 0 && juego->homero.posicion.col < 19){
        se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ARRIBA, juego);
        se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),accion, juego);
       }
      break;
      case IZQUIERDA:
       if(juego->homero.posicion.fil > 0 && juego->homero.posicion.col > 0){
        se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ARRIBA, juego);
        se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),accion, juego);
       }
      break;
    }
}

/*
PRE:el juego debe estar inicializado.
POST: desplaza a Homero si interactua con un charco.
*/
void utilizar_charco(juego_t *juego, char accion){
    switch (accion){
     case ARRIBA:
     case ABAJO:
      se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ABAJO, juego);
     break;
     case DERECHA:
      if(juego->homero.posicion.fil < 19 && juego->homero.posicion.col < 19){
       se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ABAJO, juego);
       se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),accion, juego);
      }
     break;
     case IZQUIERDA:
      if(juego->homero.posicion.fil < 19 && juego->homero.posicion.col > 0){
       se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),ABAJO, juego);
       se_movio_homero(&(juego->homero.posicion.fil),&(juego->homero.posicion.col),accion, juego);
      }
     break;
    }
}

/*
PRE: el juego debe estar inicializado.
POST: desplaza a Homero cuando choca con un propulsor.
*/
void utilizar_propulsores(juego_t *juego, char accion){
  for(int i = 0; i < juego->cantidad_propulsores; i++){
    if(juego->homero.posicion.fil == juego->propulsores[i].posicion.fil && juego->homero.posicion.col == juego->propulsores[i].posicion.col){
      for(int j = 0; j < 3; j++){
        if(juego->propulsores[i].tipo == CHARCO){
          utilizar_charco(juego, accion);
        }else{
          utilizar_escalera(juego, accion);
        }
      }
    }
  }
}

void inicializar_juego(juego_t *juego){
    coordenada_t coordenadas_usadas[CANT_COORDENADAS];
    int coordenadas_usadas_tope = 0;  
    inicializar_homero(&(juego->homero), &coordenadas_usadas_tope, coordenadas_usadas);
    inicializar_herramientas(juego->herramientas, &(juego->cantidad_herramientas), &coordenadas_usadas_tope, coordenadas_usadas);
    inicializar_obstaculos(juego->obstaculos, &(juego->cantidad_obstaculos), &coordenadas_usadas_tope, coordenadas_usadas);
    inicializar_barras(juego->barras, &(juego->cantidad_barras), &coordenadas_usadas_tope, coordenadas_usadas);
    inicializar_propulsores(juego->propulsores, &(juego->cantidad_propulsores), &coordenadas_usadas_tope, coordenadas_usadas);
}

void realizar_jugada(juego_t *juego, char accion){
  personaje_t *homero = &(juego->homero);
  bool accion_valida = false;
  desactivar_visibilidad_todos_objetos(juego);
  if(accion == LINTERNA && !homero->linterna_activada && homero->cantidad_linternas > 0){
     homero->linterna_activada = true;
     homero->cantidad_linternas--;
     cambiar_posicion_ratas(juego);
     accion_valida = true;
   }else{
    accion_valida = se_movio_homero(&(homero->posicion.fil), &(homero->posicion.col), accion, juego);
    if(accion_valida){
      homero->energia--;
    }
   }
   activar_visibilidad_por_linterna(juego);
   utilizar_propulsores(juego, accion);
   if(accion_valida){
    sumar_mov_linterna(&(homero->linterna_activada), &(homero->mov_linterna));
   }
}

void mostrar_juego(juego_t juego){
 personaje_t homero = juego.homero;
 char tablero[LONGITUD_MATRIZ][LONGITUD_MATRIZ];
 rellenar_tablero(tablero);
 ubicar_herramientas(tablero, juego.herramientas, homero.linterna_activada, homero.mov_linterna, homero.posicion, juego.cantidad_herramientas);
 ubicar_barras(tablero, juego.barras, homero.linterna_activada, homero.mov_linterna, homero.posicion, juego.cantidad_barras);
 ubicar_obstaculos(tablero, juego.obstaculos, homero.linterna_activada, homero.mov_linterna, homero.posicion, juego.cantidad_obstaculos);
 ubicar_propulsores(tablero, juego.propulsores, homero.linterna_activada, homero.mov_linterna, homero.posicion, juego.cantidad_propulsores);
 ubicar_homero(tablero, homero);
 imprimir_juego(tablero, homero, juego);
}

int estado_juego(juego_t juego){
  if(juego.homero.cantidad_barras == CANT_BARRAS_JUEGO){
    return 1;
  }
  else if(juego.homero.cantidad_barras < CANT_BARRAS_JUEGO && juego.homero.energia <= 0){
    return -1;
  }  
  return 0;
}