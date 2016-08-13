#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define VAL 150
#define VAL2 250
//================estrcuturas basicas=========================
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
//=================Estrcuturas de montaje======================
typedef struct letter{
    int id;
    char letra[2];
    char nombreDisco[20];
    struct letter *siguiente;
    struct number *abajo;
}letter;

typedef struct number{
    int id;
    char ruta[25];
    char nombreParticion[20];
    char idParticion[5];
    struct number *abajo;
}number;

//================Estructuras de orden =========================
typedef struct iniciales{
    int numero;
    struct iniciales *siguiente;
}iniciales;

typedef struct finales{
    int numero;
    struct finales *siguiente;
}finales;

typedef struct rang{
    int inicio;
    int fin;
    struct rang *siguiente;
}rang;
//================estrcuturas de Discos=========================

typedef struct partition{
    char part_status;
    char part_type;
    char part_fit[3];
    char part_name[16];
    int  part_start;
    int  part_size;
}partition;

typedef struct MBR {
   int  mbr_tamano;
   char fecha_Hora[25];
   int  mbr_disk_signature;
   partition mbr_partition_1;
   partition mbr_partition_2;
   partition mbr_partition_3;
   partition mbr_partition_4;

}MBR;

typedef struct EBR{
    char part_status;
    char part_fit[3];
    int  part_start;
    int  part_size;
    int  part_next;
    char part_name[16];
}EBR;


//=======================================================================================Variables globales===========================================================================================
reservadas palabrasRes[17];
tokens *cabezaT;
tokens *colaT;
rutas *cabezaR;
rutas *colaR;
iniciales *cabezaI;
iniciales *colaI;
finales *cabezaF;
finales *colaF;
rang*cabRango;
rang *colRango;
letter *cabezaL;
letter *colaL;
letter *tmpL;
char letras[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

char linea[VAL]; //sera la encargada de contener toda la linea leida para luego analizarla por partes
char linea2[VAL2];
tokens * tokActual;
tokens * preanalis;
int tokenActual=0;
int preanalisis=0;
//=============== variables de parametros =================
int size=0;
char path[50];
char name[25];
char unidad[10];
char fit[10];
char type[10];
char delVal[10];
char idPar[10];
int error=0;
int add=0;
//================variables banderas ======================
int Bsize=0;
int Bname=0;
int Buni=0;
int Bpath=0;
int Bfit=0;
int Bdelete=0;
int Btype=0;
int Badd=0;
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

void splitPath2(char cadena[]);

void imprimirRutas();

void crearDisco();

int comprobarExtencion(char nombre[]);

void limpiarTokens();

void limpiarRutas();

void reiniciarBanderas();

void  borrarDisco(char ruta[]);

void fecha_Hora( char datos[]);

void leerArchivos();

int numeroAleatorio();

void escribirMBR(char direc[],int tamanio);

void ValidarOperacion();

int fdisk1();

int fdisk2();

int fdisk3();

void crearPrimaria(char ruta[], int tamanio, char fit1[], char nombre[], char tipo);

void crearExtendida(char ruta[], int tamanio, char fit1[], char nombre[], char tipo);

void crearLogica(char ruta[], int tamanio, char fit1[], char nombre[]);

void addInicial(int numer);

void impIniciales();

void addFinal(int numer);

void addRango(int inicio, int fin);

void limpiarIniciales();

void limpiarFinales();

void limpiarRangos();

void actualizarRangos(int tamaDisco);

void inicialesFinales(char ruta[]);

void inicialesFinalesLog(char ruta[], int inicioE, int tamaE);

void imprimeRangos();

int VerificarNombre(char ruta[],char nombre[]);

int VerificarNombre2(char ruta[],char nombre[]);

void eliminarParticiones(char ruta[], char nombre[],char tipo[]);

void eliminarLogica(char ruta[], char nombre[], char tipo[]);

void asignarLetra(char cadena[],char letra);

void obtenerNombreDisco( char ruta[],char nombre[]);

letter * DiscoMontado(char nombre[]);

void addDisco(char nombre[]);

int particionMontada(char nombre[], letter *disco);

void addParticion(char nombre[], letter *disco,char ruta[]);

void imprimirMontaje();

int montarLogicas(char ruta[],char nombre [], char nombreDisco[]);

letter * montajeDisponible();

void leeArchivo(char ruta[]);

int leefichero(char *cadena,int n,FILE *fichero);

void consola();

void archivo();

//=========== metodos sintacticos =============
void avanzarToken();
void origen();
void COM1();
void COM2();
void COM3();
void PAR();
void PAR0();
void PAR1();
void PAR2();
void PAR3();
void PAR4();
void PAR5();
void PAR6();
void realizarAcciones();

//===========================================================================================Metodo Principal===========================================================================================
int main(){
//---------Aleatorio-----------------
	int raiz=time(NULL);        //---
	srand(raiz);                //---
//---------Reservadas----------------
    cargarPalabrasReservadas(); //---
    int salida=0;
    char intro;                 //---
//-----------------------------------

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
                    realizarAcciones();
                    //imprimirTokens();
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
    strcpy(palabrasRes[15].palabra,"exec"); //24
    strcpy(palabrasRes[16].palabra,"rep");//25

}

void addToken(int numtoken,char *lex){

    if(cabezaT==NULL){
        cabezaT = malloc(sizeof(tokens));
        cabezaT->token=numtoken;
        cabezaT->siguiente=NULL;
        strcpy(cabezaT->lexema,lex);
        colaT=cabezaT;
        colaT->siguiente=NULL;
    }else{
        colaT->siguiente=malloc(sizeof(tokens));
        colaT->siguiente->token=numtoken;
        strcpy(colaT->siguiente->lexema,lex);
        colaT=colaT->siguiente;
        colaT->siguiente=NULL;
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

int leefichero(char *cad,int n,FILE *fichero){
//fgetc(fichero).
    int i, c;

    c=fgetc(fichero);

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

    for (; i < n-1 && (c=fgetc(fichero))!=EOF && c!='\n'; i++)
       cad[i] = c;

    cad[i] = '\0';

    if (c != '\n' && c != EOF)
        while ((c = fgetc(fichero)) != '\n' && c != EOF);

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

    if( (ascii>64 && ascii<91 )|| (ascii>96 && ascii<123)){
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

void fecha_Hora( char cadena[]){
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        strftime(cadena,128,"%d/%m/%y %H:%M:%S",tlocal);
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
    char comando[150]=" mkdir ";
    strcat(comando,ruta);
    //strcat(comando," && sudo chmod 777 ");
    //strcat(comando,ruta);
    system(comando);
}

void agregarRuta(char ruta[]){
    if(cabezaR==NULL){
        cabezaR = malloc(sizeof(rutas));
        strcpy(cabezaR->nombre,ruta);
        cabezaR->siguiente=NULL;
        colaR=cabezaR;
        colaR->siguiente=NULL;
    }else{
        colaR->siguiente=malloc(sizeof(rutas));
        strcpy(colaR->siguiente->nombre,ruta);
        colaR=colaR->siguiente;
        colaR->siguiente=NULL;
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

void splitPath2(char cadena[]){
    QuitarComillas(cadena);
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
        return 2; //no existe
    }else{
        return 1; //si existe
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

        FILE *archivo;
        archivo=fopen(rutaFinal,"wb");
        //printf("dimen: %i \n",tamano);
        if(archivo!=NULL){
            fseek(archivo,tamano,SEEK_SET);
            fwrite(&espacio,sizeof(espacio),1,archivo);
            fclose(archivo); //cierro el archivo con el espacio total;
            escribirMBR(rutaFinal,tamano); //aca le asigno su  MBR de disco;
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

void escribirMBR(char direc[],int tamanio){

    FILE *arch;
    MBR escritura;
    arch=fopen(direc,"r+b");
    char datos[25];
    fecha_Hora(datos); // recuperamos la fecha y hora de creacion
    escritura.mbr_tamano=tamanio;
    strcpy(escritura.fecha_Hora,datos);
    escritura.mbr_disk_signature= numeroAleatorio();
    escritura.mbr_partition_1.part_status='F';
    escritura.mbr_partition_2.part_status='F';
    escritura.mbr_partition_3.part_status='F';
    escritura.mbr_partition_4.part_status='F';

    if(arch!=NULL){
        fseek(arch,0,SEEK_SET);
        //printf("Apertura correcta.\n");
        fwrite(&escritura,sizeof(MBR),1,arch);
        fclose(arch);
        //printf("Escritura correcta.\n");
    }
}

int numeroAleatorio(){
    int numero;
	numero=rand()%(10000-1+1)+1;//rango=(1,10000);
	return numero;
}

//=====================metodos de borrado======================

void limpiarTokens(){
    if(cabezaT!=NULL){
        tokens *tmp=cabezaT;
        while(tmp!=NULL){
            free(tmp);
        tmp=tmp->siguiente;
        }
        cabezaT=NULL;
        colaT=NULL;
    }
}

void limpiarRutas(){
    if(cabezaR!=NULL){
        rutas *tmp=cabezaR;
        while(tmp!=NULL){
            free(tmp);
        tmp=tmp->siguiente;
        }
        cabezaR=NULL;
        colaR=NULL;
    }
}

void reiniciarBanderas(){
    size=0;
    error=0;
    add=0;
    tamano=0;
//================variables banderas ======================
    Bsize=0;
    Bname=0;
    Buni=0;
    Bpath=0;
    Bfit=0;
    Bdelete=0;
    Btype=0;
    Badd=0;
}

void  borrarDisco(char ruta[]){
    QuitarComillas(ruta);
    if(existeCarpeta(ruta)==1){
        printf("Administrador:>> Esta seguro que desea borrar el disco? [Y|N].\n");
        printf("Administrador:>> ");
        leeConsola(linea,2);
        if(strcasecmp(linea,"Y")==0){
            char comando[150]="rm ";
            strcat(comando,ruta);
            system(comando);
            printf("AVISO: Disco borrado correctamente.\n");
        }else{
            printf("AVISO: No se borro el disco.\n");
        }

    }else{
        printf("ERROR: El disco no existe o el directorio es incorrecto.\n");
    }
}

//===================== metodos particiones=======================

void ValidarOperacion(){
    if(Bpath==1 && Bsize==1 && Bname==1){
        if(Badd==1){
            fdisk3();
        }else{
            fdisk1();
        }

    }else if (Bname==1 && Bdelete==1 && Bpath==1){
        fdisk2();
    }else{

        if(Bpath==0){
            printf("ERROR: Falta el parametro Path.\n");
        }else if( Bname==0){
            printf("ERROR: Falta el parametro Name.\n");
        }else{
            printf("ERROR: Falta el parametro Size.\n");
        }
        printf("AVISO: No se pudo crear la particion.\n");
    }
}

int fdisk1(){ //crea particiones
    char fitFinal[5];

    if(Buni==1){
        if((strcasecmp(unidad,"M")==0) || (strcasecmp(unidad,"K")==0)|| (strcasecmp(unidad,"B")==0)){
            if (strcasecmp(unidad,"M")==0){
                if(size<2){
                printf("ERROR: El tamano minimo de una particion es de 2 MB.\n");
                printf("AVISO: No se pudo crear la particion.\n");
                return -1;
                }else{
                    tamano=1024*1024*size;
                }
            }else if(strcasecmp(unidad,"K")==0){
                if(size<2048){
                    printf("ERROR: El tamano minimo de una paticion es de 2048 KB.\n");
                    printf("AVISO: No se pudo crear el disco.\n");
                     return -1;
                }else{
                    tamano=1024*size;
                }
            }else if(strcasecmp(unidad,"B")==0){
                if(size<2097152){
                    printf("ERROR: El tamano minimo de una particion es de 2097152 bytes.\n");
                    printf("AVISO: No se pudo crear el disco.\n");
                    return -1;
                }else{
                    tamano=size;
                }
            }
        }else{
            printf("ERROR: El parametro de unit no es valido.\n");
            printf("AVISO: No se pudo crear la particion.\n");
            return -1;
        }

    }else{
        if(size<2048){
            printf("ERROR: El tamano minimo de una particion es de 2048 KB.\n");
            printf("AVISO: No se pudo crear el disco.\n");
            return -1;
        }else{
            tamano=1024*size;
        }
    }

    if(Bfit==1){
        if((strcasecmp(fit,"BF")==0) || (strcasecmp(fit,"FF")==0)|| (strcasecmp(fit,"WF")==0)){
            if (strcasecmp(fit,"BF")==0){
                strcpy(fitFinal,"BF");
            }else if(strcasecmp(fit,"FF")==0){
                strcpy(fitFinal,"FF");
            }else{
                strcpy(fitFinal,"WF");
            }
        }else{
            printf("ERROR: El parametro de fit no es valido.\n");
            printf("AVISO: No se puede crear la particion.\n");
        }
    }else{
        strcpy(fitFinal,"wF");
    }

    if(Btype==1){
        if((strcasecmp(type,"P")==0) || (strcasecmp(type,"E")==0)|| (strcasecmp(type,"L")==0)){
            if (strcasecmp(type,"P")==0){
                if(VerificarNombre(path,name)==1){
                    printf("ERROR: Ya existe una particion con el nombre introducido.\n");
                    printf("AVISO: No se puede crear la particion Primaria.\n");
                }else{
                    crearPrimaria(path,tamano,fitFinal,name,'P');
                }
            }else if(strcasecmp(type,"E")==0){
                if(VerificarNombre(path,name)==1){
                    printf("ERROR: Ya existe una particion con el nombre introducido.\n");
                    printf("AVISO: No se puede crear la particion Extendida.\n");
                }else{
                    crearExtendida(path,tamano,fitFinal,name,'E');
                }
            }else{
                if(VerificarNombre2(path,name)==1){
                    printf("ERROR: Ya existe una particion con el nombre introducido.\n");
                    printf("AVISO: No se puede crear la particion Logica.\n");
                }else{
                    crearLogica(path,tamano,fitFinal,name);
                }

            }
        }else{
            printf("ERROR: El parametro de type no es valido.\n");
            printf("AVISO: No se puede crear la particion.\n");
        }
    }else{
       if(VerificarNombre(path,name)==1){
            printf("ERROR: Ya existe una particion con el nombre introducido.\n");
            printf("AVISO: No se puede crear la particion Primaria.\n");
        }else{
            crearPrimaria(path,tamano,fitFinal,name,'P');
        }
    }

}

int fdisk2(){ //elimina las particones
    if(strcasecmp(delVal,"FAST")){
        eliminarParticiones(path,name,delVal);
    }else if(strcasecmp(delVal,"FULL")){
        eliminarParticiones(path,name,delVal);
    }else{
        printf("ERROR: El parametro de delete no es valido.\n");
        printf("AVISO: No se pudo eliminar la particion.\n");
    }
}

int fdisk3(){ //quita o agrega espacio
    //pendienton add
}

void crearPrimaria(char ruta[], int tamanio, char fit1[], char nombre[], char tipo){

    QuitarComillas(ruta);
    QuitarComillas(nombre);
    int hayEspacio=0;
    int BcreoP=0;
    int tama=tamanio;
    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            inicialesFinales(ruta); //actualizamos los rangos;
            MBR entrada;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);
            //=========calculamos si hay espacio para la particion;
            int inicioPar;
            rang *tmp;
            tmp=cabRango;
            while(tmp!=NULL){
                 int disponible=tmp->fin-tmp->inicio;
                 if(disponible>=tamanio){
                    inicioPar=tmp->inicio;
                    hayEspacio=1;
                    break;
                 }
            tmp=tmp->siguiente;
            }
            //======================fin=============================

            if(hayEspacio==1){

                if(entrada.mbr_partition_1.part_status=='F' && BcreoP==0){
                    entrada.mbr_partition_1.part_status='V';
                    entrada.mbr_partition_1.part_type=tipo;
                    strcpy(entrada.mbr_partition_1.part_fit,fit1);
                    entrada.mbr_partition_1.part_start=inicioPar;
                    entrada.mbr_partition_1.part_size=tamanio;
                    strcpy(entrada.mbr_partition_1.part_name,nombre);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                    fclose(archivo);
                    printf("AVISO: Particion primaria creada correctamente.\n");
                    BcreoP=1;
                }

                if(entrada.mbr_partition_2.part_status=='F' && BcreoP==0){

                    entrada.mbr_partition_2.part_status='V';
                    entrada.mbr_partition_2.part_type=tipo;
                    strcpy(entrada.mbr_partition_2.part_fit,fit1);
                    entrada.mbr_partition_2.part_start=inicioPar;
                    entrada.mbr_partition_2.part_size=tamanio;
                    strcpy(entrada.mbr_partition_2.part_name,nombre);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                    fclose(archivo);
                    printf("AVISO: Particion primaria creada correctamente.\n");
                    BcreoP=1;
                }

                if(entrada.mbr_partition_3.part_status=='F' && BcreoP==0){
                    entrada.mbr_partition_3.part_status='V';
                    entrada.mbr_partition_3.part_type=tipo;
                    strcpy(entrada.mbr_partition_3.part_fit,fit1);
                    entrada.mbr_partition_3.part_start=(sizeof(MBR)+entrada.mbr_partition_1.part_size + entrada.mbr_partition_2.part_size+1);
                    entrada.mbr_partition_3.part_size=tamanio;
                    strcpy(entrada.mbr_partition_3.part_name,nombre);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                    fclose(archivo);
                    printf("AVISO: Particion primaria creada correctamente.\n");
                    BcreoP=1;
                }

                if(entrada.mbr_partition_4.part_status=='F' && BcreoP==0){
                    entrada.mbr_partition_4.part_status='V';
                    entrada.mbr_partition_4.part_type=tipo;
                    strcpy(entrada.mbr_partition_4.part_fit,fit1);
                    entrada.mbr_partition_4.part_start=(sizeof(MBR)+entrada.mbr_partition_1.part_size+entrada.mbr_partition_2.part_size+entrada.mbr_partition_3.part_size+1);
                    entrada.mbr_partition_4.part_size=tamanio;
                    strcpy(entrada.mbr_partition_4.part_name,nombre);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                    fclose(archivo);
                    printf("AVISO: Particion primaria creada correctamente.\n");
                    BcreoP=0;
                }

                if(BcreoP==0){
                    printf("AVISO: No se puede crear otra particion primaria.\n");
                }

            }else{
                printf("ERROR: No hay suficiente espacio para crea la particion.\n");
                printf("AVISO: No se pudo crear la particion primaria.\n");
            }

        }else{
            printf("ERROR: No se pudo acceder al disco.\n");
            printf("AVISO: No se pudo crear la particion primaria.\n");
        }

    }else{
        printf("ERROR: El Disco a particionar no existe.\n");
        printf("AVISO: No se pudo crear la particion primaria.\n");
    }

}

void crearExtendida(char ruta[], int tamanio, char fit1[], char nombre[], char tipo){
    QuitarComillas(ruta);
    QuitarComillas(nombre);
    int hayEspacio=0;
    int BcreoP=0;
    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            MBR entrada;
            EBR salida;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);
            //=========calculamos si hay espacio para la particion;
            inicialesFinales(ruta); //actualizamos los rangos;
            int inicioPar;
            rang *tmp;
            tmp=cabRango;
            while(tmp!=NULL){
                 int disponible=tmp->fin-tmp->inicio;
                 if(disponible>=tamanio){
                    inicioPar=tmp->inicio;
                    hayEspacio=1;
                    break;
                 }
            tmp=tmp->siguiente;
            }
            //======================fin=============================

            if (hayEspacio==1){
                if(entrada.mbr_partition_1.part_type=='E' || entrada.mbr_partition_2.part_type=='E' || entrada.mbr_partition_3.part_type=='E' || entrada.mbr_partition_4.part_type=='E' ){
                    printf("ERROR: Ya existe una particion extendida.\n");
                    printf("AVISO: No se pueden crear dos particiones Extendidas.\n");
                }else{

                    if(entrada.mbr_partition_1.part_status=='F' && BcreoP==0){
                        entrada.mbr_partition_1.part_status='V';
                        entrada.mbr_partition_1.part_type=tipo;
                        strcpy(entrada.mbr_partition_1.part_fit,fit1);
                        entrada.mbr_partition_1.part_start=inicioPar;
                        entrada.mbr_partition_1.part_size=tamanio;
                        strcpy(entrada.mbr_partition_1.part_name,nombre);
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                        //===================creando el primer EBR ==================================
                        salida.part_status='F';
                        salida.part_start=inicioPar;
                        salida.part_next=-1;
                        // aca escribo el primer ebr de la particion extendida.
                        fseek(archivo,inicioPar,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo);
                        fclose(archivo);
                        printf("AVISO: Particion Extendida creada correctamente.\n");
                        BcreoP=1;
                    }


                    if(entrada.mbr_partition_2.part_status=='F' && BcreoP==0){
                        entrada.mbr_partition_2.part_status='V';
                        entrada.mbr_partition_2.part_type=tipo;
                        strcpy(entrada.mbr_partition_2.part_fit,fit1);
                        entrada.mbr_partition_2.part_start=inicioPar;
                        entrada.mbr_partition_2.part_size=tamanio;
                        strcpy(entrada.mbr_partition_2.part_name,nombre);
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuebo el MBR
                        //===================creando el primer EBR ==================================
                        //fseek(archivo,0,SEEK_SET);
                        //fread(&entrada,sizeof(MBR),1,archivo);
                        salida.part_status='F';
                        salida.part_start=inicioPar;
                        salida.part_next=-1;
                        fseek(archivo,inicioPar,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo); // aca escribo el primer ebr de la particion extendida.
                        fclose(archivo);
                        printf("AVISO: Particion Extendida creada correctamente.\n");
                        BcreoP=1;
                    }

                    if(entrada.mbr_partition_3.part_status=='F' && BcreoP==0){
                        entrada.mbr_partition_3.part_status='V';
                        entrada.mbr_partition_3.part_type=tipo;
                        strcpy(entrada.mbr_partition_3.part_fit,fit1);
                        entrada.mbr_partition_3.part_start=inicioPar;
                        entrada.mbr_partition_3.part_size=tamanio;
                        strcpy(entrada.mbr_partition_3.part_name,nombre);
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuebo el MBR
                        //===================creando el primer EBR ==================================
                        salida.part_status='F';
                        salida.part_start=inicioPar;
                        salida.part_next=-1;
                        fseek(archivo,inicioPar,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo); // aca escribo el primer ebr de la particion extendida.
                        fclose(archivo);
                        printf("AVISO: Particion Extendida creada correctamente.\n");
                        BcreoP=1;
                    }

                    if(entrada.mbr_partition_4.part_status=='F' && BcreoP==0){
                        entrada.mbr_partition_4.part_status='V';
                        entrada.mbr_partition_4.part_type=tipo;
                        strcpy(entrada.mbr_partition_4.part_fit,fit1);
                        entrada.mbr_partition_4.part_start=inicioPar;
                        strcpy(entrada.mbr_partition_4.part_name,nombre);
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo nuevamente el MBR
                        //===================creando el primer EBR ==================================
                        salida.part_status='F';
                        salida.part_start=inicioPar;
                        salida.part_next=-1;
                        fseek(archivo,inicioPar,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo); // aca escribo el primer ebr de la particion extendida.
                        fclose(archivo);
                        printf("AVISO: Particion Extendida creada correctamente.\n");
                        BcreoP=1;
                    }

                    if(BcreoP==0){
                        printf("ERROR: Ya existen cuatro particiones primarias.\n");
                        printf("AVISO: No se pudo crear la particion Extendida.\n");
                    }
                }

            }else{
                printf("ERROR: No hay suficiente espacio para crear la particion.\n");
                printf("AVISO: No se pudo crear la particion Extendida.\n");
            }

        }else{
            printf("ERROR: No se pudo acceder al disco.\n");
            printf("AVISO: No se pudo crear la particion primaria.\n");
        }
    }else{
        printf("ERROR: El Disco a particionar no existe.\n");
        printf("AVISO: No se pudo crear la particion extendida.\n");
    }

}

void crearLogica(char ruta[], int tamanio, char fit1[], char nombre[]){
    int hayEspacio=0;
    QuitarComillas(ruta);
    QuitarComillas(nombre);
    if(existeCarpeta(ruta)==1){
        int tru=1;
        int posicion;
        int tamaEdisponible;
        int tamanioTmp=tamanio-sizeof(EBR)-1; //aca le resto al tamanio el ebr para que en la posicion tamanio+1 escribir el nuevo ebr
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            MBR entrada;
            EBR salida;
            EBR lectura;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);

            if(entrada.mbr_partition_1.part_type=='E' || entrada.mbr_partition_2.part_type=='E' || entrada.mbr_partition_3.part_type=='E' || entrada.mbr_partition_4.part_type=='E' ){

                if(entrada.mbr_partition_1.part_type=='E'){
                    //=========calculamos si hay espacio para la particion;
                    inicialesFinalesLog(ruta,entrada.mbr_partition_1.part_start,entrada.mbr_partition_1.part_size);
                    int inicioLibre;
                    rang *tmp;
                    tmp=cabRango;
                    while(tmp!=NULL){
                         int disponible=tmp->fin-tmp->inicio;
                         if(disponible>=tamanio){
                            inicioLibre=tmp->inicio;
                            hayEspacio=1;
                            break;
                         }
                    tmp=tmp->siguiente;
                    }
                    //======================fin=============================

                    if(hayEspacio==1){
                        int tamanoReal=tamanio-sizeof(EBR); //el tamano ya incluido el EBR siguiete
                        int siguienteEBR=inicioLibre+tamanoReal; //posicion donde va iniciar el ebr vacio
                        int actualEbr=inicioLibre-sizeof(EBR)-1; //posicion del ebr actual
                        //==================Actual EBR ===============================
                        fseek(archivo,actualEbr,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        lectura.part_next=siguienteEBR;
                        lectura.part_status='V';
                        strcpy(lectura.part_fit,fit1);
                        lectura.part_size=tamanoReal;
                        strcpy(lectura.part_name,nombre);
                        fseek(archivo,lectura.part_start,SEEK_SET);
                        fwrite(&lectura,sizeof(EBR),1,archivo);
                        //================sigueinte EBR===================
                        salida.part_status='F';
                        salida.part_start=siguienteEBR;
                        salida.part_next=-1;
                        fseek(archivo,siguienteEBR,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo);
                        fclose(archivo);
                        printf("AVISO: Particion Logica creada correctamente.\n");
                    }else{
                        printf("ERROR: NO  hay espacio suficiente.\n");
                        printf("AVISO: No se pudo crear la particion Logica.\n");
                    }

                }else  if(entrada.mbr_partition_2.part_type=='E'){
                    inicialesFinalesLog(ruta,entrada.mbr_partition_2.part_start,entrada.mbr_partition_2.part_size);
                    int inicioLibre;
                    rang *tmp;
                    tmp=cabRango;
                    while(tmp!=NULL){
                         int disponible=tmp->fin-tmp->inicio;
                         if(disponible>=tamanio){
                            inicioLibre=tmp->inicio;
                            hayEspacio=1;
                            break;
                         }
                    tmp=tmp->siguiente;
                    }
                    //======================fin=============================

                    if(hayEspacio==1){
                        int tamanoReal=tamanio-sizeof(EBR); //el tamano ya incluido el EBR siguiete
                        int siguienteEBR=inicioLibre+tamanoReal; //posicion donde va iniciar el ebr vacio
                        int actualEbr=inicioLibre-sizeof(EBR)-1; //posicion del ebr actual
                        //==================Actual EBR ===============================
                        fseek(archivo,actualEbr,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        lectura.part_next=siguienteEBR;
                        lectura.part_status='V';
                        strcpy(lectura.part_fit,fit1);
                        lectura.part_size=tamanoReal;
                        strcpy(lectura.part_name,nombre);
                        fseek(archivo,lectura.part_start,SEEK_SET);
                        fwrite(&lectura,sizeof(EBR),1,archivo);
                        //================sigueinte EBR===================
                        salida.part_status='F';
                        salida.part_start=siguienteEBR;
                        salida.part_next=-1;
                        fseek(archivo,siguienteEBR,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo);
                        fclose(archivo);
                        printf("AVISO: Particion Logica creada correctamente.\n");
                    }else{
                        printf("ERROR: NO  hay espacio suficiente.\n");
                        printf("AVISO: No se pudo crear la particion Logica.\n");
                    }

                }else if(entrada.mbr_partition_3.part_type=='E'){
                    //==============calculo del espacio======================
                    inicialesFinalesLog(ruta,entrada.mbr_partition_3.part_start,entrada.mbr_partition_3.part_size);
                    int inicioLibre;
                    rang *tmp;
                    tmp=cabRango;
                    while(tmp!=NULL){
                         int disponible=tmp->fin-tmp->inicio;
                         if(disponible>=tamanio){
                            inicioLibre=tmp->inicio;
                            hayEspacio=1;
                            break;
                         }
                    tmp=tmp->siguiente;
                    }
                    //======================fin=============================

                    if(hayEspacio==1){
                        int tamanoReal=tamanio-sizeof(EBR); //el tamano ya incluido el EBR siguiete
                        int siguienteEBR=inicioLibre+tamanoReal; //posicion donde va iniciar el ebr vacio
                        int actualEbr=inicioLibre-sizeof(EBR)-1; //posicion del ebr actual
                        //==================Actual EBR ===============================
                        fseek(archivo,actualEbr,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        lectura.part_next=siguienteEBR;
                        lectura.part_status='V';
                        strcpy(lectura.part_fit,fit1);
                        lectura.part_size=tamanoReal;
                        strcpy(lectura.part_name,nombre);
                        fseek(archivo,lectura.part_start,SEEK_SET);
                        fwrite(&lectura,sizeof(EBR),1,archivo);
                        //================sigueinte EBR===================
                        salida.part_status='F';
                        salida.part_start=siguienteEBR;
                        salida.part_next=-1;
                        fseek(archivo,siguienteEBR,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo);
                        fclose(archivo);
                        printf("AVISO: Particion Logica creada correctamente.\n");
                    }else{
                        printf("ERROR: NO  hay espacio suficiente.\n");
                        printf("AVISO: No se pudo crear la particion Logica.\n");
                    }

                }else if(entrada.mbr_partition_4.part_type=='E'){
                    inicialesFinalesLog(ruta,entrada.mbr_partition_4.part_start,entrada.mbr_partition_4.part_size);
                    int inicioLibre;
                    rang *tmp;
                    tmp=cabRango;
                    while(tmp!=NULL){
                         int disponible=tmp->fin-tmp->inicio;
                         if(disponible>=tamanio){
                            inicioLibre=tmp->inicio;
                            hayEspacio=1;
                            break;
                         }
                    tmp=tmp->siguiente;
                    }
                    //======================fin=============================

                    if(hayEspacio==1){
                        int tamanoReal=tamanio-sizeof(EBR); //el tamano ya incluido el EBR siguiete
                        int siguienteEBR=inicioLibre+tamanoReal; //posicion donde va iniciar el ebr vacio
                        int actualEbr=inicioLibre-sizeof(EBR)-1; //posicion del ebr actual
                        //==================Actual EBR ===============================
                        fseek(archivo,actualEbr,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        lectura.part_next=siguienteEBR;
                        lectura.part_status='V';
                        strcpy(lectura.part_fit,fit1);
                        lectura.part_size=tamanoReal;
                        strcpy(lectura.part_name,nombre);
                        fseek(archivo,lectura.part_start,SEEK_SET);
                        fwrite(&lectura,sizeof(EBR),1,archivo);
                        //================sigueinte EBR===================
                        salida.part_status='F';
                        salida.part_start=siguienteEBR;
                        salida.part_next=-1;
                        fseek(archivo,siguienteEBR,SEEK_SET);
                        fwrite(&salida,sizeof(EBR),1,archivo);
                        fclose(archivo);
                        printf("AVISO: Particion Logica creada correctamente.\n");
                    }else{
                        printf("ERROR: NO  hay espacio suficiente.\n");
                        printf("AVISO: No se pudo crear la particion Logica.\n");
                    }

                }
            }else{
                printf("ERROR: NO existe una particion extendida.\n");
                printf("AVISO: No se puede crear la particion Logica.\n");
            }
        }else{
            printf("ERROR: No se pudo acceder al disco.\n");
            printf("AVISO: No se pudo crear la particion Logica.\n");
        }
     }else{
        printf("ERROR: El Disco a particionar no existe.\n");
        printf("AVISO: No se pudo crear la particion Logica.\n");
     }
}

void addInicial(int numer){
    if(cabezaI==NULL){
		cabezaI=malloc(sizeof(iniciales));
		cabezaI->numero=numer;
		colaI=cabezaI;
	}else{
		iniciales *tmp;
		tmp=cabezaI;
		if(cabezaI->numero>numer){
			cabezaI=malloc(sizeof(iniciales));
			cabezaI->numero=numer;
			cabezaI->siguiente=tmp;
		}else if(colaI->numero<numer){
			colaI->siguiente=malloc(sizeof(iniciales));
			colaI->siguiente->numero=numer;
			colaI=colaI->siguiente;
		}else{

			while(tmp!=NULL){
                if(tmp->numero<numer && tmp->siguiente->numero>numer){
                    iniciales *aux=tmp->siguiente;
                    tmp->siguiente=malloc(sizeof(iniciales));
                    tmp->siguiente->numero=numer;
                    tmp->siguiente->siguiente=aux;
                    break;
                }
            tmp=tmp->siguiente;
			}
		}
	}
}

void addFinal(int numer){
    if(cabezaF==NULL){
		cabezaF=malloc(sizeof(finales));
		cabezaF->numero=numer;
		colaF=cabezaF;
	}else{
		finales *tmp;
		tmp=cabezaF;
		if(cabezaF->numero>numer){
			cabezaF=malloc(sizeof(finales));
			cabezaF->numero=numer;
			cabezaF->siguiente=tmp;
		}else if(colaF->numero<numer){
			colaF->siguiente=malloc(sizeof(finales));
			colaF->siguiente->numero=numer;
			colaF=colaF->siguiente;
		}else{
			while(tmp!=NULL){
                if(tmp->numero<numer && tmp->siguiente->numero>numer){
                    finales *aux=tmp->siguiente;
                    tmp->siguiente=malloc(sizeof(finales));
                    tmp->siguiente->numero=numer;
                    tmp->siguiente->siguiente=aux;
                    break;
                }
            tmp=tmp->siguiente;
			}
		}
	}
}

void impFinales(){
    if(cabezaF!=NULL){
        finales *tmp;
        tmp=cabezaF;
        while(tmp!=NULL){
            printf("Valor: %i\n",tmp->numero);
        tmp=tmp->siguiente;
        }
    }
}

void impIniciales(){
    if(cabezaI!=NULL){
        iniciales *tmp;
        tmp=cabezaI;
        while(tmp!=NULL){
            printf("Valor: %i\n",tmp->numero);
        tmp=tmp->siguiente;
        }
    }
}

void addRango(int inicio,int fin){

    if(cabRango==NULL){
        cabRango=malloc(sizeof(rang));
        cabRango->inicio=inicio;
        cabRango->fin=fin;
        cabRango->siguiente=NULL;
        colRango=cabRango;
    }else{
        colRango->siguiente=malloc(sizeof(rang));
        colRango->siguiente->inicio=inicio;
        colRango->siguiente->fin=fin;
        colRango=colRango->siguiente;
        colRango->siguiente=NULL;
    }
}

void actualizarRangos(int tamaDisco){

    if(cabezaI!=NULL){
        iniciales *tmp;
        finales *tmp2;
        tmp=cabezaI;
        tmp2=cabezaF;
        while(tmp!=NULL){
            if(tmp->siguiente==NULL){
                addRango(tmp2->numero,tamaDisco);
            }else{
                addRango(tmp2->numero,tmp->siguiente->numero);
            }
        tmp=tmp->siguiente;
        tmp2=tmp2->siguiente;
        }
    }
}

void inicialesFinales(char ruta[]){
    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            MBR entrada;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);

            addInicial(0);
            addFinal(sizeof(MBR)+1); //le sumo un bit para que no se pierda un bit de cualquiera de las dos esrcutras que van a ser vecinas 150-151

            if(entrada.mbr_partition_1.part_status=='V'){
                int fin=entrada.mbr_partition_1.part_start + entrada.mbr_partition_1.part_size+1;
                addInicial(entrada.mbr_partition_1.part_start);
                addFinal(fin);
            }

            if(entrada.mbr_partition_2.part_status=='V'){
                int fin=entrada.mbr_partition_2.part_start + entrada.mbr_partition_2.part_size+1;
                addInicial(entrada.mbr_partition_2.part_start);
                addFinal(fin);
            }

            if(entrada.mbr_partition_3.part_status=='V'){
                int fin=entrada.mbr_partition_3.part_start + entrada.mbr_partition_3.part_size+1;
                addInicial(entrada.mbr_partition_3.part_start);
                addFinal(fin);
            }

            if(entrada.mbr_partition_4.part_status=='V'){
                int fin=entrada.mbr_partition_4.part_start + entrada.mbr_partition_4.part_size+1;
                addInicial(entrada.mbr_partition_4.part_start);
                addFinal(fin);
            }

            actualizarRangos(entrada.mbr_tamano);
            //imprimeRangos();
        }
    }
}

void inicialesFinalesLog(char ruta[], int inicioE, int tamaE){
    int tru=1;
    int posicion=0;
    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            EBR lectura;
            posicion=inicioE;
            while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                fseek(archivo,posicion,SEEK_SET);
                fread(&lectura,sizeof(EBR),1,archivo);
                if(lectura.part_next==-1){
                    addInicial(lectura.part_start);
                    addFinal(lectura.part_start+sizeof(EBR)+1);
                    tru=0;
                }else{
                    posicion=lectura.part_next;
                    addInicial(lectura.part_start);
                    addFinal(lectura.part_start+sizeof(EBR)+lectura.part_size+1);
                }
            }
            actualizarRangos(inicioE+tamaE);
            //imprimeRangos();
        }
    }
}

void limpiarIniciales(){
    if(cabezaI!=NULL){
        iniciales *tmp=cabezaI;
        while(tmp!=NULL){
            free(tmp);
        tmp=tmp->siguiente;
        }
        cabezaI=NULL;
        colaI=NULL;
    }
}

void limpiarFinales(){
    if(cabezaF!=NULL){
        finales *tmp=cabezaF;
        while(tmp!=NULL){
            free(tmp);
        tmp=tmp->siguiente;
        }
        cabezaF=NULL;
        colaF=NULL;
    }
}

void limpiarRangos(){
    if(cabRango!=NULL){
        rang *tmp=cabRango;
        while(tmp!=NULL){
            free(tmp);
        tmp=tmp->siguiente;
        }
        cabRango=NULL;
        colRango=NULL;
    }
}

void imprimeRangos(){

    if(cabezaF!=NULL){
        rang *tmp;
        tmp=cabRango;
        while(tmp!=NULL){
            printf("Inicio: %i  Fin: %i \n",tmp->inicio,tmp->fin);
        tmp=tmp->siguiente;
        }
    }
}

int VerificarNombre(char ruta[],char nombre[]){
    QuitarComillas(ruta);
    QuitarComillas(nombre);

    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            MBR entrada;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);

            if(entrada.mbr_partition_1.part_status='V'){

                if(strcasecmp(entrada.mbr_partition_1.part_name,nombre)==0){
                    return 1;
                }
            }

            if(entrada.mbr_partition_2.part_status='V'){
                if(strcasecmp(entrada.mbr_partition_2.part_name,nombre)==0){
                    return 1;
                }
            }

            if(entrada.mbr_partition_3.part_status='V'){
                if(strcasecmp(entrada.mbr_partition_3.part_name,nombre)==0){
                    return 1;
                }
            }

            if(entrada.mbr_partition_4.part_status='V'){
                if(strcasecmp(entrada.mbr_partition_4.part_name,nombre)==0){
                    return 1;
                }
            }
        }
    }
    return 0;
}

int VerificarNombre2(char ruta[],char nombre[]){
    QuitarComillas(ruta);
    QuitarComillas(nombre);
    int tru=1;
    if(existeCarpeta(ruta)==1){
        FILE *archivo;
        archivo=fopen(ruta,"r+b");
        if(archivo!=NULL){
            int posicion=0;
            MBR entrada;
            EBR lectura;
            fseek(archivo,0,SEEK_SET);
            fread(&entrada,sizeof(MBR),1,archivo);

            if(entrada.mbr_partition_1.part_type=='E'){
                    posicion=entrada.mbr_partition_1.part_start;
                    while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                        fseek(archivo,posicion,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        if(lectura.part_next==-1){
                            tru=0;
                        }else{
                            posicion=lectura.part_next;
                            if(strcasecmp(lectura.part_name,nombre)==0){
                                fclose(archivo);
                                return 1;
                            }
                        }
                    }
                fclose(archivo);
                return 0;
            }

            if(entrada.mbr_partition_2.part_type=='E'){
                    posicion=entrada.mbr_partition_2.part_start;
                    while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                        fseek(archivo,posicion,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        if(lectura.part_next==-1){
                            tru=0;
                        }else{
                            posicion=lectura.part_next;
                            if(strcasecmp(lectura.part_name,nombre)==0){
                                fclose(archivo);
                                return 1;
                            }
                        }
                    }
                fclose(archivo);
                return 0;
            }

            if(entrada.mbr_partition_3.part_type=='E'){

                    posicion=entrada.mbr_partition_3.part_start;
                    while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                        fseek(archivo,posicion,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        if(lectura.part_next==-1){
                            tru=0;
                        }else{
                            posicion=lectura.part_next;
                            if(strcasecmp(lectura.part_name,nombre)==0){
                                fclose(archivo);
                                return 1;
                            }
                        }
                    }
                fclose(archivo);
                return 0;
            }

            if(entrada.mbr_partition_4.part_type=='E'){
                    posicion=entrada.mbr_partition_4.part_start;

                    while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                        fseek(archivo,posicion,SEEK_SET);
                        fread(&lectura,sizeof(EBR),1,archivo);
                        if(lectura.part_next==-1){
                            tru=0;
                        }else{
                            posicion=lectura.part_next;
                            if(strcasecmp(lectura.part_name,nombre)==0){
                                fclose(archivo);
                                return 1;
                            }
                        }
                    }
                fclose(archivo);
                return 0;
            }
        }
    }
    return 0;
}

//========================metodos de edicion de particiones===============================

void eliminarLogica(char ruta[],char nombre[], char tipo[]){

        QuitarComillas(ruta);
        QuitarComillas(nombre);
        int posAnterior=0;
        int posSiguiente=0;
        int tru=1;
        if(existeCarpeta(ruta)==1){
            FILE *archivo;
            archivo=fopen(ruta,"r+b");
            if(archivo!=NULL){
                int posicion=0;
                MBR entrada;
                EBR lectura;
                fseek(archivo,0,SEEK_SET);
                fread(&entrada,sizeof(MBR),1,archivo);

                if(entrada.mbr_partition_1.part_type=='E'){

                        posicion=entrada.mbr_partition_1.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            posAnterior=lectura.part_start;
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                fseek(archivo,posicion,SEEK_SET);
                                fread(&lectura,sizeof(EBR),1,archivo);
                                //=============vemos si hay un siguiente para dejarlo apuntado======
                                if(lectura.part_next==-1){
                                    posSiguiente=0;
                                }else{
                                    posSiguiente=lectura.part_next;
                                }
                                //==================================================================
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    if(posSiguiente==0){ // su anterior va dejar de apuntarlo por que no tiene siguientes
                                        EBR escritura;  //como este ebr esta vacio entonces el anterio hay que dejarlo vacio tambien
                                        if(strcasecmp(tipo,"FAST")==0){
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            strcpy(escritura.part_name,"\0");
                                            escritura.part_size=0;
                                            escritura.part_next=0;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET); //aca pongo con null todos los bists de este EBR
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }else{
                                       EBR escritura; //este va apuntar a su sigueinte pero es el caso donde el ebr no esta vacio, asi que no se debe perde la info
                                        if(strcasecmp(tipo,"FAST")==0){ //solo se actualiza, se lee se actualiza y se excribe de nuevo
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;//aca le doy la posicion siguiente
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET);
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }

                                }
                            }
                        }
                    fclose(archivo);
                }

                if(entrada.mbr_partition_2.part_type=='E'){
                        posicion=entrada.mbr_partition_2.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            posAnterior=lectura.part_start;
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                fseek(archivo,posicion,SEEK_SET);
                                fread(&lectura,sizeof(EBR),1,archivo);
                                //=============vemos si hay un siguiente para dejarlo apuntado======
                                if(lectura.part_next==-1){
                                    posSiguiente=0;
                                }else{
                                    posSiguiente=lectura.part_next;
                                }
                                //==================================================================
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    if(posSiguiente==0){ // su anterior va dejar de apuntarlo por que no tiene siguientes
                                        EBR escritura;  //como este ebr esta vacio entonces el anterio hay que dejarlo vacio tambien
                                        if(strcasecmp(tipo,"FAST")==0){
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            strcpy(escritura.part_name,"\0");
                                            escritura.part_size=0;
                                            escritura.part_next=0;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET); //aca pongo con null todos los bists de este EBR
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }else{
                                       EBR escritura; //este va apuntar a su sigueinte pero es el caso donde el ebr no esta vacio, asi que no se debe perde la info
                                        if(strcasecmp(tipo,"FAST")==0){ //solo se actualiza, se lee se actualiza y se excribe de nuevo
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;//aca le doy la posicion siguiente
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET);
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }

                                }
                            }
                        }
                    fclose(archivo);
                }

                if(entrada.mbr_partition_3.part_type=='E'){

                        posicion=entrada.mbr_partition_3.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            posAnterior=lectura.part_start;
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                fseek(archivo,posicion,SEEK_SET);
                                fread(&lectura,sizeof(EBR),1,archivo);
                                //=============vemos si hay un siguiente para dejarlo apuntado======
                                if(lectura.part_next==-1){
                                    posSiguiente=0;
                                }else{
                                    posSiguiente=lectura.part_next;
                                }
                                //==================================================================
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    if(posSiguiente==0){ // su anterior va dejar de apuntarlo por que no tiene siguientes
                                        EBR escritura;  //como este ebr esta vacio entonces el anterio hay que dejarlo vacio tambien
                                        if(strcasecmp(tipo,"FAST")==0){
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            strcpy(escritura.part_name,"\0");
                                            escritura.part_size=0;
                                            escritura.part_next=0;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET); //aca pongo con null todos los bists de este EBR
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }else{
                                       EBR escritura; //este va apuntar a su sigueinte pero es el caso donde el ebr no esta vacio, asi que no se debe perde la info
                                        if(strcasecmp(tipo,"FAST")==0){ //solo se actualiza, se lee se actualiza y se excribe de nuevo
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;//aca le doy la posicion siguiente
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET);
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }

                                }
                            }
                        }
                    fclose(archivo);
                }

                if(entrada.mbr_partition_4.part_type=='E'){

                        posicion=entrada.mbr_partition_4.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            posAnterior=lectura.part_start;
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                fseek(archivo,posicion,SEEK_SET);
                                fread(&lectura,sizeof(EBR),1,archivo);
                                //=============vemos si hay un siguiente para dejarlo apuntado======
                                if(lectura.part_next==-1){
                                    posSiguiente=0;
                                }else{
                                    posSiguiente=lectura.part_next;
                                }
                                //==================================================================
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    if(posSiguiente==0){ // su anterior va dejar de apuntarlo por que no tiene siguientes
                                        EBR escritura;  //como este ebr esta vacio entonces el anterio hay que dejarlo vacio tambien
                                        if(strcasecmp(tipo,"FAST")==0){
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            escritura.part_status='F';
                                            escritura.part_next=-1;
                                            strcpy(escritura.part_name,"\0");
                                            escritura.part_size=0;
                                            escritura.part_next=0;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET); //aca pongo con null todos los bists de este EBR
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }else{
                                       EBR escritura; //este va apuntar a su sigueinte pero es el caso donde el ebr no esta vacio, asi que no se debe perde la info
                                        if(strcasecmp(tipo,"FAST")==0){ //solo se actualiza, se lee se actualiza y se excribe de nuevo
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;//aca le doy la posicion siguiente
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                                        }else{
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fread(&escritura,sizeof(EBR),1,archivo);
                                            escritura.part_next=posSiguiente;
                                            fseek(archivo,posAnterior,SEEK_SET);
                                            fwrite(&escritura,sizeof(EBR),1,archivo);
                                            char uno='\0';
                                            fseek(archivo,posicion,SEEK_SET);
                                            fwrite(&uno,sizeof(EBR),1,archivo);
                                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                                        }
                                    }

                                }
                            }
                        }
                    fclose(archivo);
                }
            }
        }
}

void eliminarParticiones(char ruta[], char nombre[],char tipo[]){

    int BborroP=0;
    if(VerificarNombre(ruta,nombre)==1){
        QuitarComillas(ruta);
        QuitarComillas(nombre);
        if(existeCarpeta(ruta)==1){
            FILE *archivo;
            archivo=fopen(ruta,"r+b");
            if(archivo!=NULL){
                MBR entrada;
                fseek(archivo,0,SEEK_SET);
                fread(&entrada,sizeof(MBR),1,archivo);

                if(entrada.mbr_partition_1.part_status='V' && BborroP==0 ){

                    if(strcasecmp(entrada.mbr_partition_1.part_name,nombre)==0){

                        if(strcasecmp(tipo,"FAST")){
                            entrada.mbr_partition_1.part_status='F';
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente.\n");
                        }else{
                            entrada.mbr_partition_1.part_status='F';
                            entrada.mbr_partition_1.part_type="\0";
                            strcpy(entrada.mbr_partition_1.part_fit,"\0");
                            entrada.mbr_partition_1.part_start=0;
                            entrada.mbr_partition_1.part_size=0;
                            strcpy(entrada.mbr_partition_1.part_name,"\0");
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente.\n");
                            BborroP=1;
                        }

                    }
                }

                if(entrada.mbr_partition_2.part_status='V' && BborroP==0 ){

                        if(strcasecmp(tipo,"FAST")){
                            entrada.mbr_partition_2.part_status='F';
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                        }else{
                            entrada.mbr_partition_2.part_status='F';
                            entrada.mbr_partition_2.part_type="\0";
                            strcpy(entrada.mbr_partition_2.part_fit,"\0");
                            entrada.mbr_partition_2.part_start=0;
                            entrada.mbr_partition_2.part_size=0;
                            strcpy(entrada.mbr_partition_2.part_name,"\0");
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                            BborroP=1;
                        }
                }

                if(entrada.mbr_partition_3.part_status='V' && BborroP==0 ) {

                        if(strcasecmp(tipo,"FAST")){
                            entrada.mbr_partition_3.part_status='F';
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                        }else{
                            entrada.mbr_partition_3.part_status='F';
                            entrada.mbr_partition_3.part_type="\0";
                            strcpy(entrada.mbr_partition_3.part_fit,"\0");
                            entrada.mbr_partition_3.part_start=0;
                            entrada.mbr_partition_3.part_size=0;
                            strcpy(entrada.mbr_partition_3.part_name,"\0");
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                            BborroP=1;
                        }
                }

                if(entrada.mbr_partition_4.part_status='V' && BborroP==0 ){

                        if(strcasecmp(tipo,"FAST")){
                            entrada.mbr_partition_4.part_status='F';
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: fast.\n");
                        }else{
                            entrada.mbr_partition_4.part_status='F';
                            entrada.mbr_partition_4.part_type="\0";
                            strcpy(entrada.mbr_partition_4.part_fit,"\0");
                            entrada.mbr_partition_4.part_start=0;
                            entrada.mbr_partition_4.part_size=0;
                            strcpy(entrada.mbr_partition_4.part_name,"\0");
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&entrada,sizeof(MBR),1,archivo); // aca escribo de nuevo el MBR
                            fclose(archivo);
                            printf("AVISO: Particion eliminada correctamente en modo: full.\n");
                            BborroP=1;
                        }
                }
            }else{
                printf("ERROR: No se pudo acceder al disco.\n");
                printf("AVISO: No se pudo eliminar la particion.\n");
            }
        }else{
            printf("ERROR: La ruta ingresada no referencia ningun disco.\n");
            printf("AVISO: No se pudo eliminar la particion.\n");
        }

    }else if(VerificarNombre2(ruta,nombre)==1){
        eliminarLogica(ruta,nombre,tipo);
    }else{
        printf("ERROR: No existe niguna particion con el nombre introducido.\n");
        printf("AVISO: No se pudo eliminar la particion.\n");
    }

}

void montarParticion(char ruta[],char nombre[]){

    int BmontoP=0;
    char nombreDisco[25];
    splitPath2(ruta);
    strcpy(nombreDisco,colaR->nombre);
    if(VerificarNombre(ruta,nombre)==1){
        QuitarComillas(ruta);
        QuitarComillas(nombre);
        if(existeCarpeta(ruta)==1){
            FILE *archivo;
            archivo=fopen(ruta,"r+b");
            if(archivo!=NULL){
                MBR entrada;
                fseek(archivo,0,SEEK_SET);
                fread(&entrada,sizeof(MBR),1,archivo);

                if(entrada.mbr_partition_1.part_status='V' && BmontoP==0){

                    if(strcasecmp(entrada.mbr_partition_1.part_name,nombre)==0 ){
                        letter *disco;
                        disco=DiscoMontado(nombreDisco);
                        if(disco==NULL){
                            addDisco(nombreDisco);
                            disco=DiscoMontado(nombreDisco);
                            addParticion(nombre,disco,ruta);
                            printf("AVISO: Particion montada correctamente.\n");
                            BmontoP=1;
                        }else{
                            if(particionMontada(nombre,disco)==1){
                                printf("AVISO: La particion ya se encuentra montada.\n");
                                BmontoP=1;
                            }else{
                                addParticion(nombre,disco,ruta);
                                printf("AVISO: Particion montada correctamente.\n");
                                BmontoP=1;
                            }
                        }
                    }
                }

                if(entrada.mbr_partition_2.part_status='V' && BmontoP==0 ){

                    if(strcasecmp(entrada.mbr_partition_2.part_name,nombre)==0 ){
                        letter *disco;
                        disco=DiscoMontado(nombreDisco);
                        if(disco==NULL){
                            addDisco(nombreDisco);
                            disco=DiscoMontado(nombreDisco);
                            addParticion(nombre,disco,ruta);
                            printf("AVISO: Particion montada correctamente.\n");
                            BmontoP=1;
                        }else{
                            if(particionMontada(nombre,disco)==1){
                                printf("AVISO: La particion ya se encuentra montada.\n");
                                BmontoP=1;
                            }else{
                                addParticion(nombre,disco,ruta);
                                printf("AVISO: Particion montada correctamente.\n");
                                BmontoP=1;
                            }
                        }
                    }
                }

                if(entrada.mbr_partition_3.part_status='V' && BmontoP==0 ) {

                    if(strcasecmp(entrada.mbr_partition_3.part_name,nombre)==0 ){
                        letter *disco;
                        disco=DiscoMontado(nombreDisco);
                        if(disco==NULL){
                            addDisco(nombreDisco);
                            disco=DiscoMontado(nombreDisco);
                            addParticion(nombre,disco,ruta);
                            printf("AVISO: Particion montada correctamente.\n");
                            BmontoP=1;
                        }else{
                            if(particionMontada(nombre,disco)==1){
                                printf("AVISO: La particion ya se encuentra montada.\n");
                            }else{
                                addParticion(nombre,disco,ruta);
                                printf("AVISO: Particion montada correctamente.\n");
                            }
                        }
                    }
                }

                if(entrada.mbr_partition_4.part_status='V' && BmontoP==0 ){

                    if(strcasecmp(entrada.mbr_partition_4.part_name,nombre)==0 ){
                        letter *disco;
                        disco=DiscoMontado(nombreDisco);
                        if(disco==NULL){
                            addDisco(nombreDisco);
                            disco=DiscoMontado(nombreDisco);
                            addParticion(nombre,disco,ruta);
                            printf("AVISO: Particion montada correctamente.\n");
                            BmontoP=1;
                        }else{
                            if(particionMontada(nombre,disco)==1){
                                printf("AVISO: La particion ya se encuentra montada.\n");
                                BmontoP=1;
                            }else{
                                addParticion(nombre,disco,ruta);
                                printf("AVISO: Particion montada correctamente.\n");
                                BmontoP=1;
                            }
                        }
                    }
                }

            if(BmontoP==0){
                printf("AVISO: No se pudo montar la particion.\n");
            }

            }else{
                printf("ERROR: No se pudo acceder al disco.\n");
                printf("AVISO: No se pudo montar la particion.\n");
            }
        }else{
            printf("ERROR: La ruta ingresada no referencia ningun disco.\n");
            printf("AVISO: No se pudo montar la particion.\n");
        }

    }else if(VerificarNombre2(ruta,nombre)==1){
        montarLogicas(ruta,nombre,nombreDisco);
    }else{
        printf("ERROR: No existe niguna particion con el nombre introducido.\n");
        printf("AVISO: No se pudo montar la particion.\n");
    }
}

int montarLogicas(char ruta[],char nombre [], char nombreDisco[]){
    int posAnterior=0;
        int posSiguiente=0;
        int tru=1;
        if(existeCarpeta(ruta)==1){
            FILE *archivo;
            archivo=fopen(ruta,"r+b");
            if(archivo!=NULL){
                int posicion=0;
                MBR entrada;
                EBR lectura;
                fseek(archivo,0,SEEK_SET);
                fread(&entrada,sizeof(MBR),1,archivo);

                if(entrada.mbr_partition_1.part_type=='E'){
                        posicion=entrada.mbr_partition_1.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                     letter *disco;
                                     disco=DiscoMontado(nombreDisco);
                                     if(disco==NULL){
                                        addDisco(nombre);
                                        disco=DiscoMontado(nombreDisco);
                                        addParticion(nombre,disco,ruta);
                                        printf("AVISO: Particion montada correctamente.\n");
                                     }else{
                                        if(particionMontada(nombre,disco)==1){
                                            printf("AVISO: La particion ya se encuentra montada.\n");
                                        }else{
                                            addParticion(nombre,disco,ruta);
                                            printf("AVISO: Particion montada correctamente.\n");
                                        }
                                     }
                                    fclose(archivo);
                                    return 1;
                                }
                            }
                        }
                    fclose(archivo);
                    return 0;
                }

                if(entrada.mbr_partition_2.part_type=='E'){
                        posicion=entrada.mbr_partition_2.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    letter *disco;
                                     disco=DiscoMontado(nombreDisco);
                                     if(disco==NULL){
                                        addDisco(nombre);
                                        disco=DiscoMontado(nombreDisco);
                                        addParticion(nombre,disco,ruta);
                                        printf("AVISO: Particion montada correctamente.\n");
                                     }else{
                                        if(particionMontada(nombre,disco)==1){
                                            printf("AVISO: La particion ya se encuentra montada.\n");
                                        }else{
                                            addParticion(nombre,disco,ruta);
                                            printf("AVISO: Particion montada correctamente.\n");
                                        }
                                     }
                                    fclose(archivo);
                                    return 1;
                                }
                            }
                        }
                    fclose(archivo);
                    return 0;
                }

                if(entrada.mbr_partition_3.part_type=='E'){

                        posicion=entrada.mbr_partition_3.part_start;
                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    letter *disco;
                                     disco=DiscoMontado(nombreDisco);
                                     if(disco==NULL){
                                        addDisco(nombre);
                                        disco=DiscoMontado(nombreDisco);
                                        addParticion(nombre,disco,ruta);
                                        printf("AVISO: Particion montada correctamente.\n");
                                     }else{
                                        if(particionMontada(nombre,disco)==1){
                                            printf("AVISO: La particion ya se encuentra montada.\n");
                                        }else{
                                            addParticion(nombre,disco,ruta);
                                            printf("AVISO: Particion montada correctamente.\n");
                                        }
                                     }

                                    fclose(archivo);
                                    return 1;
                                }
                            }
                        }
                    fclose(archivo);
                    return 0;
                }

                if(entrada.mbr_partition_4.part_type=='E'){
                        posicion=entrada.mbr_partition_4.part_start;

                        while (tru==1){ //este while me ubica en el ultimo EBR que haya;
                            fseek(archivo,posicion,SEEK_SET);
                            fread(&lectura,sizeof(EBR),1,archivo);
                            if(lectura.part_next==-1){
                                tru=0;
                            }else{
                                posicion=lectura.part_next;
                                if(strcasecmp(lectura.part_name,nombre)==0){
                                    letter *disco;
                                     disco=DiscoMontado(nombreDisco);
                                     if(disco==NULL){
                                        addDisco(nombre);
                                        disco=DiscoMontado(nombreDisco);
                                        addParticion(nombre,disco,ruta);
                                        printf("AVISO: Particion montada correctamente.\n");
                                     }else{
                                        if(particionMontada(nombre,disco)==1){
                                            printf("AVISO: La particion ya se encuentra montada.\n");
                                        }else{
                                            addParticion(nombre,disco,ruta);
                                            printf("AVISO: Particion montada correctamente.\n");
                                        }
                                     }
                                    fclose(archivo);
                                    return 1;
                                }
                            }
                        }
                    fclose(archivo);
                    return 0;
                }
            }
        }

}

void obtenerNombreDisco( char ruta[],char nombre[]){
    splitPath2(ruta);
    strcpy(nombre,colaR->nombre);//aca obtengo el nombre del disco
}

void asignarLetra(char cadena[],char letra){
    cadena[0]=letra;
    cadena[1]='\0';
}

letter * DiscoMontado(char nombre[]){

    if(cabezaL!=NULL){
        letter *tmp;
        tmp=cabezaL;
        while(tmp!=NULL){
            if(strcasecmp(tmp->nombreDisco,nombre)==0){
                return tmp;
            }
        tmp=tmp->siguiente;
        }
    return NULL;
    }

}

void addDisco(char nombre[]){

    if(cabezaL==NULL){
        cabezaL=malloc(sizeof(letter));
        cabezaL->id=0;
        asignarLetra(cabezaL->letra,letras[cabezaL->id]);
        strcpy(cabezaL->nombreDisco,nombre);
        cabezaL->siguiente=NULL;
        cabezaL->abajo=NULL;
        colaL=cabezaL;
    }else{
        letter *disponible;
        disponible=montajeDisponible();
        if(disponible!=NULL){
            strcpy(disponible->nombreDisco,nombre);
        }else{
            letter *tmp;
            tmp= colaL;
            colaL->siguiente=malloc(sizeof(letter));
            colaL->siguiente->id=(tmp->id+1);
            asignarLetra(colaL->siguiente->letra,letras[tmp->id+1]);
            strcpy(colaL->siguiente->nombreDisco,nombre);
            colaL->siguiente->abajo=NULL;
            colaL=colaL->siguiente;
            colaL->siguiente=NULL;
        }

    }

}

int particionMontada(char nombre[], letter *disco){

    if(disco!=NULL){
        if(disco->abajo!=NULL){
            number *tmp;
            tmp=disco->abajo;
            while(tmp!=NULL){
                if(strcasecmp(tmp->nombreParticion,nombre)==0){
                    return 1;
                }
            tmp=tmp->abajo;
            }

        }else{
            return 0;
        }
    }

}

void addParticion(char nombre[], letter *disco,char ruta[]){

    if(disco!=NULL){
        if(disco->abajo==NULL){
            disco->abajo=malloc(sizeof(number));
            disco->abajo->id=0;
            strcpy(disco->abajo->nombreParticion,nombre);
            strcpy(disco->abajo->ruta,ruta);
            sprintf(disco->abajo->idParticion,"vd%s%i",disco->letra,disco->abajo->id);
            disco->abajo->abajo=NULL;
        }else{
            number *tmp;
            tmp=disco->abajo;
            while(tmp->abajo!=NULL){
                tmp=tmp->abajo;
            }
            tmp->abajo=malloc(sizeof(number));
            tmp->abajo->id=tmp->id+1;
            strcpy(tmp->abajo->nombreParticion,nombre);
            strcpy(tmp->abajo->ruta,ruta);
            sprintf(tmp->abajo->idParticion,"vd%s%i",disco->letra,tmp->id+1);
            tmp->abajo->abajo=NULL;
        }
    }
}

void imprimirMontaje(){

    if(cabezaL!=NULL){
        letter *tmpL;
        tmpL=cabezaL;
        number *tmpN;
        printf("%4s\t\t\t%2s\t\t\t%2s\n","ID","RUTA","NOMBRE");
        while(tmpL!=NULL){
            tmpN=tmpL->abajo;
            while(tmpN!=NULL){
            printf("%4s\t\t\t%2s\t\t%2s\n",tmpN->idParticion,tmpN->ruta,tmpN->nombreParticion);
            tmpN=tmpN->abajo;
            }
        tmpL=tmpL->siguiente;
        }

    }

}

void desmontarPar(char idePAr[]){

    if(cabezaL!=NULL){
        letter *tmpL;
        tmpL=cabezaL;
        number *tmpN;
        number *primera;
        number *anterior;
        while(tmpL!=NULL){
            primera=tmpL->abajo;
            tmpN=tmpL->abajo;
            while(tmpN!=NULL){
                if(strcasecmp(tmpN->idParticion,idePAr)==0){
                    if(primera==tmpN){
                        tmpL->abajo=tmpN->abajo;
                        free(tmpN);
                        printf("AVISO: Particion desmontada correctamente.\n");
                        break;
                    }else if(tmpN->abajo!=NULL){
                        anterior->abajo=tmpN->abajo;
                        free(tmpN);
                        printf("AVISO: Particion desmontada correctamente.\n");
                        break;
                    }else if(tmpN->abajo==NULL){
                        anterior->abajo=NULL;
                        free(tmpN);
                        printf("AVISO: Particion desmontada correctamente.\n");
                        break;
                    }
                }
            anterior=tmpN;
            tmpN=tmpN->abajo;
            }
        tmpL=tmpL->siguiente;
        }

        tmpL=cabezaL;

        while(tmpL!=NULL){
            if(tmpL->abajo==NULL){
                strcpy(tmpL->nombreDisco,"dis");
            }
        tmpL=tmpL->siguiente;
        }

    }
}

letter * montajeDisponible(){

    if(cabezaL!=NULL){
        letter*tmpL;
        tmpL=cabezaL;
        while(tmpL!=NULL){
            if(strcasecmp(tmpL->nombreDisco,"dis")==0){
                return tmpL;
            }
        tmpL=tmpL->siguiente;
        }
     return NULL;
    }

}

//=============================lectura de archivo=============

void leeArchivo(char ruta[]){
    FILE *archivo;
    QuitarComillas(ruta);
    archivo=fopen(ruta,"r");
    if(archivo!=NULL){

       while (!feof(archivo)){
            leefichero(linea,VAL,archivo);
            analisisLexico(linea);
            if(segundaLinea(linea)==1){
                leefichero(linea, VAL, archivo);
                analisisLexico(linea);
            }

            //imprimirTokens();
            if(cabezaT != NULL){
                tokenActual=cabezaT->token;
                tokActual=cabezaT;
            }
            realizarAcciones();
        }
    fclose(archivo);
    }else{
        printf("ERROR: No se pudo acceder al archivo.\n");
        printf("AVISO: Verifique la ruta.\n");
    }
}

//=============================================================================Metodos de analisis sintactico===========================================================================================

void realizarAcciones(){
    origen();
    limpiarRutas();
    limpiarTokens();
    reiniciarBanderas();
    tokActual=NULL;
    limpiarIniciales();
    limpiarFinales();
    limpiarRangos();
    //metodo que borra la tabla de tokens para analizar la siguiente linea.
}

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
}

//===================inicio del analisis sintactico y validaciones=================================================

void origen(){
    if(tokActual!=NULL){
         switch (tokenActual) {
            case 9:{
                avanzarToken();
                COM1();
                if(error==0){ // aca pienso hacer las validaciones y la creacion de los archs

                    if(Bpath==0 || Bsize==0 || Bname==0){
                        if(Bpath==0){
                            printf("ERROR: Falta el parametro Path.\n");
                        }else if( Bname==0){
                            printf("ERROR: Falta el parametro Name.\n");
                        }else{
                            printf("ERROR: Falta el parametro Size.\n");
                        }
                        printf("AVISO: No se pudo crear el disco.\n");
                        break;
                    }

                    if(Buni==1){
                            if( (strcasecmp(unidad,"M")==0) || (strcasecmp(unidad,"K")==0)){
                                if (strcasecmp(unidad,"M")==0){
                                    if(size<10){
                                        printf("ERROR: El tamano minimo de un disco es de 10 MB.\n");
                                        printf("AVISO: No se pudo crear el disco.\n");
                                        break;
                                    }else{
                                        tamano=1024*1024*size;
                                    }
                                }else{
                                    if(size<10240){
                                        printf("ERROR: El tamano minimo de un disco es de 10240 KB.\n");
                                        printf("AVISO: No se pudo crear el disco.\n");
                                        break;
                                    }else{
                                        tamano=1024*size;
                                    }
                                }
                            }else{
                                printf("ERROR: El parametro de unit no es valido.\n");
                                printf("AVISO: No se pudo crear el disco.\n");
                                break;
                            }
                    }else{
                        if(size<10){
                            printf("ERROR: El tamano minimo de un disco es de 10 MB.\n");
                            printf("AVISO: No se pudo crear el disco.\n");
                            break;
                        }else{
                            tamano=1024*1024*size;
                        }
                    }
                //EJECUCION si todo va bien hasta este punto;
                    crearDisco();

                }else{
                   printf("AVISO: No se pudo crear el disco.\n");
                }
            break;
            }
            case 14:{
                avanzarToken();

                if(tokenActual==7){ // token = ::
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba \"-\" (path...).\n");
                    break;
                }

                if(tokenActual==12){ // token = ::
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba Phat despues de \"-\".\n");
                    break;
                }

                if(tokenActual==5){ // token = ::
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba \"::\" despues de path.\n");
                    break;
                }
                if(tokenActual==4){ //token = numero
                    strcpy(path,tokActual->lexema); // global
                    //============EJECUCION========================
                    borrarDisco(path);
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba una cadea despues de \"::\".\n");
                    break;
                }

            break;
            }
            case 15:{ //fdisk
                avanzarToken();
                COM2();
                 if(error==0){ //operaciones fdisk
                    ValidarOperacion(); //esta funcion diferencia si es un add, delete o creacion.
                 }else{
                    printf("AVISO: No se pudo crear la particion.\n");
                 }
            break;
            }
            case 21:{ //mount
                avanzarToken();
                COM3();
                if(error==0){ //operaciones mount
                     montarParticion(path,name);
                 }else{
                    printf("AVISO: No se pudo montar la particion.\n");
                 }
            break;
            }
            case 23:{
                avanzarToken();

                if(tokenActual==7){ // token = -
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba \"-\" (obligatorio).\n");
                    break;
                }

                if(tokenActual==22){ // token = id
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba id despues de \"-\".\n");
                    break;
                }

                if(tokenActual==1){ // token = id
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba un id valido (idn).\n");
                    break;
                }

                if(tokenActual==5){ // token = ::
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba \"::\" despues de id.\n");
                    break;
                }

                if(tokenActual==2){ // token = id
                    strcpy(idPar,tokActual->lexema);
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba \"::\" despues de id.\n");
                    break;
                }

                if(error==0){
                    desmontarPar(idPar);
                }else{
                    printf("AVISO: No se pudo desmontar la particion.\n");
                }
            break;
            }
            case 24:{ //exec

                avanzarToken();

                if(tokenActual==7){ // token = ::
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba \"-\" (path...).\n");
                    break;
                }

                if(tokenActual==12){ // token = ::
                    avanzarToken();
                }else{
                    avanzarToken();
                    printf("ERROR: se esperaba Phat despues de \"-\".\n");
                    break;
                }

                if(tokenActual==5){ // token = ::
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba \"::\" despues de path.\n");
                    break;
                }

                if(tokenActual==4){ //token = numero
                    strcpy(path,tokActual->lexema); // global
                    avanzarToken();
                }else{
                    error=1;
                    avanzarToken();
                    printf("ERROR: se esperaba una cadea despues de \"::\".\n");
                    break;
                }

                if(error==0){
                    leeArchivo(path);
                }else{
                     printf("AVISO: No se pudo leer el archivo.\n");
                }
            break;
            }
            case 3:{
                printf("Comentario: %s\n",tokActual->lexema);
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
    }else{
        printf("ERROR: No se reconocio ningun parametro.\n");
        error=1;
    }
}

void COM2(){
    if(tokActual!=NULL){
        PAR2();
    }else{
        printf("ERROR: No se reconocio ningun parametro.\n");
        error=1;
    }
}

void COM3(){
    if(tokActual!=NULL){
        PAR5();
    }else{
        imprimirMontaje();
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
                        printf("se esperaba una cadena despues de \"::\".\n");
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
                        printf("se esperaba un caracter despues de ::.\n");
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

void PAR2(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 6:{//+
                avanzarToken();
                PAR3();
            break;
            }
            case 7:{//-
                avanzarToken();
                PAR4();
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

void PAR3(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 11:{ //unit
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

                    if(tokenActual==2){ //token = id
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(unidad,tokActual->lexema);
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba un caracter despues de ::.\n");
                        break;
                    }
                    PAR2();
                }else{
                   printf("El parametro unit no se puede repetir.\n");
                   error=1;
                }
            break;
            }
            case 16:{ //type
                if(Btype<1){
                    Btype=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de type.\n");
                        break;
                    }
                    if(tokenActual==2){
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(type,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba un caracter  despues de \"::\".\n");
                        break;
                    }
                    PAR2();
                }else{// aca notifico error cuando path viene mas de una vez
                    printf("El parametro Type no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            case 17:{ //fit
                if(Bfit<1){
                    Bfit=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de fit.\n");
                        break;
                    }
                    if(tokenActual==2){
                        // validar fit en la llamada principal
                        strcpy(fit,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("Se esperaba un dupla de caracteres despues de \"::\".\n");
                        break;
                    }
                    PAR2();
                }else{
                    printf("El parametro fit no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            case 18:{ //delete
                if(Bdelete<1){
                    Bdelete=1;
                    avanzarToken();
                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de delete.\n");
                        break;
                    }
                    if(tokenActual==2){
                        //si todo va bien aca guardaria el tamano size en una global
                        strcpy(delVal,tokActual->lexema); // global
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("Se esperaba fast o full despues de \"::\".\n");
                        break;
                    }
                    PAR2();
                }else{
                    printf("El parametro delete no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            case 20:{ //add
                if(Badd<1){
                    Badd=1;
                    avanzarToken();

                    if(tokenActual==5){ // token = ::
                        avanzarToken();
                    }else{
                        error=1;
                        avanzarToken();
                        printf("se esperaba :: despues de path.\n");
                        break;
                    }

                    if(tokenActual==1 || tokenActual==7){
                        int nega=0;
                        if(tokenActual==7){
                            avanzarToken();
                            nega=1;
                        }

                        if(tokenActual==1){
                           if(nega==1){
                                add=atoi(tokActual->lexema); // global
                                add=add*(-1);
                                avanzarToken();
                            }else{
                                add=atoi(tokActual->lexema);
                                avanzarToken();
                            }
                        }else{
                            error=1;
                            avanzarToken();
                            printf("Se esperaba una numero despues de \"::\".\n");
                            break;
                        }

                    }else{
                        error=1;
                        avanzarToken();
                        printf("Se esperaba una numero despues de \"::\".\n");
                        break;
                    }
                    PAR2();
                }else{
                    printf("El parametro add no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            default:{
                printf("EL paramentro despues de \"+\" no es correcto.\n");
                error=1;
            break;
            }

        }
    }
}

void PAR4(){
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
                    PAR2();
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
                        printf("se esperaba una cadena despues de \"::\".\n");
                        break;
                    }
                    PAR2();
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
                    PAR2();
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

void PAR5(){
    if(tokActual!=NULL){
        switch (tokenActual) {
            case 7:{
                avanzarToken();
                PAR6();
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

void PAR6(){
    if(tokActual!=NULL){
        switch (tokenActual) {
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
                        printf("se esperaba una cadena despues de \"::\".\n");
                        break;
                    }
                    PAR5();
                }else{// aca notifico error cuando path viene mas de una vez
                    printf("El parametro phat no se puede repetir.\n");
                    error=1;
                }
            break;
            }
            case 13:{ //name
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
