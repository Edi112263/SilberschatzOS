 /* Validare sudoku folosind multithreading. 
  * Pp ca in input se gasesc doar numere de la 1 la 9.
  */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define dim 9

struct zona
{
    int idx;
    int x;
    int y;
};

int v[dim][dim]; // tabla
int ok[11]; // ok pt fiecare thread

void citire_tabla()
{
    FILE *f;
    f = fopen("input.txt", "r");
    
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            fscanf(f, "%d", &v[i][j]); // citire tabla
            
    fclose(f);
}

void afisare_tabla()
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
            printf("%d ", v[i][j]);
        
        printf("\n");
    }
}

void* verif_col(void* p)
{
    int idx = *(int*)p; // index-ul threadului in vectorul ok
    int frecv[9] = {0}; // vector de frecventa
    
    ok[idx] = 1;
    for (int j = 0; j < dim && ok[idx] == 1; j++)
    {
        for (int i = 0; i < dim && ok[idx] == 1; i++)
        {
            if (frecv[v[i][j]] != j) // fiecare nr trb sa apara exact o data pt fiecare coloana
                ok[idx] = 0;
            else
                frecv[v[i][j]]++;
        }
    }
    
    free(p); // elibereaza zona alocata parametrului
    pthread_exit(0);
}

void* verif_lin(void* p)
{
    int idx = *(int*)p; // index-ul threadului in vectorul ok
    int frecv[9] = {0}; // vector de frecventa
    
    ok[idx] = 1;
    for (int i = 0; i < dim && ok[idx] == 1; i++)
    {
        for (int j = 0; j < dim && ok[idx] == 1; j++)
        {
            if (frecv[v[i][j]] != i) // fiecare nr trb sa apara exact o data pt fiecare linie
                ok[idx] = 0;
            else
                frecv[v[i][j]]++;
        }
    }
    
    free(p); // elibereaza zona alocata parametrului
    pthread_exit(0);
}

void* verif_zona(void* p)
{
    /* Verifica daca o zona de 3x3 contine toate nr de la 1 la 9 */
    
    struct zona z = *(struct zona*)p;
    int idx = z.idx; // index-ul threadului in vectorul ok
    int car[9] = {0}; // vector caracteristic
    
    ok[idx] = 1;
    for (int i = z.x; i < z.x + 2 && ok[idx] == 1; i++)
    {
        for (int j = z.y; j < z.y + 2 && ok[idx] == 1; j++)
        {
            if (car[v[i][j]] != 0)
                ok[idx] = 0;
            else
                car[v[i][j]] = 1;
        }
    }
     
    free(p); // elibereaza zona alocata parametrilor
    pthread_exit(0);
}

int main()
{
    pthread_t workers[11]; // threadurile
    int i, j, idx = 0;
    int *p;
    struct zona *zp;
    
    citire_tabla();
    //afisare_tabla();
    
    p = (int*) malloc(sizeof(int));
    *p = idx;
    pthread_create(&workers[idx], NULL, verif_col, p); // verificam coloanele
    idx++;
    
    p = (int*) malloc(sizeof(int));
    *p = idx;
    pthread_create(&workers[idx], NULL, verif_lin, p); // verificam liniile
    idx++;
    
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            zp = (struct zona*) malloc(sizeof(struct zona)); // aloca memorie pt parametrii
            zp->idx = idx; // index-ul thread-ului
            zp->x = i*3; // linia zonei   
            zp->y = j*3; // coloana zonei
            pthread_create(&workers[idx], NULL, verif_zona, zp); // verificam zona
            idx++;               
        }
    }
    
    for (i = 0; i < 11; i++)
        pthread_join(workers[i], 0); // asteptam terminarea executiei thread-urilor
    
    for (i = 0; i < 11; i++)
    {
        if (ok[i] != 1)
        {
            printf("Solutie invalida.\n");
            exit(-1);
        }
    }
    
    printf("Solutie valida!\n");
    return 0;
}
