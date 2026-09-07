/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas les funciones que tenéis 
 * que implementar en ensamblador.
 * Desde este código se hacen las llamadas a les subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO HAY QUE ENTREGARLO
 * */
 
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>     //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>      //STDIN_FILENO

/**
 * Constantes
 */
#define DimMatrix 3      //dimensión de la matriz 3x3
#define SizeMatrix  DimMatrix*DimMatrix //=9

extern int developer;   //Variable declarada en ensamblador que indica el nombre del programador

/**
 * Definición de variables globales
 */
// Matriz 3x3 donde guardamos las fichas iniciales del juego.
char tilesIni[DimMatrix][DimMatrix] = { {'1','2','3'},
                                        {'4','C',' '},                                     
                                        {'B','D','A'} }; 

// Matriz 3x3 donde guardamos las fichas del juego.
char tiles[DimMatrix][DimMatrix]    = { {'1','2','3'},
                                        {'4','X','A'},
                                        {'B','C','D'} }; 

// matrizs 3x3 donde guardamos las fichas en la posición que queremos conseguir.
char tilesEnd[DimMatrix][DimMatrix] = { {'1','2','3'},
                                        {'4',' ','A'},
                                        {'B','C','D'} }; 
                                        
/**
 * Definición de las funciones de C
 */
void clearscreen_C();
void gotoxyP2_C();
void printchP2_C(char);
char getchP2_C();

char printMenuP2_C();
void printBoardP2_C();

void  copyMatrixP2_C(char[DimMatrix][DimMatrix]);
void  updateBoardP2_C(char[DimMatrix][DimMatrix], long);
int   getSpacePosP2_C(char[DimMatrix][DimMatrix]);
void  posCurScreenP2_C(int);
int   moveCursorP2_C(char, int);
short moveTileP2_C(int, int, short);
short checkEndP2_C(long, short);
void  printMessageP2_C(short);
void  playP2_C();


/**
 * Definición de las subrutinas de ensamblador que es llaman desde C.
 */
void  copyMatrixP2(char[DimMatrix][DimMatrix]);
void  updateBoardP2(char[DimMatrix][DimMatrix], long);
int   getSpacePosP2();
void  posCurScreenP2(int);
int   moveCursorP2(char, int);
short moveTileP2(int, int, short);
short checkEndP2(long, short);
void  playP2();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 *   
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no es llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en una fila y una columna de la pantalla
 * en función de la fila (rowScreen) y de la columna (colScreen) 
 * recibidos como parámetro.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * rdi(edi): (rowScreen): Fila
 * rsi(esi): (colScreen): Columna
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'gotoxyP2' 
 * para poder llamar a esta función guardando el estado de los registros 
 * del procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void gotoxyP2_C(int rowScreen, int colScreen){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter (c) en pantalla, recibido como parámetro, 
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * rdi(dil): (c): Carácter que queremos mostrar.
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printchP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void printchP2_C(char c){
   
   printf("%c",c);
   
}


/**
 * Leer una tecla y retornar el carácter asociado 
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * rax(al): (c): Carácter que leemos de teclado
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'getchP2' para
 * llamar a esta función guardando el estado de los registros del procesador.
 * Esto se hace porque las funciones de C no mantienen el estado de los 
 * registros.
 * El paso de parámetros es equivalente.
 */
char getchP2_C(){

   int c;   

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándar (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

    /* ~ICANON para tratar la entrada de teclado carácter a carácter no como línea entera acabada en /n
    ~ECHO para que no se muestre el carácter leído.*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. */
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   c=getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

   /*Retornar el carácter leído*/
   return (char)c;
   
}


/**
 * Mostrar en pantalla el menú del juego y pedir una opción.
 * Sólo acepta una de las opciones correctas del menú ('0'-'9')
 * 
 * Variables globales utilizadas:   
 * developer:((char *)&developer): Variable definida en el código ensamblador.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * rax(al): (charac): Opción escogida del menú, leída de teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
char printMenuP2_C(){

   clearScreen_C();
   gotoxyP2_C(1,1);
   printf("                           \n");
   printf("        Developed by:      \n");
   printf("     ( %s )    \n",(char *)&developer);
   printf(" _________________________ \n");
   printf("|                         |\n");
   printf("|         MAIN MENU       |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|     1. copyMatrix       |\n");
   printf("|     2. updateBoard      |\n");
   printf("|     3. getSpacePos      |\n");
   printf("|     4. posCurScreen     |\n");
   printf("|     5. moveCursor       |\n");
   printf("|     6. moveTile         |\n");
   printf("|     7. checkEnd         |\n");
   printf("|     8. Play Game        |\n");
   printf("|     9. Play Game C      |\n");
   printf("|     0. Exit             |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|        OPTION:          |\n");
   printf("|_________________________|\n");  

   char charac =' ';
    while (charac < '0' || charac > '9') {
      gotoxyP2_C(21,18);      
      charac = getchP2_C();   
   }
   return charac;
   
}


/**
 * Mostrar el tablero de juego en pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función se llama des de C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 * No hay paso de parámetros.
 */
void printBoardP2_C(){

   gotoxyP2_C(0,0);                                   //Filas
                                                      //Tablero
   printf(" _____________________________________ \n"); //01
   printf("|                                     |\n"); //02
   printf("|           8-PUZZLE  v2.0            |\n"); //03
   printf("|                                     |\n"); //04
   printf("|      Order the tiles as shown!      |\n"); //05
   printf("|_____________________________________|\n"); //06
   printf("|                                     |\n"); //07
   printf("|  Remaining Moves[_]    FINAL GOAL   |\n"); //08
   printf("|                                     |\n"); //09
   //             8   12  16                           <- Columnas Tablero               
   printf("|      0   1   2         0   1   2    |\n"); //10
   printf("|    +---+---+---+     +---+---+---+  |\n"); //11
   printf("|  0 |   |   |   |   0 | 1 | 2 | 3 |  |\n"); //12
   printf("|    +---+---+---+     +---+---+---+  |\n"); //13
   printf("|  1 |   |   |   |   1 | 4 |   | A |  |\n"); //14
   printf("|    +---+---+---+     +---+---+---+  |\n"); //15
   printf("|  2 |   |   |   |   2 | B | C | D |  |\n"); //16
   printf("|    +---+---+---+     +---+---+---+  |\n"); //17
   printf("|_____________________________________|\n"); //18
   printf("|                                     |\n"); //19
   printf("|                                     |\n"); //20
   printf("|_____________________________________|\n"); //21
   printf("|                                     |\n"); //22
   printf("|   (i)Up (j)Left (k)Down (l)Right    |\n"); //23
   printf("|   (Space) move tile  (ESC)  Exit    |\n"); //24
   printf("|_____________________________________|\n"); //25
            
}


/**
 * Copiar la matriz (tilesIni), sobre la matriz (t), recibida como parámetro.
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte.
 * 
 * Variables globales utilizadas:   
 * (tilesIni): Matriz con las fichas iniciales del juego
 * 
 * Parámetros de entrada: 
 * rdi(rdi): (t): Dirección de la matriz donde guardamos la fichas del juego.
 *   
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'copyMatrixP2', 
 * el paso de parámetros es equivalente.
 */
void copyMatrixP2_C(char t[DimMatrix][DimMatrix]){
   
   int i, j;

   for (i=0;i<DimMatrix;i++){
      for (j=0;j<DimMatrix;j++){
         t[i][j] = tilesIni[i][j];
      }
   }

}



/**
 * Mostrar los valores de la matriz (t), recibida como parámetro, en 
 * pantalla, dentro del tablero en las posiciones correspondientes.
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte, y para cada elemento de la matriz:
 * Posicionar el cursor en el tablero llamando a la función gotoxyP2_C.
 * La posición inicial del cursor es la fila 12 de la pantalla (fila 0 
 * de la matriz), columna 8 de la pantalla (columna 0 de la matriz).
 * Mostrar los caracteres de cada posición de la matriz (t) 
 * llamando a la función printchP2_C.
 * Actualizar la columna (colScreen) de 4 en 4 y al cambiar de fila
 * (rowScreen) de 2 en 2.
 * Mostrar los movimientos que quedan por hacer (m) dentro del tablero 
 * en la fila 8, columna 20 de la pantalla.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada:
 * rdi(rdi): (t): Direcció de la matriz donde guardamos las fichas del juego.
 * rsi(esi): (m): Movimientos que quedan para ordenar las fichas.
 *   
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'updateBoardP2', 
 * el paso de parámetros es equivalente.
 */
void updateBoardP2_C(char t[DimMatrix][DimMatrix], long m){

   int i, j;
   int rowScreen, colScreen;
   rowScreen = 12;
   for (i=0;i<DimMatrix;i++){
	  colScreen = 8;
      for (j=0;j<DimMatrix;j++){
         gotoxyP2_C(rowScreen, colScreen);
         printchP2_C(t[i][j]);
         colScreen = colScreen + 4;
      }
      rowScreen = rowScreen + 2;
   }
   
   gotoxyP2_C(8, 20);
   m = m + '0';
   printchP2_C(m);
   
}

/**
 * Buscar donde está el espacio en blanco dentro de la matriz (t), 
 * recibida como parámetro i retornar la posicón (spacePos) [0..8] 
 * del espacio dentro de la matriz.
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a bajo.
 * Si el número de espacios no es 1 (spacePos = sizeMatrix).
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte.
 * 
 * Variables globales utilizadas: 
 * Ninguna 
 * 
 * Parámetros de entrada:
 * rdi(rdi): (t): Dirección de la matriz donde guardamos las fichas del juego.
 *  
 * Parámetros de salida : 
 * rax(eax): (spacePos): Posición del espacio dentro de la matriz.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'getSpacePosP2, 
 * el paso de parámetros es equivalente.
 */
int getSpacePosP2_C(char t[DimMatrix][DimMatrix]){

   int i=0, j=0, spaces=0, pos=0, spacePos=0;
   for (i=0;i<DimMatrix;i++){
      for (j=0;j<DimMatrix;j++){
        if (tiles[i][j]==' ') {
          spaces++;
          spacePos = pos;
        }
        pos++;
       }
   }
   
   if (spaces!=1){  
   	  spacePos = SizeMatrix;
   }  
   return spacePos;
}


/**
 * Posicionar el cursor en la pantalla, dentro del tablero, en función 
 * de la variable (cPos), recibida como parámetro.
 * Para calcular la posición del cursor en pantalla (rowScreen) y
 * (colScreen) utilizar estas fórmulas:
 * rowScreen=12+(cursorPos/DimMatrix)*2
 * colScreen= 8+(cursorPos%DimMatrix)*4
 * Para posicionar el cursor llamar a la función gotoxyP2_C.
 * 
 * Variables globales utilizadas: 
 * Ninguna
 * 
 * Parámetros de entrada:
 * rdi(edi): (cPos): Posició del cursor en la matriz (tiles).
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'posCurScreenP2, 
 * el paso de parámetros es equivalente.
 * 
 */
void posCurScreenP2_C(int cPos) {

   int rowScreen=12+(cPos/DimMatrix)*2;
   int colScreen= 8+(cPos%DimMatrix)*4;
   gotoxyP2_C(rowScreen, colScreen);
   
}


/**
 * Mover el cursor en la dirección indicada por la variable (c), recibido
 * como parámetro ('i':arriba, 'k':bajo, 'j':izquierda o 'l':derecha).
 * Actualizar la variable (cPos), recibida como parámetro, controlando que
 * no salga del tablero y devolver el valor actualizado.
 *
 * No se debe posicionar el cursor en la pantalla.
 *
 * Variables globales utilizadas:
 * Ninguna
 *
 * Parámetros de entrada:
 * rdi(dil) : (c)   : Caracter leído de teclado.
 * rsi(esis): (cPos): Posición del cursor en la matriz (tiles).
 *
 * Parámetros de salida:
 * rax(eax) : (cPos): Posición del cursor en la matriz (tiles).
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'moveCursorP2, 
 * el paso de parámetros es equivalente.
 */
int moveCursorP2_C(char c, int cPos){

   int i = cPos/DimMatrix;
   int j = cPos%DimMatrix;
   switch(c){
      case 'i':
         if (i > 0) {
			cPos = cPos - DimMatrix;
         }
      break;
      case 'k':
         if (i < (DimMatrix-1)) {
			cPos = cPos + DimMatrix;
         }
      break;
      case 'j':
         if (j > 0) {
			cPos = cPos - 1;
         }
      break;
      case 'l':
         if (j < (DimMatrix-1)) {
			cPos = cPos + 1;
         }
      break;
   }
   
   return cPos;

}

/**
 * Mover la ficha de la casilla donde está el cursor (cPos), recibida como parámetro,
 * a la casilla donde se encuentra el espacio (sPos), recibidos como parámetro, si están
 * de lado (arriba, abajo, izquierda o derecha).
 *
 * Si la casilla donde está el espacio está al lado de la casilla donde
 * está el cursor en la matriz (tiles):
 * - Mover la ficha donde está el cursor en la casilla donde está el espacio y
 *   poner el espacio en la casilla está el cursor, en la matriz (tiles).
 * Si no, no hacer el movimiento y poner (status=2), recibida como parámetro.
 *
 * Devolver la variable (status).
 *
 * No se debe mostrar la matriz con los cambios, se hace en updateBoardP2_C().
 *
 * Variables globales utilizadas:
 * (tiles) : Matriz donde guardamos las fichas del juego.
 *
 * Parámetros de entrada:
 * rdi (edi): (cPos)  : Posición del cursor en la matriz (tiles).
 * rsi (esi): (sPos)  : Posición del espacio en la matriz (tiles).
 * rdx (dx) : (status): Estado del juego.
 *
 * Parámetros de salida:
 * rax (ax) : (status): Estado del juego.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'moveTileP2, 
 * el paso de parámetros es equivalente.
 */
short moveTileP2_C(int cPos, int sPos, short status){

   int   rowCur  =(cPos/DimMatrix);
   int   colCur  =(cPos%DimMatrix);
   int   rowSpace=(sPos/DimMatrix);
   int   colSpace=(sPos%DimMatrix);
   
   if (  ( (rowCur==rowSpace) && ( (colCur==(colSpace-1)) || (colCur==(colSpace+1)) ) )
      || ( (colCur==colSpace) && ( (rowCur==(rowSpace-1)) || (rowCur==(rowSpace+1)) ) )  ){	 

      tiles[rowSpace][colSpace]	= tiles[rowCur][colCur];
      tiles[rowCur][colCur]		= ' ';
      
   } else {
      status = 2;
   }

   return status;
  
}


/**
 * Verificar si hemos terminado el juego.
 * Comprobar si la matriz (tiles) es igual a la matriz objetivo (tilesEnd).
 * Recorrer toda la matriz (tilas) por filas de izquierda a derecha y de arriba abajo
 * comparando cada posición con la misma posición de la matriz (tilesEnd).
 * Si hay una posición diferente (sorted=0) y detener la búsqueda.
 * Si se ha recorrido toda la matriz y todas las posiciones son iguales
 * (sorted=1), poner (status = 4), recibido como paràmetro, para indicar 
 * que se ha ganado, si no, mirar si se han agotado los movimientos (m == 0)
 * recibido cómo paràmetro, si se han agotado poner (status=3), recibido cómo paràmetro.
 * Retornar (status).
 *
 * Variables globales utilizadas:
 * (tiles)   : Matriz donde guardamos las fichas del juego.
 * (tilesEnd): Matriz donde guardamos las fichas en la posición que queremos conseguir.
 *
 * Parámetros de entrada:
 * rdi (rdi): (m)     : Movimientos que quedan para ordenar las fichas.
 * rsi (si) : (status): Estado del juego.
 *
 * Parámetros de salida:
 * rax (ax): (status): Estado del juego.
 *                     1: Continuamos jugando.
 *                     3: Pierdes, No quedan movimientos.
 *                     4: Ganas, todas las fichas están ordenadas.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'checkEndP2', 
 * el paso de parámetros es equivalente.
 */
short checkEndP2_C(long m, short status) {
   
   int  sorted = 1;
   int  i=0, j=0, k=0;
   
   while ((k<SizeMatrix) && (sorted==1)){
      i = k / DimMatrix;   // En ensamblador no es necesario
      j = k % DimMatrix;   // calcular la 'i' y la 'j'. Utilitzamos 'k'.
      if (tiles[i][j]!=tilesEnd[i][j]) {
         sorted=0;
      }
      k++;
   }
   if (sorted == 1){
      status = 4; 
   } else if (m == 0) {
      status = 3;
   }

   return status;
   
}


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (status), recibida como parámetro.
 * status: 0: Salir, hemos pulsado la tecla 'ESC' para salir.
 *         1: Continuamos jugando.
 *         2: No se ha podido hacer el movimiento.
 *         3: Pierdes, No quedan movimientos.
 *         4: Gana, todas las fichas están ordenadas.
 *         5: Error, no se ha encontrado el espacio.
 * 
 * Variables globales utilizadas:   
 * Ninguna.
 * 
 * Parámetros de entrada: 
 * rdi (dil): (status): Estado del juego.
 *          
 * Parámetros de salida : 
 * Ninguno
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printMessageP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void printMessageP2_C(short status) {

  gotoxyP2_C(20, 3);
   
   switch(status){
      case 0:
         printf("<<<<<<< EXIT: (ESC) Pressed >>>>>>>");
      break;
      case 1:
         printf("===========  NEXT MOVE  ===========");
      break;
      case 2:
         printf("**********  CAN'T  MOVE  **********");
      break;
      case 3:
         printf("------ SORRY, NO MORE MOVES! ------");
      break;
      case 4:
         printf("++++++++ SORTED!!! YOU WIN ++++++++");
      break;
      case 5:
         printf("xxxxxxxx ERROR -> NO SPACE xxxxxxxx");
      break;
   }
   
}


/**
 * Juego del 8-PUZZLE.
 * Función principal del juego.
 * Permite jugar al juego del 8-PUZZLE llamando todas las funcionalidades.
 * 
 * Pseudo-código:
 * Mostrar el tablero de juego llamado a la función printBoardP2_C.
 * Inicializar el estado del juego, (state=1).
 * Inicializar los movimientos que se pueden realizar (muevas = 9).
 * Inicializar la matriz (tiles) con los valores de la matriz (tilesIni)
 * llamando a la función copyMatrixP2_C.
 * Actualizar el tablero de juego llamando a la función updateBoardP2_C.
 * Busca dónde está el espacio en blanco dentro de la matriz (tiles) llamando
 * a la función getSpacePosP2_C.
 * Si sólo hay un espacio (spacePos<sizeMatrix) posicionar el
 * cursor en el lugar donde está el espacio (cursorPos = spacePos),
 * si no, poner (state=5) y mostrar el mensaje para
 * indicarlo llamando a la función printMessageP2_C.
 * Mientras (state==1) hacer:
 *   Posicionar el cursor dentro del panel llamando a la función posCurScreenP2_C.
 *   Leer una tecla (charac) llamando la función getchP2_C.
 *   Según la tecla leída haremos lo siguiente:
 *    - ['i','j','k' o 'l'] desplazar el cursor según la dirección
 *      elegida llamando a la función moveTileP2_C.
 *    - [' '] Mover la ficha de la casilla donde está el cursor gritando
 *      la función moveTileP2_C, si se puede hacer el movimiento (state!=2)
 *      poner el espacio donde ya está el cursor (spacePos=cursorPos)
 *      y decrementar los movimientos que quedan (moves).
 *    - '<ESC>' (código ASCII 27) poner (state = 0) para salir.
 *   Actualizar el tablero de juego llamando a la función updateBoardP2_C.
 *   Verificar si se ha agotado los movimientos llamando la función checkEndP2_C.
 *   Mostrar un mensaje debajo del tablero según el valor de la
 *   variable (state) llamando a la función printMessageP2_C.
 *   Si no se ha podido hacer movimiento (state == 2) poner
 *   (state = 1) para continuar jugando.
 * Fin mientras.
 *
 * Antes de salir, esperar a que se pulse una tecla llamando la función getchP2_C para terminar.
 * 
 * Variables globals utilitzades:   
 * (tiles)   : Matriz donde guardamos las fichas del juego.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'playP2', para  
 * llamar les subrutinas del juego definidas en ensamblador .
  */
void playP2_C(){
	
   int spacePos = 0; //Posición del espacio dentro de la matriz tiles (3x3),
                     //spacePos [0 : (SizeMatrix-1)].
                     //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                     //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
   int cursorPos = 0;//Posición del cursor dentro de la matriz tiles (3x3),
                     //spacePos [0 : (SizeMatrix-1)].
                     //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                     //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
   
   short state = 1;  //0: Salir, hemos pulsado la tecla 'ESC' para salir.
                     //1: Continuamos jugando.
                     //2: No se ha podido hacer el movimiento.
                     //3: Pierdes, no quedan movimientos.
                     //4: Gana, todas las fichas están ordenadas.
                     //5: Error, no se ha encontrado el espacio.
   
   long moves = 9;   //Movimientos que quedan para ordenar les fichas.
   
   char charac;
   
   printBoardP2_C();
   state = 1;
   moves = 9;
   copyMatrixP2_C(tiles);
   updateBoardP2_C(tiles, moves);
   spacePos = getSpacePosP2_C(tiles);
   if (spacePos < SizeMatrix) {
      cursorPos = spacePos;  
   } else {
      state = 5;
      printMessageP2_C(state);
   }
   while (state == 1) {     //Bucle principal.
	   
	  posCurScreenP2_C(cursorPos);   

      charac = getchP2_C();                   
      if (charac >= 'i' && charac <= 'l') {
		 cursorPos = moveCursorP2_C(charac, cursorPos);
      } else if (charac == ' ') {
		  state = moveTileP2_C(cursorPos, spacePos, state);
   	      if (state != 2) {
              spacePos = cursorPos; 
              moves--;
          }
      } else if (charac == 27) {
            state = 0;
      }
      
      updateBoardP2_C(tiles, moves);
      state = checkEndP2_C(moves, state);
      printMessageP2_C(state);
      if (state == 2) state = 1; 
      
   }
   getchP2_C(); 
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓN: Podéis probar la funcionalidad de las subrutinas que se tienen
 * que desarrollar eliminando los comentarios de la llamada a la función 
 * equivalente implementada en C que hay debajo de cada opción.
 * Para al juego completo hay una opción para la versión en ensamblador y 
 * una opción para el juego en C.
 */
int main(void){   

   int spacePos = 0;//Posición del espacio dentro de la matriz tiles (3x3),
                    //spacePos [0 : (SizeMatrix-1)].
                    //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                    //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
   int cursorPos;   //Posición del cursor dentro de la matriz tiles (3x3),
                    //spacePos [0 : (SizeMatrix-1)].
                    //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                    //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
   
   char state = '1';//0: Salir, hemos pulsado la tecla 'ESC' para salir.
                    //1: Continuamos jugando.
                    //2: No se ha podido hacer el movimiento.
                    //3: Pierdes, no quedan movimientos.
                    //5': Error, no se ha encontrado el espacio.
   
   int moves = 9;   //Movimientos que quedan para ordenar les fichas.
   
   char charac;
   
   char op = '9';
   
   while (op != '0') {
     op = printMenuP2_C();   
     switch(op){
        case '1'://Copiar la matriz tilesIni a tiles.
          printBoardP2_C();
          //=======================================================
            copyMatrixP2(tiles);
            //copyMatrixP2_C(tiles); 
          //=======================================================
          updateBoardP2_C(tiles, moves);
          gotoxyP2_C(20, 12);
          printf("Press any key ...");
          getchP2_C();
        break;
        case '2'://Actualitzar el contenido del tablero.  
          printBoardP2_C();      
          //=======================================================
            updateBoardP2(tiles, moves);
            //updateBoardP2_C(tiles, moves);
          //=======================================================
          gotoxyP2_C(20, 12);
          printf("Press any key ...");
          getchP2_C();
        break;
        case '3': //Buscar donde está el espacio en blanco dentro de la matriu (tiles).
          printBoardP2_C();
          updateBoardP2_C(tiles, moves);
          //=======================================================        
            spacePos = getSpacePosP2(tiles);
            //spacePos = getSpacePosP2_C(tiles);
          //=======================================================
          if (spacePos < SizeMatrix) {
			 gotoxyP2_C(20, 12);
             printf("Press any key ...");
             cursorPos = spacePos; 
             posCurScreenP2_C(cursorPos);
          } else {
             gotoxyP2_C(20, 3);
             printf("ERROR->ONLY 1 SPACE: Press any key ");
          } 
          getchP2_C();
        break;
        case '4'://Posicionar el cursor donde hay el espacio.  
          printBoardP2_C();
          updateBoardP2_C(tiles, moves);
          spacePos = getSpacePosP2_C(tiles);
          cursorPos = spacePos;
          gotoxyP2_C(20,12);
          printf("Press any key ..."); 
          //=======================================================
             posCurScreenP2(cursorPos);
             //posCurScreenP2_C(cursorPos);
          //=======================================================
          getchP2_C();
        break;
        case '5'://Mover el cursor en la dirección indicada.  
          printBoardP2_C();
          gotoxyP2_C(20, 12);
          printf("Press i,j,k,l: ");
          updateBoardP2_C(tiles, moves);
          spacePos = getSpacePosP2_C(tiles);
          cursorPos = spacePos;
          posCurScreenP2_C(cursorPos);
          charac = getchP2_C();   
          if (charac>='i' && charac<='l') {
          //=======================================================
             cursorPos = moveCursorP2(charac, cursorPos);
             //cursorPos = moveCursorP2_C(charac, cursorPos);
          //=======================================================
          }
          gotoxyP2_C(20,12);
          printf(" Press any key ...");
          posCurScreenP2_C(cursorPos);
          getchP2_C();
        break;
        case '6'://Mover la fitxa donde está el espacio.
          printBoardP2_C();
          gotoxyP2_C(20, 12);
          printf("Press <SPACE>: ");
          updateBoardP2_C(tiles, moves);
          spacePos = getSpacePosP2_C(tiles);
          posCurScreenP2_C(cursorPos);
          charac = getchP2_C();   
          if (charac>=' ') {
          //=======================================================
             state = moveTileP2(cursorPos, spacePos, state);
             //state = moveTileP2_C(cursorPos, spacePos, state);
          //=======================================================
	      }
          updateBoardP2_C(tiles, moves);
          if (state != 2) {
             gotoxyP2_C(20,12);
             printf(" Press any key ...");
             spacePos = cursorPos;
             posCurScreenP2_C(cursorPos);
          } else {			 
             printMessageP2_C(state);
          }
          getchP2_C();
        break;
        case '7': //Verificar el estado del juego.
          printBoardP2_C();    
          moves = 5;
          state = 1;
          ///moves = 0; //-> state=3 (NO MORE MOVES)
          updateBoardP2_C(tiles, moves);
          posCurScreenP2_C(cursorPos);
          //=======================================================
            state = checkEndP2(moves, state);
            //state = checkEndP2_C(moves, state);
          //=======================================================
          printMessageP2_C(state);
          gotoxyP2_C(26 , 12);
          printf(" Press any key ...");
          getchP2_C();  
        break;
        case '8': //Joc complet en Assemblador. 
          //=======================================================
          playP2();
          //=======================================================
        break;
        case '9': //Joc complet en C.    
          //=======================================================
          playP2_C();
          //=======================================================
        break;
      }
   }
   printf("\n\n");
   
   return 0;
  
}
