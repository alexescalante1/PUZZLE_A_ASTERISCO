#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
using namespace std;

HINSTANCE hInst;
HBRUSH brushBlack;
HBRUSH brushWhite;
HPEN greenPen;
COLORREF actual;

string DIRECCIONES[4] = {"IZQUIERDA","DERECHA","ARRIBA","ABAJO"};
int  HDCRECTANGLE = 0;
int  AVANCEHDC = 0;
bool NODOMPLESEC = 0;
bool GAMEOVER = 0;
bool ONE = 1;

int MATRIZOBJETIVO[3][3]={{1,2,3},
                          {8,0,4},
                          {7,6,5}};

int DATO[3][3];

int CONVERTIR_INT_A_CAD(char **,long long);
bool RANGONUM(int **);
void INSERTAR_NUEVO_NODO(int[3][3],int,int,bool);
void INSERTAR_FINAL_NODO(int[3][3],int,int,bool);
BOOL CALLBACK DlgMain(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgACTUALIZAR(HWND, UINT, WPARAM, LPARAM);

int CONVERTIR_INT_A_CAD(char **R,long long NUM){int TAM=0,i=0;
    for(long long AUX=NUM;AUX!=0;i++,AUX/=10);
    *R = new char [i];TAM=i;*(*(R)+i)='\0';i-=1;
    for(;i>=0;i--,NUM/=10){
        *(*(R)+i)=(char)(NUM%10+48);
    }
    return TAM;
}

int CONCATENAR(string A,string B,char **R){int TAM=0,i=0;
    for(;A[TAM];TAM++);
    for(;B[i];i++,TAM++);TAM+=1;
    *R = new char [TAM];TAM=0;
    for(i=0;A[i];i++,TAM++){
        *(*(R)+TAM)=A[i];
    }
    for(i=0;B[i];i++,TAM++){
        *(*(R)+TAM)=B[i];
    }*(*(R)+TAM) = '\0';
    return TAM;
}

bool RANGONUM(int MTRIZ[3][3]){int CEROS=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(CEROS==2){return 1;}
            if(MTRIZ[i][j]==0){
                CEROS++;
            }else{
                if(!(MTRIZ[i][j]<10&&MTRIZ[i][j]>0)){
                    return 1;
                }
            }
        }
    }
    if(CEROS==1){
        return 0;
    }else{
        return 1;
    }
}

class NODO_AUX_ELECT{
    public:
    int MATRIZ[3][3],FUNCION,DIR,PROFUNDIDA;
    bool NODO_MULTIPLE;
    NODO_AUX_ELECT *SIG;
    NODO_AUX_ELECT(int D[3][3],int FUNCC,int DR,int FROF,bool NMPLE){
        for(int l=0;l<3;l++){
            for(int j=0;j<3;j++){
                MATRIZ[l][j]=D[l][j];
            }
        }
        FUNCION = FUNCC;
        DIR = DR;
        NODO_MULTIPLE = NMPLE;
        PROFUNDIDA = FROF;
        SIG = NULL;
    }
};

class SELECCION_MENOR{
    public:
    NODO_AUX_ELECT *RAIZ;
    SELECCION_MENOR(){
        RAIZ=NULL;
    }
    void INSERT_N(int N[3][3],int FUNCC,int DR,int FROF,bool NMPLE){
        INS(RAIZ,N,FUNCC,DR,FROF,NMPLE);
    }
    void INS(NODO_AUX_ELECT *&R,int N[3][3],int FUNCC,int DR,int FROF,bool NMPLE){
        if(R){
            INS(R->SIG,N,FUNCC,DR,FROF,NMPLE);
        }else{
            R = new NODO_AUX_ELECT(N,FUNCC,DR,FROF,NMPLE);
        }
    }
    NODO_AUX_ELECT *BUSCARMENOR(){
        return BUSCM(RAIZ);
    }
    NODO_AUX_ELECT *BUSCM(NODO_AUX_ELECT *R){int MENOR=999;NODO_AUX_ELECT *B=R;
        while(R){
            if(R->FUNCION<=MENOR){
                if(R->FUNCION==MENOR){
                    B->NODO_MULTIPLE=1;
                    R->NODO_MULTIPLE=1;
                }else{
                    if(B->NODO_MULTIPLE){
                        LIMPIAR_NMPLE(RAIZ);
                    }
                    MENOR = R->FUNCION;
                    B=R;
                }
            }
            R=R->SIG;
        }
        if(VAL_NODO_MPLE(RAIZ)){
            NODOMPLESEC = 1;
            return RAIZ;
        }else{
            return B;
        }

    }
    void LIMPIAR_NMPLE(NODO_AUX_ELECT *R){ //LIMPIA EL VALOR MULTIPLE
        while(R){
            R->NODO_MULTIPLE=0;
            R=R->SIG;
        }
    }
    bool VAL_NODO_MPLE(NODO_AUX_ELECT *R){ //VALIDA SI EXISTE VALOR MULTIPLE
        while(R){
            if(R->NODO_MULTIPLE){
                return 1;
            }
            R=R->SIG;
        }
        return 0;
    }
    void ELIMINARDATO(int DAT){ //ELIMINA TODA LA LISTA
        ELIMDATO(RAIZ,DAT);
    }
    void ELIMDATO(NODO_AUX_ELECT *&R,int DAT){
        while(R){
            NODO_AUX_ELECT *AUX = R;
            NODO_AUX_ELECT *ANTERIOR = NULL;

            while(AUX&&AUX->PROFUNDIDA!=DAT){
                ANTERIOR = AUX;
                AUX = AUX->SIG;
            }
            if(AUX==NULL){
                cout<<"No existe";
            }else if(ANTERIOR==NULL){
                R=R->SIG;
                delete AUX;
            }else{
                ANTERIOR->SIG = AUX->SIG;
                delete AUX;
            }
        }
    }
    void ELIMINARTODO(){ //ELIMINA TODA LA LISTA
        ELIMTODO(RAIZ);
    }
    void ELIMTODO(NODO_AUX_ELECT *&R){
        while(R){
            NODO_AUX_ELECT *AUX = R;
            R=AUX->SIG;
            delete AUX;
        }
    }
    void MOSTRAR(){
        MOS(RAIZ);
    }
    void MOS(NODO_AUX_ELECT *R){
        while(R){
            cout<<R->PROFUNDIDA<<"<-  ";
            R=R->SIG;
        }
    }

}AUX_N;

class NODO{
    public:
    int MATRIZ[3][3];
    int PROFUNDIDAD,CORRECTOS,ORIGEN;
    bool NODO_MULTIPLE;
    NODO *SIG;
    NODO(int D[3][3],int ORIGENN,int PROFUND,bool NMPLE){
        for(int l=0;l<3;l++){
            for(int j=0;j<3;j++){
                MATRIZ[l][j]=D[l][j];
            }
        }
        ORIGEN = ORIGENN;
        PROFUNDIDAD = PROFUND;
        NODO_MULTIPLE = NMPLE;
        CORRECTOS = 0;
        SIG = NULL;
    }
};

class OCHOPUZZLE{
    public:
    NODO *RAIZ,*PDRE;
    OCHOPUZZLE(){
        RAIZ=NULL;
        PDRE=NULL;
    }
    void INSERT_N(int N[3][3],int O,int PROF,bool NMP){
        INS(RAIZ,N,O,PROF,NMP);
    }
    void INS(NODO *&R,int N[3][3],int O,int PROF,bool NMP){
        if(R){
            INS(R->SIG,N,O,PROF,NMP);
        }else{
            R = new NODO(N,O,PROF,NMP);
        }
    }
    void EJECUTAR(){
        EJC(RAIZ);
    }
    void EJC(NODO *R){
        while(R){
            cout<<"#  PROFUNDIDAD: "<<R->PROFUNDIDAD<<"\n#  MATRIZ INICIAL:\n#  MULTIPLE: "<<R->NODO_MULTIPLE<<"\n#  ";
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    cout<<R->MATRIZ[i][j];
                }cout<<"\n#  ";
            }cout<<"\n\n";
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(R->MATRIZ[i][j]==0){ //BUSCA EL 0
                        MOVIMIENTOS(R,i,j); //CALCULA POSIVILIDADES
                    }
                }
            }

            if(R->NODO_MULTIPLE){///REVISAR!!
                if(!R->SIG){
//                    cout<<"\n\n\n\n\n\n\n";
                    NODO_AUX_ELECT *MENOR;
                    MENOR = AUX_N.BUSCARMENOR();
                    if(NODOMPLESEC){
                        while(MENOR){
                            if(MENOR->NODO_MULTIPLE){ //NUEVO NODO MULTIPLE
//                                cout<<MENOR->PROFUNDIDA;
                                INSERTAR_NUEVO_NODO(MENOR->MATRIZ,MENOR->DIR,R->PROFUNDIDAD+1,1);
                            }
                            MENOR=MENOR->SIG;
                        }
                        NODOMPLESEC = 0;
                    }else{ //INSERTA NODO RESULTADO
                        INSERTAR_NUEVO_NODO(MENOR->MATRIZ,MENOR->DIR,R->PROFUNDIDAD+1,MENOR->NODO_MULTIPLE);
                    }
                    AUX_N.MOSTRAR();
                    AUX_N.ELIMINARTODO();
//                    cout<<"\n\n\n\n\n\n\n";
                }
            }

            cout<<"\n===========================================\n\n";
            R=R->SIG;
        }
    }
    int MOVIMIENTOS(NODO *R,int i,int j){int OPCIONES[2][4]={{0,0,0,0},{0,0,0,0}};int MENOR[2]={99,0};
        if(R->ORIGEN!=1){
            if(LIMITE(i,j-1)){cout<<"\nAVANZA A IZQUIERDA:";OPCIONES[0][1]=EVALUACION(R,i,j,1,R->PROFUNDIDAD+1);}
        }
        if(R->ORIGEN!=2){
            if(LIMITE(i,j+1)){cout<<"\nAVANZA A DERECHA:";OPCIONES[0][0]=EVALUACION(R,i,j,0,R->PROFUNDIDAD+1);}
        }
        if(R->ORIGEN!=3){
            if(LIMITE(i-1,j)){cout<<"\nAVANZA A ARRIBA:";OPCIONES[0][3]=EVALUACION(R,i,j,3,R->PROFUNDIDAD+1);}
        }
        if(R->ORIGEN!=4){
            if(LIMITE(i+1,j)){cout<<"\nAVANZA A ABAJO:";OPCIONES[0][2]=EVALUACION(R,i,j,2,R->PROFUNDIDAD+1);}
        }

        cout<<"\nRETORNO DE F: ";
        for(int m=0;m<4;m++){ cout<<OPCIONES[0][m]<<" ";}

        for(int k=0;k<4;k++){ //VALIDA DATOS MENORES IGUALES
            if(OPCIONES[0][k]){
                if(OPCIONES[0][k]<=MENOR[0]){
                    if(OPCIONES[0][k]==MENOR[0]){
                        MENOR[0]=OPCIONES[0][k];
                        OPCIONES[1][MENOR[1]]=1;
                        OPCIONES[1][k]=1;
                        MENOR[1]=k;
                    }else{
                        MENOR[0]=OPCIONES[0][k];
                        MENOR[1]=k;
                        for(int l=0;l<4;l++){OPCIONES[1][l]=0;}
                    }
                }
            }

        }
        cout<<"\n\nF MENOR: "<<MENOR[0]<<"\nPADRE DIR: "<<DIRECCIONES[MENOR[1]]<<"\n"; //MENOR +1 PARA VALIDACION DE PADRE

        bool VALIDA_F=1;
        for(int l=0;l<4;l++){
            if(OPCIONES[1][l]){VALIDA_F=0;
                cout<<"\n- EXISTE F IGUALES!\n- PADRE DIR: "<<DIRECCIONES[l]<<"\n";
                ASIGNACION_NUEVO_NODO(R,i,j,l,MENOR[0],R->PROFUNDIDAD+1,1);
            }
        }
        if(VALIDA_F){
            ASIGNACION_NUEVO_NODO(R,i,j,MENOR[1],MENOR[0],R->PROFUNDIDAD+1,0);
        }

        return 0;
    }

    bool LIMITE(int i,int j){ //VERIFICA LIMITES DE LA MATRIZ
        if(i>=0&&j>=0){
            if(j<3&&i<3){
                return 1;
            }else{
                return 0;
            }
        }else{
            return 0;
        }
    }

    int EVALUACION(NODO *R,int i,int j,int DIRR,int G){
        int **AUXM = new int*[3];
        for(int k=0;k<3;k++){*(AUXM+k) = new int[3];}
        for(int l=0;l<3;l++){
            for(int j=0;j<3;j++){
                AUXM[l][j]=R->MATRIZ[l][j];
            }
        }
        switch(DIRR){
            case 0:
                AUXM[i][j]=AUXM[i][j+1];
                AUXM[i][j+1]=0;
            break;
            case 1:
                AUXM[i][j]=AUXM[i][j-1];
                AUXM[i][j-1]=0;
            break;
            case 2:
                AUXM[i][j]=AUXM[i+1][j];
                AUXM[i+1][j]=0;
            break;
            case 3:
                AUXM[i][j]=AUXM[i-1][j];
                AUXM[i-1][j]=0;
            break;
        }
        cout<<"\nEVALUACION: "<<AUXM[i][j]<<"\nREEMPLAZANDO:\n";
        int H=0; //INCORRECTAS
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(MATRIZOBJETIVO[i][j]){
                    if(MATRIZOBJETIVO[i][j]!=AUXM[i][j]){
                        H++;
                    }
                }
                cout<<AUXM[i][j];
            }cout<<"\n";
        }
        if(H==0){
            GAMEOVER = 1;
        }
        return H+G;
    }

    void ASIGNACION_NUEVO_NODO(NODO *R,int i,int j,int DIRR,int F,int G,bool NMPLE){
        int AUXM[3][3];
        for(int l=0;l<3;l++){
            for(int j=0;j<3;j++){
                AUXM[l][j]=R->MATRIZ[l][j];
            }
        }
        switch(DIRR){
            case 0:
                AUXM[i][j]=AUXM[i][j+1];
                AUXM[i][j+1]=0;
            break;
            case 1:
                AUXM[i][j]=AUXM[i][j-1];
                AUXM[i][j-1]=0;
            break;
            case 2:
                AUXM[i][j]=AUXM[i+1][j];
                AUXM[i+1][j]=0;
            break;
            case 3:
                AUXM[i][j]=AUXM[i-1][j];
                AUXM[i-1][j]=0;
            break;
        }

        if(!R->NODO_MULTIPLE){
            INSERTAR_NUEVO_NODO(AUXM,DIRR+1,G,NMPLE);
        }else{
            AUX_N.INSERT_N(AUXM,F,DIRR+1,G-1,0);
        }
        cout<<"\nNUEVO NODO F: "<<F<<", G: "<<G<<", PADRE DIR: "<<DIRECCIONES[DIRR]<<"\n";
    }
    void APILARALFINAL(){
        APILAR(RAIZ);
    }
    void APILAR(NODO *R){NODO *B=R;
        while(R){
            B = R;
            R = R->SIG;
        }
        INSERTAR_FINAL_NODO(MATRIZOBJETIVO,0,B->PROFUNDIDAD+1,0);
    }
    void MOSTRAR(HDC hdc,int NU){
        MOS(RAIZ,hdc,NU,40,200);
    }
    void MOS(NODO *R,HDC hdc,int NU,int Xini,int Yini){bool V=1;
        brushBlack=CreateSolidBrush(RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc,brushBlack);
        while(R){
            if(R->PROFUNDIDAD==NU){
                actual = SetTextColor(hdc, RGB(255,0,0));
                if(!R->PROFUNDIDAD){
                    TextOut(hdc,Xini+18,Yini-35,"g = 0",5);
                }else{
                    char *CAD1,*PROFUNDIDAD;
                    CONVERTIR_INT_A_CAD(&PROFUNDIDAD,R->PROFUNDIDAD);
                    int T = CONCATENAR("g = ",PROFUNDIDAD,&CAD1);
                    TextOut(hdc,Xini+18,Yini-35,CAD1,T);
                }
                Rectangle(hdc,Xini-15,Yini-10,Xini+85,Yini+85);
                actual = SetTextColor(hdc, RGB(255,255,255));
                for(int i=Xini-15;i<Xini+80;i+=33){
                    MoveToEx(hdc,i,Yini-10,NULL);
                    LineTo(hdc,i,Yini+84);
                }
                for(int i=Yini-10;i<Yini+80;i+=33){
                    MoveToEx(hdc,Xini-15,i,NULL);
                    LineTo(hdc,Xini+84,i);
                }
                for(int i=0,YAux=Yini;i<3;i++,YAux+=30){
                    for(int j=0,XAux=Xini;j<3;j++,XAux+=30){
                        char *CAD2;
                        int T = CONVERTIR_INT_A_CAD(&CAD2,R->MATRIZ[i][j]);
                        TextOut(hdc,XAux,YAux,CAD2,T);
                    }
                }Xini+=150;V=0;
                SetTextColor(hdc, actual);
            }
            R=R->SIG;
        }if(V){AVANCEHDC=0;}
        DeleteObject(brushBlack);
    }
    void ELIMINARTODO(){ //ELIMINA TODA LA LISTA
        ELIMTODO(RAIZ);
    }
    void ELIMTODO(NODO *&R){
        while(R){
            NODO *AUX = R;
            R=AUX->SIG;
            delete AUX;
        }
    }
}A;

void INSERTAR_NUEVO_NODO(int M[3][3],int PDRE,int PROFUND,bool NMPLE){
    if(!GAMEOVER){
        A.INSERT_N(M,PDRE,PROFUND,NMPLE);
    }
}
void INSERTAR_FINAL_NODO(int M[3][3],int PDRE,int PROFUND,bool NMPLE){
    A.INSERT_N(M,PDRE,PROFUND,NMPLE);
}

void INICIALIZADO(){

    DATO[0][0]=2;DATO[0][1]=8;DATO[0][2]=3;
    DATO[1][0]=1;DATO[1][1]=6;DATO[1][2]=4;
    DATO[2][0]=7;DATO[2][1]=0;DATO[2][2]=5;

    A.INSERT_N(DATO,0,0,0);
    A.EJECUTAR();
    A.APILARALFINAL();

}

void VISUAL_DESING(HDC hdc){
        brushBlack=CreateSolidBrush(RGB(0, 0, 0));
        brushWhite=CreateSolidBrush(RGB(255, 255, 255));
        greenPen=CreatePen(PS_SOLID, 2, RGB(51, 204, 255));

        SelectObject(hdc,brushBlack);
        Rectangle(hdc,-5,-5,1400,800);

        SelectObject(hdc,greenPen);
        SelectObject(hdc,brushWhite);
        switch(HDCRECTANGLE){
            case 0:
                Rectangle(hdc,5,8,727,62);
                Rectangle(hdc,5,380,727,434);
                Rectangle(hdc,5,70,727,370);
                A.MOSTRAR(hdc,AVANCEHDC);
                break;
            case 1:
                Rectangle(hdc,5,10,245,360);
                break;
        }
        DeleteObject(brushWhite);
        DeleteObject(greenPen);
}

BOOL CALLBACK DlgACTUALIZAR(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_PAINT:
    {       HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);

            VISUAL_DESING(hdc);

            EndPaint(hwndDlg,&ps);
    }
    case WM_INITDIALOG:
    {
        if(ONE){ONE=0;
            char *CAD;
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[0][0]);
            SetDlgItemText(hwndDlg,BTN00,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[0][1]);
            SetDlgItemText(hwndDlg,BTN01,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[0][2]);
            SetDlgItemText(hwndDlg,BTN02,CAD);

            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[1][0]);
            SetDlgItemText(hwndDlg,BTN10,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[1][1]);
            SetDlgItemText(hwndDlg,BTN11,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[1][2]);
            SetDlgItemText(hwndDlg,BTN12,CAD);

            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[2][0]);
            SetDlgItemText(hwndDlg,BTN20,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[2][1]);
            SetDlgItemText(hwndDlg,BTN21,CAD);
            CONVERTIR_INT_A_CAD(&CAD,MATRIZOBJETIVO[2][2]);
            SetDlgItemText(hwndDlg,BTN22,CAD);
        }
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            char C1[5],C2[5],C3[5],C4[5],C5[5],C6[5],C7[5],C8[5],C9[5];
            int  N1;

            case BTN_ACTUALIZAR:
                GetDlgItemText(hwndDlg,BTN00,C1,5);
                GetDlgItemText(hwndDlg,BTN01,C2,5);
                GetDlgItemText(hwndDlg,BTN02,C3,5);

                GetDlgItemText(hwndDlg,BTN10,C4,5);
                GetDlgItemText(hwndDlg,BTN11,C5,5);
                GetDlgItemText(hwndDlg,BTN12,C6,5);

                GetDlgItemText(hwndDlg,BTN20,C7,5);
                GetDlgItemText(hwndDlg,BTN21,C8,5);
                GetDlgItemText(hwndDlg,BTN22,C9,5);

                N1 = atoi(C1);
                MATRIZOBJETIVO[0][0]=N1;
                N1 = atoi(C2);
                MATRIZOBJETIVO[0][1]=N1;
                N1 = atoi(C3);
                MATRIZOBJETIVO[0][2]=N1;

                N1 = atoi(C4);
                MATRIZOBJETIVO[1][0]=N1;
                N1 = atoi(C5);
                MATRIZOBJETIVO[1][1]=N1;
                N1 = atoi(C6);
                MATRIZOBJETIVO[1][2]=N1;

                N1 = atoi(C7);
                MATRIZOBJETIVO[2][0]=N1;
                N1 = atoi(C8);
                MATRIZOBJETIVO[2][1]=N1;
                N1 = atoi(C9);
                MATRIZOBJETIVO[2][2]=N1;

                if(!RANGONUM(MATRIZOBJETIVO)){
                    EndDialog(hwndDlg, 0);HDCRECTANGLE = 0;
                    DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
                }else{
                    SetDlgItemText(hwndDlg,EDITESTADO,"0<D<10 && !0");
                }

            return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_PAINT:
    {       HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);

            VISUAL_DESING(hdc);

            EndPaint(hwndDlg,&ps);
    }
    case WM_INITDIALOG:
    {
        if(ONE){ONE=0;
            INICIALIZADO();
        }
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            char C1[5],C2[5],C3[5],C4[5],C5[5],C6[5],C7[5],C8[5],C9[5];
            int  N1;

            case BTN_INGRESAR_MATRIZ:
                GetDlgItemText(hwndDlg,BTN00,C1,5);
                GetDlgItemText(hwndDlg,BTN01,C2,5);
                GetDlgItemText(hwndDlg,BTN02,C3,5);

                GetDlgItemText(hwndDlg,BTN10,C4,5);
                GetDlgItemText(hwndDlg,BTN11,C5,5);
                GetDlgItemText(hwndDlg,BTN12,C6,5);

                GetDlgItemText(hwndDlg,BTN20,C7,5);
                GetDlgItemText(hwndDlg,BTN21,C8,5);
                GetDlgItemText(hwndDlg,BTN22,C9,5);

                N1 = atoi(C1);
                DATO[0][0]=N1;
                N1 = atoi(C2);
                DATO[0][1]=N1;
                N1 = atoi(C3);
                DATO[0][2]=N1;

                N1 = atoi(C4);
                DATO[1][0]=N1;
                N1 = atoi(C5);
                DATO[1][1]=N1;
                N1 = atoi(C6);
                DATO[1][2]=N1;

                N1 = atoi(C7);
                DATO[2][0]=N1;
                N1 = atoi(C8);
                DATO[2][1]=N1;
                N1 = atoi(C9);
                DATO[2][2]=N1;

                if(!RANGONUM(DATO)){ //DATO>0&&DATO<10 Y VALIDA VACIO
                    system("cls");
                    GAMEOVER = 0;
                    AVANCEHDC = 0;
                    A.ELIMINARTODO();
                    AUX_N.ELIMINARTODO();
                    A.INSERT_N(DATO,0,0,0);
                    A.EJECUTAR();
                    A.APILARALFINAL();
                    EndDialog(hwndDlg, 0);HDCRECTANGLE = 0;
                    DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);

                }else{
                    SetDlgItemText(hwndDlg,EDITESTADO,"0<D<10 && !0");
                }

            return TRUE;

            case BTN_SIG:
                AVANCEHDC++;
                InvalidateRect(hwndDlg,NULL,true);
            return TRUE;

            case BTN_INGRESAR:
                EndDialog(hwndDlg, 0);HDCRECTANGLE = 1;
                DialogBox(hInst, MAKEINTRESOURCE(INGRESAR_NUEVA_MATRIZ), NULL, (DLGPROC)DlgMain);
            return TRUE;

            case BTN_ACTUALIZAR_M:
                ONE = 1;
                EndDialog(hwndDlg, 0);HDCRECTANGLE = 1;
                DialogBox(hInst, MAKEINTRESOURCE(ACTUALIZARMATRIZ), NULL, (DLGPROC)DlgACTUALIZAR);
            return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
