#include <stdlib.h>
#include <stdio.h>
#include <math.h>

class Matriz
{
public:
    Matriz(int c, int f);
    ~Matriz();
    
    int columnas;
    int filas;
    float **matriz;

    static int totalMult;
    static Matriz * multiplicacionDinamica (Matriz ** matrices, int numMatrices);
    static Matriz * resolver (Matriz ** matrices, int **tableDiv,int i, int j);
    static Matriz * strassen(Matriz *m1, Matriz *m2);
    static Matriz * dividirMatriz(Matriz * m1, int parte);
    static Matriz * unirMatrices(Matriz * m0, Matriz * m1, Matriz * m2, Matriz * m3);
    static Matriz * hacerCuadrada(Matriz *m1);

    Matriz * operator*(Matriz m2);

    Matriz * operator-(Matriz m2);

    Matriz * operator+(Matriz m2);

    void imprimir();

};

Matriz::Matriz(int f, int c){
    this->columnas = c;
    this->filas = f;

    this->matriz = (float **) malloc(sizeof(float *) * f);

    for (int i = 0; i < f; i++)
    {
        this->matriz[i] = (float *) calloc(c, sizeof(float)); 
    }
    
}

Matriz::~Matriz(){
}

void Matriz::imprimir(){
    printf("\n");
    for (int i = 0; i < this->filas; i++)
    {
        for (int j = 0; j < this->columnas; j++)
        {
            printf("%.2f\t", this->matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

Matriz * Matriz::multiplicacionDinamica(Matriz ** matrices, int numMatrices){

    bool verificado = true;

    for (int i = 0; i < numMatrices-1; i++)
    {

        if (matrices[i]->columnas != matrices[i+1]->filas)
        {
            verificado = false;
            break;
        }
        
    }
    
    if (!verificado)
    {
        return NULL;
    }

    int **tableDiv = (int **) malloc(sizeof(int *)*numMatrices);
    for (int i = 0; i < numMatrices; i++)
    {
        tableDiv[i] = (int *) calloc(numMatrices,sizeof(int));
    }
    
    int tableMult[numMatrices][numMatrices];

    int tableDivMax[numMatrices][numMatrices];

    int tableMultMax[numMatrices][numMatrices];

    //Poner tables a 0
    for (int i = 0; i < numMatrices; i++)
    {
        for (int j = 0; j < numMatrices; j++)
        {
            tableDiv[i][j]=0;
            tableMult[i][j]=0;

            tableDivMax[i][j]=0;
            tableMultMax[i][j]=0;
        }
    }

    int numMultiplicaciones, numMultiplicacionesMin, numMultiplicacionesMax, kMin, kMax;

    //Llenar table con minimos y maximos
    for (int j = 1; j < numMatrices; j++)
    {
        for (int i = 0; i+j <= numMatrices-1; i++)
        {
            printf("[%d,%d] -----------------------------\n",i,i+j);
            
            for (int k = i; k < i+j; k++)
            {
                printf("\tm[%d,%d] + m[%d,%d] + CA%d*FA%d*FA%d => %d + %d + %d*%d*%d \n",i,k,k+1,i+j,i,k,i+j,tableMult[i][k],tableMult[k+1][i+j],matrices[i]->filas,matrices[k]->columnas,matrices[i+j]->columnas);
                numMultiplicaciones = 
                    tableMult[i][k] 
                    + tableMult[k+1][i+j] 
                    + (matrices[i]->filas * matrices[k]->columnas * matrices[i+j]->columnas);
                
                if (k==i)
                {
                    numMultiplicacionesMin = numMultiplicaciones;
                    numMultiplicacionesMax = numMultiplicaciones;
                    kMin = k;
                    kMax = k;
                }else if (numMultiplicaciones < numMultiplicacionesMin)
                {
                    numMultiplicacionesMin = numMultiplicaciones;
                    kMin = k;
                }else if(numMultiplicaciones > numMultiplicacionesMax){
                    numMultiplicacionesMax = numMultiplicaciones;
                    kMax = k;
                }
                
            }

            printf("[%d,%d] => K=%d , mult=%d ------------------------------\n\n",i,i+j,kMin,numMultiplicacionesMin);
            
            tableDiv[i][i+j] = kMin;
            tableMult[i][i+j] = numMultiplicacionesMin;

            tableDivMax[i][i+j] = kMax;
            tableMultMax[i][i+j] = numMultiplicacionesMax;

        }
        
    }

    //Imprimir tables
    printf("---------------------Caso más óptimo------------------------\n");
    printf("------Subdivisiones-----\n");
    for (int i = 0; i < numMatrices; i++)
    {
        for (int j = 0; j < numMatrices; j++)
        {

            printf("\t%d",tableDiv[i][j]);

        }

        printf("\n");
        
    }
    printf("\n-------- Costos ---------\n");
    for (int i = 0; i < numMatrices; i++)
    {
        for (int j = 0; j < numMatrices; j++)
        {

            printf("\t%d",tableMult[i][j]);

        }

        printf("\n");
        
    }
    printf("\n--------------------- Peor caso ------------------------\n");
    printf("------Subdivisiones-----\n");
    for (int i = 0; i < numMatrices; i++)
    {
        for (int j = 0; j < numMatrices; j++)
        {

            printf("\t%d",tableDivMax[i][j]);

        }

        printf("\n");
        
    }
    printf("\n-------- Costos ---------\n");
    for (int i = 0; i < numMatrices; i++)
    {
        for (int j = 0; j < numMatrices; j++)
        {

            printf("\t%d",tableMultMax[i][j]);

        }

        printf("\n");
        
    }

    return Matriz::resolver(matrices,tableDiv,0,numMatrices-1);
       
}

Matriz * Matriz::resolver(Matriz ** matrices, int **tableDiv,int i, int j){

    if(i == j){
        return matrices[i];
    }

    Matriz *m1 = Matriz::resolver(matrices,tableDiv,i,tableDiv[i][j]);
    Matriz *m2 = Matriz::resolver(matrices,tableDiv,tableDiv[i][j]+1,j);
    printf("\n------ Multiplicacion de A[%d,%d] * A[%d,%d] ------\n\n",i,tableDiv[i][j],tableDiv[i][j]+1,j);
    Matriz *nueva = *m1 * *m2;
    return nueva;
}

Matriz * Matriz::operator*(Matriz m2)
{
    //printf("m1 Col:%d\n",columnas);
    //printf("m2 Fil:%d\n",m2.filas);
    
    if((int)columnas != (int)m2.filas){
        printf("Error\n");
        return new Matriz(0,0);
    }
    printf("\n");

    Matriz *newMatriz = new Matriz(filas, m2.columnas);
    
    float sum=0;
    
    for (int i = 0; i < filas; i++)
    {
        for (int k = 0; k < m2.columnas; k++)
        {
            for (int j = 0; j < m2.filas; j++)
            {
                //printf("[%d,%d] * [%d,%d] = %.2f * %.2f = %.2f\n",i,j,j,k,matriz[i][j], m2.matriz[j][k],matriz[i][j] * m2.matriz[j][k]);
                sum += matriz[i][j] * m2.matriz[j][k];
            }
            
            newMatriz->matriz[i][k] = sum; 
        }
        sum=0;
    }

    printf("\n");
    
    return newMatriz;
}

Matriz * Matriz::operator-(Matriz m2)
{
    //printf("m1 Col:%d\n",columnas);
    //printf("m2 Fil:%d\n",m2.filas);
    
    if((int)columnas != (int)m2.columnas || filas != m2.filas){
        printf("Error\n");
        return new Matriz(0,0);
    }

    Matriz *newMatriz = new Matriz(filas, m2.columnas);
    
    float sum=0;
    
    for (int i = 0; i < filas; i++)
    {
        for (int k = 0; k < columnas; k++)
        {
            newMatriz->matriz[i][k]= matriz[i][k] - m2.matriz[i][k]; 
        }        
    }

    //printf("Resta\n");
    
    return newMatriz;
}

Matriz * Matriz::operator+(Matriz m2)
{
    //printf("m1 Col:%d\n",columnas);
    //printf("m2 Fil:%d\n",m2.filas);
    
    if((int)columnas != (int)m2.columnas || filas != m2.filas){
        printf("Error\n");
        return new Matriz(0,0);
    }

    Matriz *newMatriz = new Matriz(filas, m2.columnas);
    
    float sum=0;
    
    for (int i = 0; i < filas; i++)
    {
        for (int k = 0; k < columnas; k++)
        {
            newMatriz->matriz[i][k] = matriz[i][k] + m2.matriz[i][k]; 
        }        
    }

    //printf("Suma\n");

    return newMatriz;
}

Matriz * Matriz::strassen(Matriz *m1, Matriz *m2){

    if(m1->filas == 1){
        Matriz * nueva = new Matriz(1,1);
        nueva->matriz[0][0]=m1->matriz[0][0]*m2->matriz[0][0];
        return nueva;
    }

    if(!( m1->filas == m1->columnas && (m1->filas != 0 && (m1->filas & (m1->filas-1)) == 0) ) && !( m2->filas == m2->columnas && (m2->filas != 0 && (m2->filas & (m2->filas-1)) == 0) )){
        return NULL;
    }

    Matriz *a = Matriz::dividirMatriz(m1,0);
    Matriz *b = Matriz::dividirMatriz(m1,1);
    Matriz *c = Matriz::dividirMatriz(m1,2);
    Matriz *d = Matriz::dividirMatriz(m1,3);

    Matriz *e = Matriz::dividirMatriz(m2,0);
    Matriz *f = Matriz::dividirMatriz(m2,1);
    Matriz *g = Matriz::dividirMatriz(m2,2);
    Matriz *h = Matriz::dividirMatriz(m2,3);

    Matriz *p1 = Matriz::strassen(a,*f-*h);
    Matriz *p2 = Matriz::strassen(*a+*b,h);
    Matriz *p3 = Matriz::strassen(*c+*d,e);
    Matriz *p4 = Matriz::strassen(d,*g-*e);
    Matriz *p5 = Matriz::strassen(*a+*d,*e+*h);
    Matriz *p6 = Matriz::strassen(*b-*d,*g+*h);
    Matriz *p7 = Matriz::strassen(*a-*c,*e+*f);

    // (*(*(*p5+*p4)-*p2)+*p6)->imprimir();
    // (*p1+*p2)->imprimir();
    // (*p3+*p4)->imprimir();
    // (*(*(*p1+*p5)-*p3)-*p7)->imprimir();
    return Matriz::unirMatrices((*(*(*p5+*p4)-*p2)+*p6),(*p1+*p2),(*p3+*p4),(*(*(*p1+*p5)-*p3)-*p7));
}

Matriz * Matriz::dividirMatriz(Matriz * m1, int parte){
    //printf("\nIniciamos dividir\n");
    Matriz * nueva = new Matriz(m1->filas/2,m1->columnas/2);

    int inicioFila = (int)(parte/2) * (m1->filas/2);

    int finFila = inicioFila + (m1->filas/2);

    int inicioColumna = (int)(parte%2) * (m1->columnas/2);

    int finColumna = inicioColumna + (m1->columnas/2);

    for (int i = inicioFila ; i < finFila; i++)
    {
        for (int j = inicioColumna; j < finColumna; j++)
        {
            //printf("Nueva i:%d j:%d -> Vieja\n",i-inicioFila,j-inicioColumna);
            nueva->matriz[i-inicioFila][j-inicioColumna]=m1->matriz[i][j];
        }
        
    }
    
    return nueva;
}

Matriz * Matriz::unirMatrices(Matriz * m0, Matriz * m1, Matriz * m2, Matriz * m3){
    //printf("\niniciamos union\n");

    Matriz * nueva = new Matriz(m0->filas+m2->filas,m0->columnas+m1->columnas);
    
    for (int i = 0; i < m0->filas; i++)
    {
        for (int j = 0; j < m0->columnas; j++)
        {
            nueva->matriz[i][j] = m0->matriz[i][j];
        }
        
    }
    //printf("\nm0\n");
    for (int i = 0; i < m1->filas; i++)
    {
        for (int j = 0; j < m1->columnas; j++)
        {
            nueva->matriz[i][j + m0->columnas] = m1->matriz[i][j];
        }
        
    }
    //printf("\nm1\n");
    for (int i = 0; i < m2->filas; i++)
    {
        for (int j = 0; j < m2->columnas; j++)
        {
            nueva->matriz[i + m0->filas][j] = m2->matriz[i][j];
        }
        
    }
    //printf("\nm2\n");
    for (int i = 0; i < m3->filas; i++)
    {
        for (int j = 0; j < m3->columnas; j++)
        {
            nueva->matriz[i + m0->filas][j + m0->columnas] = m3->matriz[i][j];
        }
        
    }
    //printf("\nm3\n");

    //printf("\nUnir Matrices\n");
    
    return nueva;
}


Matriz * Matriz::hacerCuadrada(Matriz *m1){
    if (m1->columnas == m1->filas && (m1->filas != 0 && (m1->filas & (m1->filas-1)) == 0))
    {
        return m1;
    }

    int nuevoTamano = pow(2,(int)ceil(log2(m1->filas < m1->columnas ? m1->columnas : m1->filas)));

    Matriz * nueva = new Matriz(nuevoTamano,nuevoTamano);
    
    for (int i = 0; i < nuevoTamano; i++)
    {
        for (int j = 0; j < nuevoTamano; j++)
        {
            if(i < m1->filas && j < m1->columnas){
                nueva->matriz[i][j] =  m1->matriz[i][j];
            }else
            {
                nueva->matriz[i][j] = 0;
            }
            
        }
        
    }
    
    return nueva;
}

