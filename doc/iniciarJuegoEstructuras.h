#include <stdio.h>
#include <stdlib.h>  //rand(), system()
#include <time.h>    // time()
#include <ctype.h>   // toupper()
#include <string.h>  // srtdup()
#include <unistd.h>  // sleep()


// Numero de cartas de la baraja
#define NUM_CARTAS_BARAJA 48

// Numero de pilas en el juego
#define NUM_PILAS 8


// Identificadores de las pilas
#define MAZO     0 	    
#define COLUMNA1 1
#define COLUMNA2 2
#define COLUMNA3 3
#define COLUMNA4 4
#define COLUMNA5 5
#define COLUMNA6 6
#define COLUMNA7 7 
#define SALIDA8 8
#define SALIDA9 9
#define SALIDA10 10
#define SALIDA11 11
#define DESCARTE 12 


// Palos de la baraja
#define BASTOS 	0
#define COPAS	1
#define ESPADAS	2
#define OROS	3

// Valores de las cartas
#define AS	1
#define DOS	2
#define TRES	3
#define CUATRO	4
#define CINCO	5
#define SEIS	6
#define SIETE	7
#define OCHO	8
#define NUEVE	9
#define SOTA	10
#define CABALLO	11
#define REY	12

// Estado de las cartas
#define BOCA_ABAJO 	0
#define BOCA_ARRIBA	1
#define VACIA		2


// Estructura de datos que representa a una carta
struct s_carta
{
	int valor;     	// Valor de la carta, de 1 a 12 
	int palo;  	// Palo de la carta (0 = bastos, 1 = copas, 2 = espadas, 3 = oros)
	int estado; 	// Indica si la carta debe mostrase boca abajo (0) o boca arriba (1), o si no debe mostrarse por estar vacia (2)
};

typedef struct s_carta t_carta;
typedef struct s_carta * t_pCarta;


// Estructura de datos que representa a una pila
struct s_pila
{
	char *cabecera; // Nombre la pila
	int numCartas;     // Numero de cartas que hay en la pila
	t_carta set[NUM_CARTAS_BARAJA];   // Conjunto de cartas que hay en la pila
};

typedef struct s_pila t_pila;
typedef struct s_pila * t_pPila;

// Estructura de datos que representa a todo el juego
struct s_juego
{
	int contNumVueltas;
  char *text;
  char *text2;
  char *aviso;
	t_pila mazo;
	t_pila columna1;
	t_pila columna2;
	t_pila columna3;
	t_pila columna4;
	t_pila columna5;
	t_pila columna6;
	t_pila columna7;
	t_pila salida8;
	t_pila salida9;
	t_pila salida10;
	t_pila salida11;
  t_pila descarte;
};

typedef struct s_juego t_juego;
typedef struct s_juego * t_pJuego;


/* Esta funcion recibe como parametro un puntero a juego e inicializa todas las pilas
   Las pilas se inicializan sin cartas. Es decir, el conjunto SET, estara vacio.
   Una variable de tipo carta se considera vacia si el campo estado vale VACIA (2)
*/
int iniciarPartida(t_pJuego xogo);

// Esta funcion recibe como parametro un puntero a juego, y se encarga de inicializar la pila mazo
int barajar(t_pJuego pXogo);

/* Esta funcion recibe como parametro un puntero a juego, y se encarga de repartir las cartas 
 entre las columnas, 1 en la primera, 2 en la segunda y 3 en la tercera. 
 Solo la carta que esta en la cima de las pilas columna se muestra bocaArriba */
int repartirCartas(t_pJuego pXogo);

// Esta funcion muestra el estado de las pilas en pantalla
int mostrarCartas(t_juego xogo);
int iniciarPartida(t_pJuego pXogo);
int barajar(t_pJuego pXogo);
int repartirCartas(t_pJuego pXogo);
int mostrarCartas(t_juego xogo);
int pila_vacia(t_pila *pila);
t_pila *num_pila(t_juego *xogo, int num_pila);
FILE *gfopen(t_juego *xogo,char *nom_fich, char *modo);
void guardar_juego(t_juego *xogo);
/* Control del puntero de pantalla: gotoxy(X,Y), donde los argumentos X e Y son las coordenadas del puntero
de pantalla. En otras palabras, la coordenada X es el n<FA>mero de columna y la cordenada Y es el n<FA>mero de fila. */
#define gotoxy(x,y) printf("\x1b[%d;%dH",(y),(x))

//Establecer atributos en pantalla: atributo(X), donde el argumento X representa el atributo [eg. X=1 (negrita), X=7 (v<ED>deo inverso)].
#define atributo(x) printf("\x1b[%dm",(x)) 

//Elimina el atributo o atributos establecidos: no_atributo(), macro sin argumentos.
#define no_atributo() printf("\x1b[0m");

int iniciarPartida(t_juego *pxogo)
{
	int i;

	// Inicializamos el contador del juego
	pxogo->contNumVueltas=0; 
  pxogo->aviso=strdup(" ");
	
	// Inicializamos los nombres de las cabeceras de las pilas
	pxogo->mazo.cabecera=strdup("MAZO");
	pxogo->columna1.cabecera=strdup("COLUMNA1");
	pxogo->columna2.cabecera=strdup("COLUMNA2");
	pxogo->columna3.cabecera=strdup("COLUMNA3");
	pxogo->columna4.cabecera=strdup("COLUMNA4");
	pxogo->columna5.cabecera=strdup("COLUMNA5");
	pxogo->columna6.cabecera=strdup("COLUMNA6");
	pxogo->columna7.cabecera=strdup("COLUMNA7");
	pxogo->salida8.cabecera=strdup("SALIDA8");
	pxogo->salida9.cabecera=strdup("SALIDA9");
	pxogo->salida10.cabecera=strdup("SALIDA10");	
	pxogo->salida11.cabecera=strdup("SALIDA11");
  pxogo->descarte.cabecera=strdup("DESCARTE");	
  // Inicializamos los conjuntos de cartas
	pxogo->mazo.numCartas=0;  
  pxogo->columna1.numCartas=0;
  pxogo->columna2.numCartas=0;
  pxogo->columna3.numCartas=0;
  pxogo->columna4.numCartas=0;
  pxogo->columna5.numCartas=0;
  pxogo->columna6.numCartas=0;	
  pxogo->columna7.numCartas=0;
  pxogo->salida8.numCartas=0;
  pxogo->salida9.numCartas=0;
  pxogo->salida10.numCartas=0;
  pxogo->salida11.numCartas=0;
  pxogo->descarte.numCartas=0;

	for (i=0; i<NUM_CARTAS_BARAJA; i++)
	{
		pxogo->mazo.set[i].estado=VACIA;
		pxogo->mazo.set[i].valor=0;
		pxogo->mazo.set[i].palo=0;
		pxogo->columna1.set[i].estado=VACIA;
		pxogo->columna1.set[i].valor=0;
                pxogo->columna1.set[i].palo=0;
		pxogo->columna2.set[i].estado=VACIA;
		pxogo->columna2.set[i].valor=0;
                pxogo->columna2.set[i].palo=0;
		pxogo->columna3.set[i].estado=VACIA;
		pxogo->columna3.set[i].valor=0;
                pxogo->columna3.set[i].palo=0;
		pxogo->columna4.set[i].estado=VACIA;
		pxogo->columna4.set[i].valor=0;
                pxogo->columna4.set[i].palo=0;
		pxogo->columna5.set[i].estado=VACIA;
		pxogo->columna5.set[i].valor=0;
                pxogo->columna5.set[i].palo=0;
		pxogo->columna6.set[i].estado=VACIA;
		pxogo->columna6.set[i].valor=0;
                pxogo->columna6.set[i].palo=0;
		pxogo->columna7.set[i].estado=VACIA;
		pxogo->columna7.set[i].valor=0;
                pxogo->columna7.set[i].palo=0;
		pxogo->salida8.set[i].estado=VACIA;
		pxogo->salida8.set[i].valor=0;
                pxogo->salida8.set[i].palo=0;
		pxogo->salida9.set[i].estado=VACIA;
		pxogo->salida9.set[i].valor=0;
                pxogo->salida9.set[i].palo=0;
		pxogo->salida10.set[i].estado=VACIA;
		pxogo->salida10.set[i].valor=0;
                pxogo->salida10.set[i].palo=0;
		pxogo->salida11.set[i].estado=VACIA;
		pxogo->salida11.set[i].valor=0;
                pxogo->salida11.set[i].palo=0;
    pxogo->descarte.set[i].estado=VACIA;
		pxogo->descarte.set[i].valor=0;
                pxogo->descarte.set[i].palo=0;

	}

	return 0;
}

int barajar(t_juego *pxogo)
{
	int i,j;
	
	for (i=BASTOS; i<=OROS; i++)
	{
		for (j=AS-1; j<REY; j++)	
		{
			pxogo->mazo.set[i*12+j].valor=j;
			pxogo->mazo.set[i*12+j].palo=i;
			pxogo->mazo.set[i*12+j].estado=BOCA_ABAJO;
			pxogo->mazo.numCartas++;
		}
	}

	return 0;
}

int repartirCartas(t_juego *pxogo)
{
        int i;
	int numCartasOrigen;
	int numCartasDestino;
        
        pxogo->contNumVueltas = 1;   
        
 
        // Repartirmos a la columna 1

	numCartasOrigen=pxogo->mazo.numCartas;
	numCartasDestino=pxogo->columna1.numCartas;
	pxogo->columna1.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
	pxogo->columna1.set[numCartasDestino].estado=BOCA_ARRIBA;
	pxogo->columna1.numCartas++;
	pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
	pxogo->mazo.numCartas--;
        
	// Repartimos a la columna 2	
	
	for (i=0; i<=1; i++)
	{
		numCartasOrigen=pxogo->mazo.numCartas;
	        numCartasDestino=pxogo->columna2.numCartas;
        	pxogo->columna2.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
        	pxogo->columna2.numCartas++;
		pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
        	pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna2.numCartas;
	 pxogo->columna2.set[numCartasDestino-1].estado=BOCA_ARRIBA;

	 // Repartimos a la columna 3    

        for (i=0; i<=2; i++)
        {
                numCartasOrigen=pxogo->mazo.numCartas;
                numCartasDestino=pxogo->columna3.numCartas;
                pxogo->columna3.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
                pxogo->columna3.numCartas++;
		            pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
                pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna3.numCartas;
         pxogo->columna3.set[numCartasDestino-1].estado=BOCA_ARRIBA;
        
	 // Repartimos a la columna 4    

        for (i=0; i<=3; i++)
        {
                numCartasOrigen=pxogo->mazo.numCartas;
                numCartasDestino=pxogo->columna4.numCartas;
                pxogo->columna4.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
                pxogo->columna4.numCartas++;
		pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
                pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna4.numCartas;
         pxogo->columna4.set[numCartasDestino-1].estado=BOCA_ARRIBA;
        
	
	 // Repartimos a la columna 5    

       	for (i=0; i<=4; i++)
        {
                numCartasOrigen=pxogo->mazo.numCartas;
                numCartasDestino=pxogo->columna5.numCartas;
                pxogo->columna5.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
                pxogo->columna5.numCartas++;
		pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
                pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna5.numCartas;
         pxogo->columna5.set[numCartasDestino-1].estado=BOCA_ARRIBA;
        
	
	 // Repartimos a la columna 6    

        for (i=0; i<=5; i++)
        {
                numCartasOrigen=pxogo->mazo.numCartas;
                numCartasDestino=pxogo->columna6.numCartas;
                pxogo->columna6.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
                pxogo->columna6.numCartas++;
		pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
                pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna6.numCartas;
         pxogo->columna6.set[numCartasDestino-1].estado=BOCA_ARRIBA;

	 // Repartimos a la columna 7    

        for (i=0; i<=6; i++)
        {
                numCartasOrigen=pxogo->mazo.numCartas;
                numCartasDestino=pxogo->columna7.numCartas;
                pxogo->columna7.set[numCartasDestino]=pxogo->mazo.set[numCartasOrigen-1];
                pxogo->columna7.numCartas++;
		pxogo->mazo.set[numCartasOrigen-1].estado=VACIA;
                pxogo->mazo.numCartas--;
       }
	 numCartasDestino=pxogo->columna7.numCartas;
         pxogo->columna7.set[numCartasDestino-1].estado=BOCA_ARRIBA;
        
        return 0;
}

int mostrarCartas(t_juego xogo)
{

	int i;

	// Borra la pantalla
	system("clear");    

	// Pinta el nombre la pila MAZO y CONT
        gotoxy(5,3);  atributo(1); printf("%s (%d)",xogo.mazo.cabecera, xogo.mazo.numCartas); no_atributo()
        gotoxy(25,3); atributo(1); printf("CONT"); no_atributo();
        

        // Pinta la carta en la cima de las pila MAZO, el valor de CONTADOR
        gotoxy(5,5);  
        if(pila_vacia(&xogo.mazo))
          printf("vacia");
        else 
          printf("???");
          
        gotoxy(25,5); printf("%i",xogo.contNumVueltas);
	
	// Pinta Descarte
        gotoxy(5,7); atributo(1); printf("%s", xogo.descarte.cabecera);no_atributo();
        gotoxy(25,7); if(pila_vacia(&xogo.descarte))
                          printf("vacia");
                      else   
                          switch (xogo.descarte.set[xogo.descarte.numCartas-1].palo)
                        {
				                        case BASTOS:  printf("%2d B",xogo.descarte.set[xogo.descarte.numCartas-1].valor+1); break;
                                case COPAS:   printf("%2d C",xogo.descarte.set[xogo.descarte.numCartas-1].valor+1); break;
                                case ESPADAS: printf("%2d E",xogo.descarte.set[xogo.descarte.numCartas-1].valor+1); break;
                                case OROS:    printf("%2d O",xogo.descarte.set[xogo.descarte.numCartas-1].valor+1); break;

                        }   
	
	// Pinta las pilas columna
  gotoxy(5,9);  atributo(1); printf("%s", xogo.columna1.cabecera); no_atributo();
	gotoxy(25,9); atributo(1); printf("%s", xogo.columna2.cabecera); no_atributo();
	gotoxy(45,9); atributo(1); printf("%s", xogo.columna3.cabecera); no_atributo();
	gotoxy(65,9); atributo(1); printf("%s", xogo.columna4.cabecera); no_atributo();
	gotoxy(85,9); atributo(1); printf("%s", xogo.columna5.cabecera); no_atributo();
	gotoxy(105,9); atributo(1); printf("%s", xogo.columna6.cabecera); no_atributo();
	gotoxy(125,9); atributo(1); printf("%s", xogo.columna7.cabecera); no_atributo();
	
  if(pila_vacia(&xogo.columna1))
                        gotoxy(5,11),printf("vacia");
  if(pila_vacia(&xogo.columna2))
                        gotoxy(25,11),printf("vacia");
  if(pila_vacia(&xogo.columna3))
                        gotoxy(45,11),printf("vacia");
  if(pila_vacia(&xogo.columna4))
                        gotoxy(65,11),printf("vacia");
  if(pila_vacia(&xogo.columna5))
                        gotoxy(85,11),printf("vacia");
  if(pila_vacia(&xogo.columna6))
                        gotoxy(105,11),printf("vacia");                                                                                                
  if(pila_vacia(&xogo.columna7))
                        gotoxy(125,11),printf("vacia");
                        
	for (i=0; i<xogo.columna1.numCartas; i++)
	{
        	if (xogo.columna1.set[i].estado==BOCA_ABAJO)
                {
                	gotoxy(5,(11+2*i)); printf(" ???");
                }
                else {                
                	switch (xogo.columna1.set[i].palo)
                        {
				                        case BASTOS:  gotoxy(5,11+2*i); printf("%2d B",xogo.columna1.set[i].valor+1); break;
                                case COPAS:   gotoxy(5,11+2*i); printf("%2d C",xogo.columna1.set[i].valor+1); break;
                                case ESPADAS: gotoxy(5,11+2*i); printf("%2d E",xogo.columna1.set[i].valor+1); break;
                                case OROS:    gotoxy(5,11+2*i); printf("%2d O",xogo.columna1.set[i].valor+1); break;

                        }
                    }  

	}

	for (i=0; i<xogo.columna2.numCartas; i++)
	{
                if (xogo.columna2.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(25,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna2.set[i].palo)
                        {
                                case BASTOS:  gotoxy(25,11+2*i); printf("%2d B",xogo.columna2.set[i].valor+1); break;
                                case COPAS:   gotoxy(25,11+2*i); printf("%2d C",xogo.columna2.set[i].valor+1); break;
                                case ESPADAS: gotoxy(25,11+2*i); printf("%2d E",xogo.columna2.set[i].valor+1); break;
                                case OROS:    gotoxy(25,11+2*i); printf("%2d O",xogo.columna2.set[i].valor+1); break;
                        }
                    } 

        }


	for (i=0; i<xogo.columna3.numCartas; i++)
	{
                if (xogo.columna3.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(45,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna3.set[i].palo)
                        {
                                case BASTOS:  gotoxy(45,11+2*i); printf("%2d B",xogo.columna3.set[i].valor+1); break;
                                case COPAS:   gotoxy(45,11+2*i); printf("%2d C",xogo.columna3.set[i].valor+1); break;
                                case ESPADAS: gotoxy(45,11+2*i); printf("%2d E",xogo.columna3.set[i].valor+1); break;
                                case OROS:    gotoxy(45,11+2*i); printf("%2d O",xogo.columna3.set[i].valor+1); break;
                        }
                    } 

        }
	
	for (i=0; i<xogo.columna4.numCartas; i++)
	{
                if (xogo.columna4.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(65,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna4.set[i].palo)
                        {
                                case BASTOS:  gotoxy(65,11+2*i); printf("%2d B",xogo.columna4.set[i].valor+1); break;
                                case COPAS:   gotoxy(65,11+2*i); printf("%2d C",xogo.columna4.set[i].valor+1); break;
                                case ESPADAS: gotoxy(65,11+2*i); printf("%2d E",xogo.columna4.set[i].valor+1); break;
                                case OROS:    gotoxy(65,11+2*i); printf("%2d O",xogo.columna4.set[i].valor+1); break;
                        }
                    } 

        }
	
	for (i=0; i<xogo.columna5.numCartas; i++)
	{
                if (xogo.columna5.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(85,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna5.set[i].palo)
                        {
                                case BASTOS:  gotoxy(85,11+2*i); printf("%2d B",xogo.columna5.set[i].valor+1); break;
                                case COPAS:   gotoxy(85,11+2*i); printf("%2d C",xogo.columna5.set[i].valor+1); break;
                                case ESPADAS: gotoxy(85,11+2*i); printf("%2d E",xogo.columna5.set[i].valor+1); break;
                                case OROS:    gotoxy(85,11+2*i); printf("%2d O",xogo.columna5.set[i].valor+1); break;
                        }
                    } 

        }

	for (i=0; i<xogo.columna6.numCartas; i++)	
	{
                if (xogo.columna6.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(105,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna6.set[i].palo)
                        {
                                case BASTOS:  gotoxy(105,11+2*i); printf("%2d B",xogo.columna6.set[i].valor+1); break;
                                case COPAS:   gotoxy(105,11+2*i); printf("%2d C",xogo.columna6.set[i].valor+1); break;
                                case ESPADAS: gotoxy(105,11+2*i); printf("%2d E",xogo.columna6.set[i].valor+1); break;
                                case OROS:    gotoxy(105,11+2*i); printf("%2d O",xogo.columna6.set[i].valor+1); break;
                        }
                    } 

        }

	for (i=0; i<xogo.columna7.numCartas; i++)	
	{
                if (xogo.columna7.set[i].estado==BOCA_ABAJO)
                {
                        gotoxy(125,(11+2*i)); printf(" ???");
                }
                else {                  
                        switch (xogo.columna7.set[i].palo)
                        {
                                case BASTOS:  gotoxy(125,11+2*i); printf("%2d B",xogo.columna7.set[i].valor+1); break;
                                case COPAS:   gotoxy(125,11+2*i); printf("%2d C",xogo.columna7.set[i].valor+1); break;
                                case ESPADAS: gotoxy(125,11+2*i); printf("%2d E",xogo.columna7.set[i].valor+1); break;
                                case OROS:    gotoxy(125,11+2*i); printf("%2d O",xogo.columna7.set[i].valor+1); break;
                        }
                    } 

        }
        
  // Pinta salidas
        gotoxy(65,3); atributo(1); printf("%s", xogo.salida8.cabecera);no_atributo();
        gotoxy(85,3); atributo(1); printf("%s", xogo.salida9.cabecera);no_atributo();
        gotoxy(105,3); atributo(1); printf("%s", xogo.salida10.cabecera);no_atributo();
        gotoxy(125,3); atributo(1); printf("%s", xogo.salida11.cabecera);no_atributo();
        
        gotoxy(65,5); if(pila_vacia(&xogo.salida8))
                          printf("vacia");
                      else   
                          switch (xogo.salida8.set[xogo.salida8.numCartas-1].palo)
                        {
				                        case BASTOS:  printf("%2d B",xogo.salida8.set[xogo.salida8.numCartas-1].valor+1); break;
                                case COPAS:   printf("%2d C",xogo.salida8.set[xogo.salida8.numCartas-1].valor+1); break;
                                case ESPADAS: printf("%2d E",xogo.salida8.set[xogo.salida8.numCartas-1].valor+1); break;
                                case OROS:    printf("%2d O",xogo.salida8.set[xogo.salida8.numCartas-1].valor+1); break;
                        }     
         gotoxy(85,5); if(pila_vacia(&xogo.salida9))
                          printf("vacia");
                      else   
                          switch (xogo.salida9.set[xogo.salida9.numCartas-1].palo)
                        {
				                        case BASTOS:  printf("%2d B",xogo.salida9.set[xogo.salida9.numCartas-1].valor+1); break;
                                case COPAS:   printf("%2d C",xogo.salida9.set[xogo.salida9.numCartas-1].valor+1); break;
                                case ESPADAS: printf("%2d E",xogo.salida9.set[xogo.salida9.numCartas-1].valor+1); break;
                                case OROS:    printf("%2d O",xogo.salida9.set[xogo.salida9.numCartas-1].valor+1); break;
                        }    
          gotoxy(105,5); if(pila_vacia(&xogo.salida10))
                          printf("vacia");
                      else   
                          switch (xogo.salida10.set[xogo.salida10.numCartas-1].palo)
                        {
				                        case BASTOS:  printf("%2d B",xogo.salida10.set[xogo.salida10.numCartas-1].valor+1); break;
                                case COPAS:   printf("%2d C",xogo.salida10.set[xogo.salida10.numCartas-1].valor+1); break;
                                case ESPADAS: printf("%2d E",xogo.salida10.set[xogo.salida10.numCartas-1].valor+1); break;
                                case OROS:    printf("%2d O",xogo.salida10.set[xogo.salida10.numCartas-1].valor+1); break;
                        }        
           gotoxy(125,5); if(pila_vacia(&xogo.salida11))
                          printf("vacia");
                      else   
                          switch (xogo.salida11.set[xogo.salida11.numCartas-1].palo)
                        {
				                        case BASTOS:  printf("%2d B",xogo.salida11.set[xogo.salida11.numCartas-1].valor+1); break;
                                case COPAS:   printf("%2d C",xogo.salida11.set[xogo.salida11.numCartas-1].valor+1); break;
                                case ESPADAS: printf("%2d E",xogo.salida11.set[xogo.salida11.numCartas-1].valor+1); break;
                                case OROS:    printf("%2d O",xogo.salida11.set[xogo.salida11.numCartas-1].valor+1); break;
                        }     
  gotoxy(5,39); printf("%s",xogo.aviso);
	gotoxy(1,34); printf("%s",xogo.text2);
  gotoxy(1,30); printf("%s",xogo.text); 
  strcpy(xogo.aviso," ");
	return 0;
}


int pila_vacia(t_pila *pila)     // si la pila esta vacia devuelve 1
{
  if(pila->numCartas == 0) 
  return 1;
  else
  return 0;
}

int pila_llena(t_pila *pila)     // si la pila esta vacia devuelve 1
{
  if(pila->numCartas == 12) 
  return 1;
  else
  return 0;
}

t_pila *num_pila(t_juego *xogo, int num_pila)  //devuelve puntero a la pila segun el numero introducido
{
  t_pila *pila;
  switch(num_pila)
  {
    case 0: pila=&xogo->mazo; 
      return pila;
      break;
    case 1: pila=&xogo->columna1;
      return pila;
      break;
    case 2: pila=&xogo->columna2;
      return pila;
      break;
    case 3: pila=&xogo->columna3;
      return pila;
      break;
    case 4: pila=&xogo->columna4;
      return pila;
      break;
    case 5: pila=&xogo->columna5;
      return pila;
      break;
    case 6: pila=&xogo->columna6;
      return pila;
      break;
    case 7: pila=&xogo->columna7;
      return pila;
      break;
    case 8: pila=&xogo->salida8;
      return pila;
      break;
    case 9: pila=&xogo->salida9;
      return pila;
      break;
    case 10: pila=&xogo->salida10;
      return pila;
      break;
    case 11: pila=&xogo->salida11;
      return pila;
      break;
    case 12: pila=&xogo->descarte;
      return pila;
      break;
  }
}


//Esta funci\F3n implementa la funcionalidad Guardar juego.
void guardar_juego(t_juego *xogo)
{
  if(xogo->contNumVueltas==0)
  {
     xogo->aviso=strdup("AVISO: NO EXISTE JUEGO QUE GUARDAR");
  }
  else
  {
    FILE *pf;
    char file[]="juego_guardado.txt";
    char modo[]="wb";
    pf = gfopen(xogo,file, modo); /* El fichero se abre en binario para escritura.(ver funci\F3n gfopen) */
    fwrite(xogo, sizeof(t_juego), 1, pf); //Se pasa el puntero pila.
    fclose(pf); //Cierra el flujo asociado al fichero (simplemente, cierra el fichero).
    //scanf("%*c"); //El car\E1cter * permite leer el car\E1cter \91\n\92 del ENTER y no asignarlo.
    xogo->aviso=strdup("AVISO: JUEGO GUARDADO\n");
  }
}
 //Esta funci\F3n no funciona!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void recuperar_baraja(t_juego *xogo)
{
  FILE *pf;
  char file[]="juego_guardado.txt";
  char modo[]="rb";
  pf = gfopen(xogo,file, modo); /* El fichero se abre en binario para leer.(ver funci\F3n gfopen) */
  fread(xogo, sizeof(t_juego), 1, pf); //Se pasa el puntero juego.
  fclose(pf); //Cierra el flujo asociado al fichero (simplemente, cierra el fichero).
 }  
  
/* ********Definici\F3n de la funci\F3n general gfopen**************************
Esta funci\F3n determina si se produce error en la apertura de un fichero.
Utiliza la funci\F3n fopen de la librer\EDa stdio.h.
******************************************************************** */
FILE *gfopen(t_juego *xogo,char *nom_fich, char *modo)
{
  FILE *pf;
  char cad[50];
  if((pf = fopen(nom_fich, modo)) == NULL)
  {
    sprintf(cad,"ERROR EN LA APERTURA DEL FICHERO %s", nom_fich );
    xogo->aviso=strdup(cad);
    exit(1);
  }
  return pf;
}
