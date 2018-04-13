#include "iniciarJuegoEstructuras.h"


void recuperar_baraja(t_juego *xogo);
void barajar_cartas(t_juego *xogo);
void mover_carta(t_juego *xogo, int nro_pila_origen, int nro_pila_destino, int num_cartas);
void juego_nuevo(t_juego *xogo);
void menu(t_juego *xogo);
void recuperar_juego(t_juego *pxogo);
int leer_mov(t_juego *pxogo, char str[], int *nro_pila_origen, int *nro_pila_destino, int *num_cartas);
void clear_buffer(void);
int pila_llena(t_pila *pila);
int estado_juego(t_juego *pxogo);
int movimiento_valido(t_juego *pxogo, int pila_origen, int pila_destino,int num_cartas);
int mov_descarte_columnas_valido(t_juego *pxogo, int nro_pila_origen, int nro_pila_destino);
int mov_mazo_descarte_valido(t_juego *pxogo,int num_cartas,int nro_pila_origen);
int mov_columnas_columnas_valido(t_juego *pxogo, int nro_pila_origen, int nro_pila_destino, int num_cartas);
int mov_descarte_mazo_valido(t_juego *pxogo, int nro_pila_destino);



int main ()
{
	t_juego xogo;
  iniciarPartida(&xogo);
	barajar(&xogo);
  char aviso[90];
  do
  {
    menu(&xogo); 
  }
  while(1);
  
	return 0;
}
        


void barajar_cartas(t_juego *xogo)
{
  int i, j, k;
  t_carta *p_carta, *p_almacen; /* Punteros utilizados en el intercambio de cartas al barajarlas de forma seudo aleatoria. */
  //Cargar la baraja en la pila mazo
  //Barajar las cartas de la pila mazo.
  p_carta = p_almacen = &xogo->mazo.set[0]; /* Se inicializan los punteros p_carta y p_almacen con
                                          la dirección de la posición 0 del array mazo. También: p_carta = p_almacen = &(pila->mazo[0]); */
  srand((unsigned)time(NULL));
  for(k = 1; k <= 500; k++)               //Se establece que se realicen 500 intercambios aleatorios de cartas.
  { 
    i = random()%48;
    j = random()%48;
    if(i != j)
    {
      p_almacen[49] = p_carta[i];
      p_carta[i] = p_carta[j];
      p_carta[j] = p_almacen[49]; /* Se utiliza la posición 49 del array que implementa la pila mazo como almacén de intercambio */
    }
  }
}

void mover_carta(t_juego *xogo, int nro_pila_origen, int nro_pila_destino, int num_cartas)
{
  int ii, numCartasOrigen, numCartasDestino;
  t_pila *pila_origen;
  t_pila *pila_destino;
  
  pila_origen=num_pila(xogo,nro_pila_origen);   // apuntamos a la pila origen
  pila_destino=num_pila(xogo,nro_pila_destino); // apuntamos a la pila destino
  
  numCartasOrigen=pila_origen->numCartas;     //obtenemos numeros de cartas
  numCartasDestino=pila_destino->numCartas;
  
  if (nro_pila_origen==12&&nro_pila_destino==0)
  {
    for(ii=0;ii<numCartasOrigen;ii++)
    {
      pila_destino->set[ii]=pila_origen->set[numCartasOrigen-ii-1];
      
      pila_origen->set[numCartasOrigen-ii-1].estado=VACIA;        // borar las cartas de la pila origen
      pila_origen->set[numCartasOrigen-ii-1].palo=0;
      pila_origen->set[numCartasOrigen-ii-1].valor=VACIA;
        
      numCartasDestino++; 
    }
  
    pila_origen->numCartas=0;        //cambiamos numeros de cartas en las pilas
    pila_destino->numCartas=numCartasDestino;

    xogo->contNumVueltas++;
  }
  else
  {
    if(nro_pila_destino==12)
      pila_origen->set[numCartasOrigen-1].estado=BOCA_ARRIBA;   //en caso de movimiento hacía a descarte damos la vuelta
    
    for(ii=num_cartas;ii>0;ii--)
    {
        pila_destino->set[numCartasDestino]=pila_origen->set[numCartasOrigen-ii];    //copiar las cartas de la pila origen a destino
        
        pila_origen->set[numCartasOrigen-ii].estado=VACIA;        // borar las cartas de la pila origen
        pila_origen->set[numCartasOrigen-ii].palo=0;
        pila_origen->set[numCartasOrigen-ii].valor=VACIA;
        
        numCartasDestino++;         
    }
    numCartasOrigen=numCartasOrigen-num_cartas;
  
    pila_origen->numCartas=numCartasOrigen;        //cambiamos numeros de cartas en las pilas
    pila_destino->numCartas=numCartasDestino;
  
    if (pila_vacia(pila_origen)==0)                               // damos vuelta a la ultima carta
      pila_origen->set[numCartasOrigen-1].estado=BOCA_ARRIBA;  
  }
  
  
}

void menu(t_juego *pxogo)
{
  char opt;       // option del menu
  pxogo->text=strdup("\n\n\t 1. Iniciar nuevo juego. \n\t 2. Recuperar juego guardado. \
\n\t 3. Guardar juego. \n\t 4. Salir del juego.\n\t\t");
  pxogo->text2=strdup("\t\v\n\n\n (Elegir una opcion pulsando 1,2,3 o 4)\n");
  mostrarCartas(*pxogo);
  scanf("%c", &opt);     
  switch (opt)
    {
    case '1':
          juego_nuevo(pxogo);
      break;
    case '2':
          recuperar_juego(pxogo);
      break;
    case '3':
          guardar_juego(pxogo);
          clear_buffer();	  
      break;
    case '4':
      system("clear");
      exit(1);
      break;
    default:
      pxogo->aviso=strdup(" ERROR: ELECCION INCORRECTA, PULSAR 1, 2, 3, o 4"); 
     break; 
    }
}

void recuperar_juego(t_juego *pxogo)
{
  int pila_origen, pila_destino, num_cartas, readCheck; 
  int res, estado,valido;
  char str[10];

  /*iniciarPartida(pxogo);
	barajar(pxogo);
  barajar_cartas(pxogo);
	repartirCartas(pxogo);
                        */
  recuperar_baraja(pxogo);
  clear_buffer();
  do
  {
    pxogo->text=strdup("\n\n\n\n\vINDICAR MOVIMIENTO ENTRE LAS PILAS DE CARTAS: <nro_pila_origen> <nro_pila_destino> [<num_cartas>] \nMOVIMIENTO: ");
     pxogo->text2=strdup("\n\n\v(Para volver al menu pulsar m o M)\n");
     mostrarCartas(*pxogo); 
     readCheck=scanf ("%[^\n]%*c", str);
     if(readCheck==0)
     {
          strcpy(str,"99");
          pxogo->aviso=strdup("ERROR: ENTRADA INCORRECTA\n"); 
          clear_buffer();
          continue;          
     }  
     res=leer_mov(pxogo, str, &pila_origen, &pila_destino, &num_cartas);
     switch (res)
     {
      case 0:
       break;
      case 1: valido=movimiento_valido(pxogo, pila_origen, pila_destino, num_cartas);
              if (valido==1)
              {
              mover_carta(pxogo, pila_origen, pila_destino, num_cartas);
              estado=estado_juego(pxogo);
              }
       break;
      case 2:
       break;
     }
  }
  while(res!=2&&estado!=1);	
  //mostrarCartas(*pxogo);
} 
 

void juego_nuevo(t_juego *pxogo)
{
  int pila_origen, pila_destino, num_cartas, readCheck; 
  int res, estado,valido;
  char str[10];
  
  iniciarPartida(pxogo);
	barajar(pxogo);
  barajar_cartas(pxogo);
	repartirCartas(pxogo);  
  clear_buffer();
  do
  {
     pxogo->text=strdup("\n\n\n\n\vINDICAR MOVIMIENTO ENTRE LAS PILAS DE CARTAS: <nro_pila_origen> <nro_pila_destino> [<num_cartas>] \nMOVIMIENTO: ");
     pxogo->text2=strdup("\n\n\v(Para volver al menu pulsar m o M)\n");
     mostrarCartas(*pxogo); 
     readCheck=scanf ("%[^\n]%*c", str);
     if(readCheck==0)
     {
          strcpy(str,"99");
          pxogo->aviso=strdup("ERROR: ENTRADA INCORRECTA\n"); 
          clear_buffer();
          continue;          
     }  
     res=leer_mov(pxogo, str, &pila_origen, &pila_destino, &num_cartas);
     switch (res)
     {
      case 0:
       break;
      case 1: valido=movimiento_valido(pxogo, pila_origen, pila_destino, num_cartas);
              if (valido==1)
              {
              mover_carta(pxogo, pila_origen, pila_destino, num_cartas);
              estado=estado_juego(pxogo);
              }
       break;
      case 2:
       break;
     }
  }
  while(res!=2&&estado!=1);	
  //mostrarCartas(*pxogo);
}  

int leer_mov(t_juego *pxogo, char str[], int *nro_pila_origen, int *nro_pila_destino, int *num_cartas)
{   
  char * pEnd;
  int i;         
  if(!(str[0] >= '0' && str[0] <= '9')) 
  {
    if((strlen(str)==1) &&((str[0]=='m') || (str[0]=='M')))
    {
      return 2;
    }
    else
    {
      pxogo->aviso=strdup("ERROR: CARACTER INCORRECTO (introducir m o M)");
      return 0;
    }
  }
  else
  {
    for (i=0;i<strlen(str);i++)
      { 
        if(!(str[i] >= '0' && str[i] <= '9') && !(str[i]==' '))
        {
          pxogo->aviso=strdup("ERROR: ENTRADA INCORRECTA\n"); 
          return 0; 
        }
        
      }
    *nro_pila_origen = strtol (str,&pEnd,10);
    *nro_pila_destino = strtol (pEnd,&pEnd,10);
    if((pEnd[0]=='0'))
        {
          pxogo->aviso=strdup("ERROR: ENTRADA INCORRECTA 2\n"); 
          return 0; 
        }      
    *num_cartas = strtol (pEnd,NULL,10);
    if(0==*num_cartas)
    *num_cartas=1;      
        
    if((strlen(str)==1)||(*nro_pila_origen<0 || *nro_pila_origen>12) || (*nro_pila_destino<0 || *nro_pila_destino >12) || (*num_cartas<1 || *num_cartas>12)) 
    {
      pxogo->aviso=strdup("ERROR: ENTRADA INCORRECTA\n");
      return 0;
    }      
  }
  return 1;
}

void clear_buffer(void)
{    
  while ( getchar() != '\n' );
}

int estado_juego(t_juego *pxogo)
{
  if(pxogo->contNumVueltas==3)
  {
    pxogo->aviso=strdup("AVISO: JUEGO CARTAS SOLITARIO SIN COMPLETAR");
    pxogo->text=strdup(" ");
    pxogo->text2=strdup(" ");
    return 1;
  }
  if(pila_llena(&pxogo->salida8)&&pila_llena(&pxogo->salida9)&&pila_llena(&pxogo->salida10)&&pila_llena(&pxogo->salida11)&&(pxogo->contNumVueltas==2))
  {   
    pxogo->aviso=strdup("AVISO: JUEGO CARTAS SOLITARIO COMPLETO\n\tPUNTUACION: 50 puntos sobre 100.");
    pxogo->text=strdup(" ");
    pxogo->text2=strdup(" ");
    return 1;
  }
  if(pila_llena(&pxogo->salida8)&&pila_llena(&pxogo->salida9)&&pila_llena(&pxogo->salida10)&&pila_llena(&pxogo->salida11)&&(pxogo->contNumVueltas==1))
  {
    pxogo->aviso=strdup("AVISO: JUEGO CARTAS SOLITARIO COMPLETO\n\tPUNTUACION: 100 puntos, maxima puntuacion.");
    pxogo->text=strdup(" ");
    pxogo->text2=strdup(" ");
    return 1;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
int movimiento_valido(t_juego *pxogo, int pila_origen, int pila_destino, int num_cartas)
{
  int valido;
 if(pila_origen==12&&(pila_destino>=1&&pila_destino<=12))
       valido=mov_descarte_columnas_valido(pxogo, pila_origen, pila_destino);
   else if(pila_origen==0&&pila_destino==12)
         valido=mov_mazo_descarte_valido(pxogo, num_cartas, pila_origen);
      else if(pila_origen==12&&pila_destino==0)
          valido=mov_descarte_mazo_valido(pxogo, pila_destino);
        else if((pila_destino>=1&&pila_destino<=11)&&(pila_destino>=1&&pila_destino<=11))
          valido=mov_columnas_columnas_valido(pxogo, pila_origen, pila_destino, num_cartas);
            else 
            {
               pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
               valido=0;
            }
            
  return valido;
}

//******************************************************************************************************

int mov_descarte_columnas_valido(t_juego *pxogo, int nro_pila_origen, int nro_pila_destino)
{


  t_pila *pila_origen;
  t_pila *pila_destino;
  
  pila_origen=num_pila(pxogo,nro_pila_origen);   // apuntamos a la pila origen
  pila_destino=num_pila(pxogo,nro_pila_destino); // apuntamos a la pila destino                     // pila->set[pila->numCartas-1]

  // de descarte a columas entre 1 y 7
  if(nro_pila_destino>0 && nro_pila_destino<8){
    if(pila_vacia(pila_origen)==1){pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}
    else{if(pila_vacia(pila_destino)==1){
	if(((pila_origen->set[pila_origen->numCartas-1]).valor)==11){return 1; //llamada a movimiento
	}
	else{pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}}
      else{if(((pila_destino->set[pila_destino->numCartas-1]).palo != (pila_origen->set[pila_origen->numCartas-1]).palo) && 
	      (((pila_destino->set[pila_destino->numCartas-1]).valor) == (pila_origen->set[pila_origen->numCartas-1]).valor+1)){ return 1;
//llamada a movimiento
	}
	else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
	  return 0;
	}}}}
 
  // de descarte a columnas entre 8 y 11
  else if(nro_pila_destino>=8 && nro_pila_destino<12){
    if(pila_vacia(pila_origen)==1){pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}
    else{if(pila_vacia(pila_destino)==1){
	if(((pila_origen->set[pila_origen->numCartas-1]).valor)==0){return 1; //llamada a movimiento
	}
	else{pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}}
      else{if(((pila_destino->set[pila_destino->numCartas-1]).palo == (pila_origen->set[pila_origen->numCartas-1]).palo) && 
	      ((pila_destino->set[pila_destino->numCartas-1]).valor+1 == (pila_origen->set[pila_origen->numCartas-1]).valor)){ return 1;//llamada a movimiento
	}
	else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
	  return 0;
	}}}}

  else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
    return 0;}}




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int mov_mazo_descarte_valido(t_juego *pxogo,int num_cartas,int nro_pila_origen)
{
  t_pila *pila_origen;
  pila_origen=num_pila(pxogo,nro_pila_origen);   // apuntamos a la pila origen
  if((num_cartas !=1)||(pila_vacia(pila_origen)==1)) 
    {
      pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
      return 0;
    }
  else {return 1;//llamada a movimiento
  }
}

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int mov_columnas_columnas_valido(t_juego *pxogo, int nro_pila_origen, int nro_pila_destino, int num_cartas)
{
  
  t_pila *pila_origen;
  t_pila *pila_destino;
  
  pila_origen=num_pila(pxogo,nro_pila_origen);   // apuntamos a la pila origen
  pila_destino=num_pila(pxogo,nro_pila_destino); // apuntamos a la pila destino                     // pila->set[pila->numCartas-1]

  // columas entre 1 y 7
  if(nro_pila_destino>0 && nro_pila_destino<8){
    if(pila_vacia(pila_origen)==1){pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}
    else{if(pila_vacia(pila_destino)==1){
	if(((pila_origen->set[pila_origen->numCartas-num_cartas]).valor)==11){return 1; //llamada a movimiento
	}
	else{pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}}
      else{if(((pila_destino->set[pila_destino->numCartas-1]).palo != (pila_origen->set[pila_origen->numCartas-num_cartas]).palo) && 
	      (((pila_destino->set[pila_destino->numCartas-1]).valor) == ((pila_origen->set[pila_origen->numCartas-num_cartas]).valor+1)
	       &&  ((pila_origen->set[pila_origen->numCartas-num_cartas]).estado==1))){ return 1;
//llamada a movimiento
	}
	else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
	  return 0;
	}}}}
 
  //  columnas entre 8 y 11
  else if(nro_pila_destino>=8 && nro_pila_destino<12){
    if(pila_vacia(pila_origen)==1){pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}
    else{if(pila_vacia(pila_destino)==1){
	if(((pila_origen->set[pila_origen->numCartas-1]).valor)==0){return 1; //llamada a movimiento
	}
	else{pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO"); return 0;}}
      else{if(((pila_destino->set[pila_destino->numCartas-1]).palo == (pila_origen->set[pila_origen->numCartas-1]).palo) && 
	      ((pila_destino->set[pila_destino->numCartas-1]).valor+1 == (pila_origen->set[pila_origen->numCartas-1]).valor)){ return 1;
//llamada a movimiento
	}
	else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
	  return 0;
	}}}}

  else{ pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
    return 0;}}
  


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int mov_descarte_mazo_valido(t_juego *pxogo, int nro_pila_destino)
{
  t_pila *pila_destino; 
  pila_destino=num_pila(pxogo,nro_pila_destino); // apuntamos a la pila destino

  if(pila_vacia(pila_destino)==1)
  { 
    return 1; //llamada movimiento
  }
 else
 {
    pxogo->aviso=strdup("ERROR: MOVIMIENTO NO VALIDO");
    return 0;
  }
}


