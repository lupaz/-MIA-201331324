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
reservadas palabrasRes[15];
tokens *cabezaT;
tokens *colaT;
char linea[VAL]; //sera la encargada de contener toda la linea leida para luego analizarla por partes
//===================================================================================Declaracion de Metodos===========================================================================================
void anlisisLexico( char *linea );

void addToken(int numtoken, char * lex);

void imprimirTokens();

void cargarPalabrasReservadas();

int leeConsola(char *cad, int n);

int compararCadenas(char * cad1,char *cad2);

int esNumero(char caracter);

int esLetra(char caracter);

//=========================================================================================Metpodo Principal===========================================================================================
int main(){
    cargarPalabrasReservadas();
    int salida=0;

    do{
        printf("Adminitrador:>> ");
        leeConsola(linea,VAL);
        analisisLexico(linea);
        imprimirTokens();
        if(strcmp(linea,"exit")==0){
            salida=1;
        }

    }while(salida!=1);

    return 0;


}



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
    int aux=0;
    char caracter='\0';
    char lexema[50];
    lexema[0]='\0';
    //limpiarCadena(lexema);
    int estado=0;
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
                    estado=3;
                }else if(caracter=='-'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=3;
                }else if((int)caracter==92){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=3;
                }else if(caracter=='"'){
                    lexema[aux]=caracter;
                    aux+=1;
                    estado=3;
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
                    addToken(2,lexema); //guardamos el token y su lexema para el analisis sintactico
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
                }if(caracter==' '){
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
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 24:{
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

