#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STIAMO ASSEGNANDO UN NOME AL NUMERO NON è una VARIABILE GLOBALE
#define BUFFSIZE 16

typedef struct
{
    char *filename;
} Parameters; // Cn il nome alla fine soltanto noi stiamo semplificando una definizione un pò piu lunga

/*Stiamo praticamente facendo una cosa del tipo
struct p{
    char* filename;
};

typedef struct p Parameters  definire un tipo alla struttura serve per LEGGIBILITà
*/

// Ora quando lo facciamo per node avviene qualcosa di più complesso
//  Nel caso del node ho bisogno di riferirmi dentro e per farlo ho bisogno di darli un nome "provvisorio" e poi dare un nome definitivo alla struttura
typedef struct node
{
    int val;
    struct node *next;
} Node;

void error(char *err)
{

    fprintf(stderr, "[ERROR] %s", err);
    exit(1);
}

// Dobbiamo definire l'inserimento nella pila, la pila di piatti ogni volta che inseriamo un elemento lo mettiamo sopra l'altro ogni volta che prendiamo un elemento,prendiamo il primo della pila
void push(int val, Node **P)
{
    // Dopo che faremo un inseriemento la head dovrà necessariametne cambiare
    //  CI creiamo un puintatore a nodo e a questa variabile ci mettiamo una locazione di memoria che allochiamo appositamente per un nuovo nodo

    Node *new_node = (Node *)malloc(sizeof(Node));

    new_node->val = val;
    new_node->next = *P;
    *P = new_node;
}

Parameters decodeParameters(int argc, char *argv[])
{
    // Controllo sul numero di parametri
    if (argc != 2)
        error("Numero di parametri errato");

    if (strlen(argv[1]) > 100)
        error("Nome del file troppo lungo");

    // Andiamo a sommare la sua lunghezza meno quattro, quindi vado a trovare la sua lunghezza fino al quart'ultimo carattere

    Parameters p;
    p.filename = argv[1];
    // DOBBIAMO CONTROLLARE SE L'ESTENZIONE è .text
    if (strcmp(p.filename + strlen(p.filename) - 4, ".txt") != 0)
    {
        error("L'estensione del file non è corretta");
    }

    return p;
}

Node *readFile(FILE *f)
{
    // Dobbiamo leggere il file RIga per Riga
    Node *P = NULL;
    int n;
    char line[BUFFSIZE];

    // In questo modo a ogni iterazione leggeremo UNA RIGA PER VOLTA, quando finirà il file restituirà null e il while si interromperà
    while (fgets(line, BUFFSIZE, f))
    {
        // atoi converte una stringa in un  intero se può
        n = atoi(line);
        push(n, &P);
    }

    return P;
}

void printStack(Node *P)
{
    Node *curr = P;

    while (curr != NULL)
    {
        // Stampiamo il nodo
        printf("%d -> ", curr->val);
        curr = curr->next;
    }

    printf("NULL\n");
}

int stackSize(Node *P)
{
    int count = 0;
    Node *curr = P;

    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }

    return count;
}

// Pop da uno stack
int pop(Node **P)
{
    // Controllare se la pila è vuota
    if (*P == NULL)
        return -1;

    // Mi faccio il puntatore al nodo da elminare
    Node* to_delete = *P;
    // Sposto la head a destra
    *P = to_delete->next;
    int n = to_delete->val;
    free(to_delete);

    return n;
}

// Dobbiamo passare la pila a questo funzione! Come lo facciamo? DOBBIAMO PASSARE UN PUNTATORE ALLA HEAD DELLA PILA poichè la head
// della pila è un puntatore alla head devo passare un doppip puntatore
float getMean(Node **P)
{
    // Ogni volta che prendiamo un valore lo sommiamo alla variabile e auemntaimo il cantatore
    float m = 0;
    int count = 0;
    int n = pop(P);

    while (n != -1)
    {
        m += n;
        count++;
        n = pop(P);
    }

    m /= count; // m = m/ count trovo la media praticamente
    return m;
}

void fillP(FILE *f, Node **P, float m)
{
    int x;
    char line[BUFFSIZE];
    rewind(f);
    fgets(line, BUFFSIZE, f);
    push(atoi(line), P);

    // In questo modo a ogni iterazione leggeremo UNA RIGA PER VOLTA, quando finirà il file restituirà null e il while si interromperà
    while (fgets(line, BUFFSIZE, f))
    {
        // atoi converte una stringa in un  intero se può
        x = atoi(line);
        if (x > m)
        {
            push(x, P);
        }
        else
        {
            int y = pop(P);
            push((x + y) / 2, P);
        }
    }
}

int *transferP(Node **P)
{
    int dim = stackSize(*P);
    int* A = (int*) malloc(sizeof(int) * dim);
    for (int i =0; i < dim; i++)
    {
        A[i] = pop(P);
    }

    return A;
}

void bubbleSort(int *A, int n)
{
    int aux;
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < n-i-1; j++)
        {
            if(A[j] > A[j+1]) {
                aux = A[j+1];
                A[j+1] = A[j];
                A[j] = aux;
            }
        }
    }
}

// UNa pila una lista una coda la identifichiamo come un puntatore al primo eleento

int main(int argc, char *argv[])
{
    printf("===============PUNTO A===============\n");
    // Definiamo la nostra struttura a questo punto
    Parameters p = decodeParameters(argc, argv);
    printf("Filename %s\n\n", p.filename);

    printf("===============PUNTO B===============\n");
    FILE *f = fopen(p.filename, "r");
    Node *P = readFile(f);
    printStack(P);

    printf("===============PUNTO C===============\n");
    float m = getMean(&P);
    printf("mean = %f", m);

    printf("===============PUNTO D===============\n");

    fclose(f);

    return 0;
}