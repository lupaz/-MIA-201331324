#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAL 80

typedef struct tokens {
    int token;
    char lexema[50];
    struct tokens *siguiente;
}tokens;

typedef struct{
    char palabra[10];
}reservadas;

//=======================================================================================Variables globales===========================================================================================
reservadas palabrasRes[17];
tokens *cabezaT;
tokens *colaT;
char linea[VAL]; //sera la encargada de contener toda la linea leida para luego analizarla por partes
tokens * tokActual;
tokens * preanalis;
int tokenActual=0;
int preanalisis=0;
//====== variables de parametros =======
int size=0;
char path[50];
char name[50];
char unidad='\0';

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

//===========================================================================================Metodo Principal===========================================================================================
int main(){
    cargarPalabrasReservadas();
    int salida=0;

    do{
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
        if(cabezaT->siguiente!=NULL){
            preanalisis=cabezaT->siguiente->token;
            preanalis=cabezaT->siguiente;
        }
        imprimirTokens();
        if(strcmp(linea,"exit")==0){
            salida=1;
        }

    }while(salida!=1);

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
                    addToken(2,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                }else{
                    lexema[aux]='\0'; //cerramos la cadena
                    addToken(2,lexema); //guardamos el token y su lexema para el analisis sintactico
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
                    addToken(2,lexema); //guardamos el token y su lexema para el analisis sintactico
                    aux=0;
                    estado=0;
                    puntero-=1;
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
//=============================================================================Metodos de analisis sintactico===========================================================================================
void avanzarToken(){
    tokActual=tokActual->siguiente;
    tokenActual=tokActual->token;
    preanalis=tokActual->siguiente;
    preanalisis=preanalis->token;
}


void origen(){ //inicio del analisis sintactico y validaciones
    if(tokActual!=NULL){
         switch (tokenActual) {

            case 9:{
                avanzarToken();
                COM1();
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
                printf("No se reconocio ningun comnado inicial.\n");
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
                PAR0();
            break;
            }
            case 7:{
                avanzarToken();
                PAR1();
            break;
            }
            default:{
                printf("Los atributos deben iniciar con \"+\" o \"-\".\n");
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
                avanzarToken();
                if(tokenActual==5){
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("se esperaba :: despues de size.\n");
                    break;
                }
                if(tokenActual==1){
                    //si todo va bien aca guardaria el tamano size en una global
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("se esperaba un numero positivo despues de ::.\n");
                    break;
                }

            break;
            }
            case 12:{
                avanzarToken();
                //if()
                PAR0();
            break;
            }
            default:{
                printf("El paramentro despues de \"-\" no es correcto.\n");
            break;
            }

        }
    }
}

void PAR1(){


}

