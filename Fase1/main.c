#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define VAL 80

typedef struct tokens {
    int token;
    char lexema[50];
    struct tokens *siguiente;
}tokens;

typedef struct{
    char palabra[10];
}reservadas;

typedef struct rutas{
    char nombre[25];
    struct rutas *siguiente;
}rutas;



//=======================================================================================Variables globales===========================================================================================
reservadas palabrasRes[17];
tokens *cabezaT;
tokens *colaT;
rutas *cabezaR;
rutas *colaR;
char linea[VAL]; //sera la encargada de contener toda la linea leida para luego analizarla por partes
tokens * tokActual;
tokens * preanalis;
int tokenActual=0;
int preanalisis=0;
//=============== variables de parametros =================
int size=0;
char path[50];
char name[25];
char unidad[2];
int error=0;
//================variables banderas ======================
int Bsize=0;
int Bname=0;
int Buni=0;
int Bpath=0;
int tamano=0;

//===================================================================================Declaracion de Metodos===========================================================================================
void anlisisLexico( char *linea );

void addToken(int numtoken, char * lex);

void imprimirTokens();

void cargarPalabrasReservadas();

int leeConsola(char *cad, int n);

int compararCadenas(char * cad1,char *cad2);

int esNumero(char caracter);

int esLetra(char caracter);

int segundaLinea(char linea[]);

int esReservada(char lexema[]);

void QuitarComillas();

void crearCarpeta(char ruta[]);

void agregarRuta(char ruta[]);

void splitPath(char cadena[]);

void imprimirRutas();

void crearDisco();

int comprobarExtencion(char nombre[]);

//=========== metodos sintacticos =============
void avanzarToken();
void origen();
void COM1();
void PAR();
void PAR0();
void PAR1();
void realizarAcciones();
//===========================================================================================Metodo Principal===========================================================================================
int main(){
    cargarPalabrasReservadas();
    int salida=0;

    /*do{
        printf("Adminitrador:>> ");
        leeConsola(linea,VAL);
        analisisLexico(linea);
        if(segundaLinea(linea)==1){
            printf("Second line:>> ");
            leeConsola(linea,VAL);
            analisisLexico(linea);
        }
        if(cabezaT != NULL){
            tokenActual=cabezaT->token;
            tokActual=cabezaT;
        }
        imprimirTokens();
        realizarAcciones();
        if(strcmp(linea,"exit")==0){
            salida=1;
        }

    }while(salida!=1);//*/

    return 0;

}

//=========================================================================================Metodo secundarios===========================================================================================

void cargarPalabrasReservadas(){
    strcpy(palabrasRes[0].palabra,"mkdisk");
    strcpy(palabrasRes[1].palabra,"size");
    strcpy(palabrasRes[2].palabra,"unit");
    strcpy(palabrasRes[3].palabra,"path");
    strcpy(palabrasRes[4].palabra,"name");
    strcpy(palabrasRes[5].palabra,"rmdisk");
    strcpy(palabrasRes[6].palabra,"fdisk");
    strcpy(palabrasRes[7].palabra,"type");
    strcpy(palabrasRes[8].palabra,"fit");
    strcpy(palabrasRes[9].palabra,"delete");
    strcpy(palabrasRes[10].palabra,"name");
    strcpy(palabrasRes[11].palabra,"add");
    strcpy(palabrasRes[12].palabra,"mount");
    strcpy(palabrasRes[13].palabra,"id");
    strcpy(palabrasRes[14].palabra,"umount");
    strcpy(palabrasRes[15].palabra,"fast");
    strcpy(palabrasRes[16].palabra,"full");
}

void addToken(int numtoken,char *lex){

    if(cabezaT==NULL){
        cabezaT = malloc(sizeof(tokens));
        cabezaT->token=numtoken;
        strcpy(cabezaT->lexema,lex);
        colaT=cabezaT;
    }else{
        colaT->siguiente=malloc(sizeof(tokens));
        colaT->siguiente->token=numtoken;
        strcpy(colaT->siguiente->lexema,lex);
        colaT=colaT->siguiente;
    }
}

int leeConsola(char *cad, int n) {
    int i, c;

    c=getchar();
    if (c == EOF) {
        cad[0] = '\0';
        return 0;
    }

    if (c == '\n')
        i = 0;
    else {
        cad[0] = c;
        i = 1;
    }

    for (; i < n-1 && (c=getchar())!=EOF && c!='\n'; i++)
       cad[i] = c;

    cad[i] = '\0';

    if (c != '\n' && c != EOF)
        while ((c = getchar()) != '\n' && c != EOF);

    return 1;
}

int compararCadenas(char * cad1,char *cad2){
    int resultado = strcasecmp(cad1,cad2);
    return resultado;
}

void analisisLexico(char linea[]){
    int i=0;
    char caracter='\0';
    char lexema[50];
    int estado=0;
    int aux=0;
    lexema[0]='\0';
    int puntero=0;
    int tamano=strlen(linea);
    while (puntero<strlen(linea)+1){

        caracter=linea[puntero];
        switch(estado){
            case 0:
            {
                if(esLetra(caracter)==1){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=2;
                }else if(esNumero(caracter)==1){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=1;
                }else if(caracter=='#'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=3;
                }else if(caracter==':'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=4;
                }else if(caracter=='+'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=6;
                }else if(caracter=='-'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=7;
                }else if((int)caracter==92){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=8;
                }else if(caracter=='"'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=9;
                }else if(caracter!='    ' && caracter!=' ' && caracter!='\0'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=24;
                }
                break;
            }
            case 1:{

                if(esNumero(caracter)==1){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=1;
                }else if(caracter==' '){
                    lexema[aux]='\0';
                    addToken(1,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                }else if(caracter=='\0'){
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(1,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(1,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
                break;
            }
            case 2:{
                 if(esLetra(caracter)==1){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=2;
                }else if(esNumero(caracter)==1){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=2;
                }else if(caracter==' '){
                    lexema[aux]='\0';
                    if(esReservada(lexema)==1){
                        aux=0;
                        estado=0;
                    }else{
                        addToken(2,lexema); //guardamos el token y su lexema para el analisis sintactico
                        aux=0;
                        estado=0;
                    }
                }else if(caracter=='\0'){
                    lexema[aux]='\0'; //cerramos la cadena
                    if(esReservada(lexema)==1){
                        aux=0;
                        estado=0;
                    }else{
                        addToken(2,lexema);
                        aux=0;
                        estado=0;
                    }
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    if(esReservada(lexema)==1){
                        aux=0;
                        estado=0;
                        puntero-=1;
                    }else{
                        addToken(2,lexema);
                        aux=0;
                        estado=0;
                        puntero-=1;
                    }
                }
                break;
            }
            case 3:{
                if(caracter=='\0'){
                    lexema[aux]='\0';
                    addToken(3,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=3;
                }
                break;
            }

            case 4:{
                if(caracter==':'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=5;
                }else{
                    estado=24;
                    puntero-=1;
                }
            break;
            }
            case 5:{
                if(caracter=='\0'){
                    lexema[aux]='\0';
                    addToken(5,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(5,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
            break;
            }
            case 6:{
               if(caracter=='\0'){
                    lexema[aux]='\0';
                    addToken(6,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(6,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
            break;
            }
            case 7:{
                if(caracter=='\0'){
                    lexema[aux]='\0';
                    addToken(7,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(7,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
            break;
            }
            case 8:{if(caracter=='\0'){
                    lexema[aux]='\0';
                    //addToken(8,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    //addToken(8,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
            break;
            }
            case 9:{
                if(caracter=='"'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=10;
                }else if(caracter=='\0'){
                    printf("ERROR: Se esperaba cierre de cadena.\n");
                }else{
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=9;
                }
            break;
            }
            case 10:{
                if(caracter=='\0'){
                    lexema[aux]='\0';
                    addToken(4,lexema);
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(4,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
                }
            break;
            }
            case 24:{ //estado que notifica los errores o caracteres que no pertenecen al lenguaje
                lexema[aux]='\0';
                printf("ERROR: El caracter: \"%s\" no pertenece al lenguaje.\n",lexema);
                estado=0;
                puntero-=1;
                aux=0;
            break;
            }
            default:
                break;
        }
    puntero+=1;
    }

}

int esNumero(char caracter){
    int ascii=(int)caracter;
    if( ascii>47 && ascii<58){
        return 1;
    }else{
        return 0;
    }
}

int esLetra(char caracter){
    int ascii=(int)caracter;

    if( (ascii>64 & ascii<91 )|| (ascii>96 && ascii<123)){
        return 1; //es un evento verdadero;
    }else{
        return 0; //es un evento falso;
    }

}

void imprimirTokens(){
    if(cabezaT==NULL){
        printf("Tabla de tokens vacia.\n");
    }else{
        tokens *tmp=cabezaT;
        printf("%8s \t%2s","Token","Lexema\n");
        while(tmp!=NULL){
        printf("%7i \t\%2s\n",tmp->token,tmp->lexema);
        tmp=tmp->siguiente;
        }
    }

}

int esReservada(char lexema[]){
    int i;
    for(i=0; i<17;i++){//como sabemos cuantas reservadas tenemos
        if(compararCadenas(palabrasRes[i].palabra,lexema)==0){
            switch (i){
                case 0:{
                    addToken(9,lexema);
                    return 1;
                }

                case 1:{
                    addToken(10,lexema);
                    return 1;
                }
                case 2:{
                    addToken(11,lexema);
                    return 1;
                }
                case 3:{
                    addToken(12,lexema);
                    return 1;
                }
                case 4:{
                    addToken(13,lexema);
                    return 1;
                }
                case 5:{
                    addToken(14,lexema);
                    return 1;
                }
                case 6:{
                    addToken(15,lexema);
                    return 1;
                }
                case 7:{
                    addToken(16,lexema);
                    return 1;
                }
                case 8:{
                    addToken(17,lexema);
                    return 1;
                }
                case 9:{
                    addToken(18,lexema);
                    return 1;
                }
                case 10:{
                    addToken(19,lexema);
                    return 1;
                }
                case 11:{
                    addToken(20,lexema);
                    return 1;
                }
                case 12:{
                    addToken(21,lexema);
                    return 1;
                }
                case 13:{
                    addToken(22,lexema);
                    return 1;
                }
                case 14:{
                    addToken(23,lexema);
                    return 1;
                }
                case 15:{
                    addToken(24,lexema);
                    return 1;
                }
                case 16:{
                    addToken(25,lexema);
                    return 1;
                }
            }

        }
    }
    return 0;
}


int segundaLinea(char linea[]){
    int i;
    for(i=0;i<strlen(linea);i++){
        if((int)linea[i]==92){
            return 1;
        }
    }
    return 0;
}

//=========================carpetas===========================

void QuitarComillas(char cad[]){
    int i;
    int j=0;
    if(cad[0]=='"' && cad[strlen(cad)-1]=='"'){
        for(i=0;i<strlen(cad);i++){
            if(cad[i]!='"'){
                cad[j]=cad[i];
                j++;
            }
            if(i==(strlen(cad)-2)){
                cad[i]='\0';
            }
        }
    }
}

void crearCarpeta(char ruta[]){ //recibe la rutas ya sin comillas
    char comando[50]="mkdir ";
    strcat(comando,ruta);
    system(comando);
}

void agregarRuta(char ruta[]){
    if(cabezaR==NULL){
        cabezaR = malloc(sizeof(rutas));
        strcpy(cabezaR->nombre,ruta);
        cabezaR->siguiente=NULL;
        colaR=cabezaR;
    }else{
        colaR->siguiente=malloc(sizeof(rutas));
        strcpy(colaR->siguiente->nombre,ruta);
        colaR=colaR->siguiente;
    }
}

void splitPath(char cadena[]){
    char lexema[25];
    lexema[0]='\0';
    char caracter;
    int aux=0;
    int i=0;
    for(;i<strlen(cadena);i++){
        caracter=cadena[i];
        if(caracter=='/'){
            if(lexema[0]!='\0'){
                lexema[aux]='\0';
                aux=0;
                agregarRuta(lexema);
            }
        }else{
            if(caracter==' '){
                caracter='_';
            }
            lexema[aux]=caracter;
            aux++;
            if(i==(strlen(cadena)-1)){
                lexema[aux]='\0';
                aux=0;
                agregarRuta(lexema);
            }
        }
    }
}

void imprimirRutas(){
    if(cabezaR==NULL){
        printf("Tabla de rutas vacia.\n");
    }else{
        rutas *tmp=cabezaR;
        printf("%8s","Carpetas\n");
        while(tmp!=NULL){
        printf("%7s\n",tmp->nombre);
        tmp=tmp->siguiente;
        }
    }
}

void crearDirectorios(char path[]){
    QuitarComillas(path);
    splitPath(path);
    char general[50]="/";

    if(cabezaR!=NULL){
        rutas *tmp;
        tmp=cabezaR;
        while(tmp!=NULL){
            strcat(general,tmp->nombre);
            if(existeCarpeta(general)==2){
                crearCarpeta(general);
            }
            strcat(general,"/");
        tmp=tmp->siguiente;
        }
    }
}

int existeCarpeta(char ruta[]){
    struct stat estado;
    if(stat(ruta,&estado)==-1){
        return 2;
    }else{
        return 1;
    }
}

//========================Archivos dsk==========================

void crearDisco(){
    if(comprobarExtencion(name)==1){
        crearDirectorios(path);
        char rutaFinal[75];
        char espacio='\0';
        QuitarComillas(path);
        QuitarComillas(name);
        if(path[(strlen(path)-1)]!='/'){
            strcpy(rutaFinal,path);
            strcat(rutaFinal,"/");
            strcat(rutaFinal,name);
        }else{
            strcpy(rutaFinal,path);
            strcat(rutaFinal,name);
        }

        FILE *nuevo;
        nuevo=fopen(rutaFinal,"wb");
        //printf("dimen: %i \n",tamano);
        if(nuevo!=NULL){
            fseek(nuevo,tamano,SEEK_SET);
            fwrite(&espacio,sizeof(espacio),1,nuevo);
            printf("AVISO: Disco \"%s\" creado Correctamente.\n",name);
        }else{
            printf("AVISO: No se pudo crear el disco.(ER=IO)\n");
        }

    }else{
        printf("ERROR: La extension del disco no es valida.\n");
        printf("AVISO: No se pudo crear el disco.\n");
    }

}

int comprobarExtencion(char nombre[]){
    char *val;
    char extension[]=".dsk";
    val=strstr(nombre,extension);

    if(val==NULL){
        return 0;
    }else{
        return 1;
    }

}


//=============================================================================Metodos de analisis sintactico===========================================================================================

void avanzarToken(){
    if(tokActual!=NULL){
        if(tokActual->siguiente!=NULL){
            tokActual=tokActual->siguiente;
            tokenActual=tokActual->token;
        }else{
            tokActual=NULL;
            tokenActual=0;
        }
    }
    //preanalis=tokActual->siguiente;
    //preanalisis=preanalis->token;
}


void origen(){ //inicio del analisis sintactico y validaciones
    if(tokActual!=NULL){
         switch (tokenActual) {
            case 9:{
                avanzarToken();
                COM1();
                if(error==0){ // aca pienso hacer las validaciones y la creacion de los archs
                    if(Buni==1){
                            if( (strcasecmp(unidad,"M")==0) || (strcasecmp(unidad,"K")==0)){
                                if (strcasecmp(unidad,"M")==0){
                                    tamano=1024*1024*size;
                                }else{
                                    tamano=1024*size;
                                }
                            }else{
                                 tamano=1024*1024*size;
                                 printf("ERROR: El parametro de unit no es valido.\n");
                            }
                    }else{
                            tamano=1024*1024*size;
                    }

                    if(Bpath==0 || Bsize==0 || Bname==0){
                        if(Bpath==0){
                            printf("ERROR: Falta el parametro Path.\n");
                        }else if( Bname==0){
                            printf("ERROR: Falta el parametro Name.\n");
                        }else{
                            printf("ERROR: Falta el parametro Size.\n");
                        }
                        printf("AVISO: No se pudo crear el disco.\n");
                    }else{
                        crearDisco();
                    }
                }else{
                   printf("AVISO: No se pudo crear el disco.\n");
                }
            break;
            }
            case 14:{

            break;
            }
            case 15:{

            break;
            }
            case 21:{

            break;
            }
            case 23:{

            break;
            }
            default:{
                printf("No se reconocio ningun comando inicial valido.\n");
            break;
            }
        }
    }
}

void COM1(){
    if(tokActual!=NULL){
        PAR();
    }
}

void PAR(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 6:{
                avanzarToken();
                PAR1();
            break;
            }
            case 7:{
                avanzarToken();
                PAR0();
            break;
            }
            default:{
                printf("Los parametros deben iniciar con \"+\" o \"-\".\n");
                error=1;
                //podria hacer un ciclo que me llegue a un punto de recuperacion de errores pero por el momenot vamos a asumir que no se ejecutara nada si no esta sintacticamente bien;
            break;
            }

        }
    }
}

void PAR0(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 10:{
                if(Bsize<1){
                    Bsize=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de size.\n");
                        break;
                    }
                    if(tokenActual==1){ //token = numero
                        //si todo va bien aca guardaria el tamano size en una global
                        size=atoi(tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba un numero positivo despues de ::.\n");
                        break;
                    }
                    PAR();
                }else{
                   printf("El parametro size no se puede repetir.\n");
                   error=1;
                }
            break;
            }
            case 12:{
                if(Bpath<1){
                    Bpath=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de path.\n");
                        break;
                    }
                    if(tokenActual==4){
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(path,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba un numero positivo despues de \"::\".\n");
                        break;
                    }
                    PAR();
                }else{// aca notifico error cuando path viene mas de una vez
                    printf("El parametro phat no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            case 13:{
                if(Bname<1){
                    Bname=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de path.\n");
                        break;
                    }
                    if(tokenActual==4){
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(name,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("Se esperaba una cadena despues de \"::\".\n");
                        break;
                    }
                    PAR();
                }else{
                    printf("El parametro name no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            default:{
                printf("El paramentro despues de \"-\" no es correcto.\n");
                error=1;
            break;
            }

        }
    }
}

void PAR1(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 11:{
                if(Buni<1){
                    Buni=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de unit.\n");
                        break;
                    }
                    if(tokenActual==2){ //token = numero
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(unidad,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba un numero positivo despues de ::.\n");
                        break;
                    }
                    PAR();
                }else{
                   printf("El parametro unit no se puede repetir.\n");
                   error=1;
                }
            break;
            }
            default:{
                printf("El paramentro despues de \"+\" no es correcto.\n");
                error=1;
            break;
            }

        }
    }

}

void realizarAcciones(){
    origen();
    //metodo que borra la tabla de tokens para analizar la siguiente linea.
}


