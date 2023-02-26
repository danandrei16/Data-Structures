#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graf {
    int nr_noduri;
    int nr_depozite;
    int *depozite;
    float **M; // matrice de adiacenta
} Graf;

typedef struct Node {
    int val;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *head;
    Node *tail;
    int len;
} Stack;

Node* initNode(Node *cell, int val) {

    cell = (Node*)malloc(sizeof(Node));
    cell->next = cell->prev = NULL;
    cell->val = val;

    return cell;
}

Stack* initStack(Stack *s) {

    s = (Stack*)malloc(sizeof(Stack));
    s->head = s->tail = NULL;
    s->len = 0;

    return s;
}

void push(Stack *s, int val) {

    Node *new = initNode(new, val);

    if(s->len == 0)
        s->head = s->tail = new;
    else {
        new->next = s->head;
        s->head->prev = new;
        s->head = new;
    }
    s->len++;
}

int pop(Stack *s) {
    if(s->len == 1) {
        Node *elim = s->head;
        int val = elim->val;
        s->head = s->tail = NULL;
        s->len--;
        free(elim);
        return val;
    }
    else {
        Node *elim = s->head;
        int val = elim->val;
        s->head->next->prev = NULL;
        s->head = s->head->next;
        s->len--;
        free(elim);
        return val;
    }
}

void DFS(float **mat, int n, int *visited, int i, Stack *s) {
    visited[i] = 1;
    int j;
    for(j = 0; j < n; j++)
        if(visited[j] == 0 && mat[i][j] != 0)
            DFS(mat, n, visited, j, s);
    push(s, i);
    //printf("%d ", i);
}

int min(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}

void DFS_Transpusa(float **mat, int n, int *visited, Stack *s) {
        //printf("<%d>\n", s->head->val);
        int i;
        int x = pop(s);  // elimin din stiva si salvez valoare din varf
        printf("%d ", x);
        visited[x] = 1;
        for(i = 0; i < n; i++)
            if(visited[x] == 1) // daca nu e vizitat nodul
            //visited[x] = 0;
                DFS_Transpusa(mat, n, visited, s);
}

void matrice_adiacenta(Graf *g, int n, int m, int d) {
    int i, j;
    int u, v;
    float w;
    // creez matricea de adiacenta si citesc valori
    g->nr_noduri = n;

    g->M = malloc(n * sizeof(float*));

    for(i = 0; i < n; i++) {
        g->M[i] = malloc(n * sizeof(float));
        for(j = 0; j < n; j++)
            g->M[i][j] = 0; // initial graful este nul (0 pestetot)
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d %f", &u, &v, &w);
        g->M[u][v] = w;
    }

    // creez vectorul cu depozitele
    g->nr_depozite = d;

    g->depozite = malloc(g->nr_depozite * sizeof(int));
    for(i = 0; i < g->nr_depozite; i++)
        scanf("%d", &g->depozite[i]);
}

void print_graf(Graf *g) {
    int i, j;
    for(i = 0; i < g->nr_noduri; i++) {
        for(j = 0; j < g->nr_noduri; j++)
            printf("%.1f ", g->M[i][j]);
        printf("\n");
    }
}

void free_graf(Graf *g) {
    int i;
    for(i = 0; i < g->nr_noduri; i++)
        free(g->M[i]);
    free(g->M);
    free(g->depozite);
    free(g);
}

void printPath(int *path, int j, int depozit) {
    // afisez recursiv drumul de cost minim (nod cu nod)
    if(path[j] == -1)
        return;
    printPath(path, path[j], depozit);
    printf("%d", j);
    if(j != depozit)
        printf(" ");
}

void sortArr(int *v, int n) {
    // functie de sortare crescator a unui vector cu n elemente
    int i, j;
    for(i = 0; i < n - 1; i++)
        for(j = i; j < n; j++)
            if(v[i] > v[j]) {
                int aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
}

void Dijkstra(Graf *g, int nod_curent, int *path, float *dist) {
    int i, j, k, x;
    // i si j pentru iteratii
    // k va contine indicele cu min(cost)
    //nod_curent = depozit; // pornesc sa caut drumul cel mai scurt de la nodul depozit
    // pentru drumurile de la magazin la depozit, nodul curent va fi magazinul

    // initial distanta va avea o valoare mare, 100 de ex (mai mare decat costul maxim din orice graf din inputuri)
    // daca aveam valori mult mai mari as fi luat FLT_MAX
    for(i = 0; i < g->nr_noduri; i++)
        dist[i] = 100;
    
    // calculez costul minim de la depozit pana la cele nr_noduri - 1 magazine
    int *visited = malloc(g->nr_noduri * sizeof(int)); // tin evidenta care noduri au fost deja vizitate
    for(i = 0; i < g->nr_noduri; i++)
        visited[i] = 0; // initial niciun nod vizitat
    //int visited[20] = {0}; // 0 daca nodul i nu a fost vizitat, 1 daca este vizitat
    //int ok = 0;

    //construiesc un vector pentru unde path[i] este parinte a lui i
    for(i = 0; i < g->nr_noduri; i++)
        path[i] = -1; // val initiala, presupun -1

    // intai nodurile adiacente cu nodul initial, costul va fi initial valoarea de pe muchia respectiva, daca exista
    for(j = 0; j < g->nr_noduri; j++) {
        float cost = g->M[nod_curent][j];
        if(cost != 0) {
            dist[j] = cost;
            path[j] = nod_curent;
        }
    }
    visited[nod_curent] = 1; // nodul a fost vizitat
    
    // vizitez restul nr_noduri - 1
    for(x = 1; x < g->nr_noduri; x++) {
        float minim = 100; // consider initial val maxima de 100    
        for(j = 0; j < g->nr_noduri; j++) {
            float cost = g->M[nod_curent][j]; // cost
            if(cost != 0 && visited[j] == 0) { // exista muchie cu nod nevizitat
                    if(dist[nod_curent] + cost < dist[j]) {
                        dist[j] = cost + dist[nod_curent];
                        path[j] = nod_curent;
                    }
            }
            // caut indicele care are costul minim
        }
        visited[nod_curent] = 1;
        for(j = 0; j < g->nr_noduri; j++)
            if(dist[j] < minim && visited[j] == 0) {
                    minim = dist[j];
                    k = j;
                }
        nod_curent = k;
        //printf("<%d>", nod_curent);
    }
    free(visited);
}

// transpusa unei matrice
void **transpose(float **m, float **trans, int n) {
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            trans[i][j] = m[j][i];
}

void Tarjan(Graf *g, int nod, int *index, int *id, int *lowLink, int *visited, Stack *s, int *scc, int **mat_scc) {
    id[nod] = *index;
    lowLink[nod] = *index;
    visited[nod] = 1;
    push(s, nod);
    (*index)++;

    // int nr_noduri_adiacente = 0;
    // int i;
    // for(i = 0; i < g->nr_noduri; i++)
    //     if(g->M[nod][i] != 0)
    //         nr_noduri_adiacente++;
    int i;
    for(i = 0; i < g->nr_noduri; i++) {
        // ignor depozitul cand caut SCC
        if(i == g->depozite[0])
           continue;
        
        if(g->M[nod][i] != 0) {
            // vizitez vecinii nodului 'nod'
            if(id[i] == -1) {
                Tarjan(g, i, index, id, lowLink, visited, s, scc, mat_scc);
                lowLink[nod] = min(lowLink[nod], lowLink[i]);
            }
            else
                if(visited[i] != 0)
                    lowLink[nod] = min(lowLink[nod], id[i]);
        }
    }
    
    if(lowLink[nod] == id[nod] && nod != 0) {
        (*scc)++;
        // am gasit o componenta tare conexa
        // eliberam nodurile respective din stiva
        //printf("<%d>", nod);
        int *crescator = malloc(g->nr_noduri * sizeof(int));
        int x = 0;
        while(s->head->val != nod) {
            // if(s->head->val == nod)
            //     break;
            //printf("lol");
            int nod_scc = pop(s);
            visited[nod_scc] == 0;
            //printf("%d ", nod_scc);
            crescator[x++] = nod_scc;
        }
        int nod_scc = pop(s);
        visited[nod_scc] == 0;
        //printf("%d ", nod_scc);
        crescator[x++] = nod_scc;
        sortArr(crescator, x);
        int y;
        int linie = crescator[0];
        for(y = 0; y < x; y++)
            mat_scc[linie][y] = crescator[y];
        //printf("\n");
        free(crescator);
    }
}

int main() {
    Graf *g = malloc(sizeof(Graf));
    int i, j;
    int n, m, d;
    scanf("%d %d %d", &n, &m, &d); // noduri; muchii; depozite
    matrice_adiacenta(g, n, m, d);

    // rezolvare task-uri
    int nr_taskuri; // nr de task-uri de rezolvat
    scanf("%d", &nr_taskuri);
    char task[3];
    for(i = 0; i < nr_taskuri; i++) {
        scanf("%s", task);
        if(strcmp(task, "e1") == 0) {
            // rezolvam task1
            int depozit, nr_magazine;
            scanf("%d%d", &depozit, &nr_magazine);
            int *magazin = malloc(nr_magazine * sizeof(int)); // vector care contine indicii magazinelor vizitate
            for(j = 0; j < nr_magazine; j++)
                scanf("%d", &magazin[j]);

            float *cost = malloc(g->nr_noduri * sizeof(float)); // vector care contine costurile de la depozit la magazinul i
            int *path = malloc(g->nr_noduri * sizeof(int)); // path[i] = parintele lui i
            Dijkstra(g, depozit, path, cost); // drum dus
            float suma = 0; // suma totala a drumurilor
            for(j = 0; j < nr_magazine; j++) {
                // acum fac acelasi lucru pentru drumul intors (ptr fiecare magazin)
                int *path_return = malloc(g->nr_noduri * sizeof(int));
                float *cost_return = malloc(g->nr_noduri * sizeof(float));
                Dijkstra(g, magazin[j], path_return, cost_return);
                // afisare
                printf("%d\n", magazin[j]); // indicele magazinului
                printf("%.1f %.1f\n", cost[magazin[j]], cost_return[depozit]); // costul dus catre magazinul j si costul intors inapoi in depozit
                
                // afisez drumul minim 
                printf("%d ", depozit); // primul nod
                printPath(path, magazin[j], depozit); // parcurgerea dus
                printPath(path_return, depozit, depozit); // parcurgerea intors

                printf("\n");
                suma += cost[magazin[j]] + cost_return[depozit];

                free(path_return);
                free(cost_return);
            }
            printf("%.1f", suma); // suma costurilor dus+intors
            free(path);
            free(cost);
            free(magazin);
        }
        if(strcmp(task, "e2") == 0) {
            int k, x, y;
            // creez o matrice n x n in care retin pe linia i, a i-a componenta tare conexa
            int **mat_scc = malloc(g->nr_noduri * sizeof(int*));
            for(k = 0; k < g->nr_noduri; k++)
                mat_scc[k] = malloc(g->nr_noduri * sizeof(int));
            for(x = 0; x < g->nr_noduri; x++)
                for(y = 0; y < g->nr_noduri; y++)
                    mat_scc[x][y] = 0; // initial 0 peste tot

            // aplic algoritmul lui Tarjan
            int *id = malloc(g->nr_noduri * sizeof(int)); // id[i] -> id-ul nodului i
            int *lowLink = malloc(g->nr_noduri * sizeof(int));
            int *visited = malloc(g->nr_noduri * sizeof(int));
            for(j = 0; j < g->nr_noduri; j++) {
                id[j] = -1;
                lowLink[j] = -1;
                visited[j] = 0; // initial nu a fost vizitat
            }

            // initiez o stiva
            Stack *s = initStack(s);
            int nr_scc = 0;
            int index = 0;
            for(j = 0; j < g->nr_noduri; j++)
                if(id[j] == -1)
                    Tarjan(g, j, &index, id, lowLink, visited, s, &nr_scc, mat_scc);

            printf("%d\n", nr_scc); // nr comp tare conexe
            for(x = 0; x < g->nr_noduri; x++) {
                for(y = 0; y < g->nr_noduri; y++)
                    if(mat_scc[x][y] != 0)
                        printf("%d ", mat_scc[x][y]);
                if(mat_scc[x][0] != 0)
                    printf("\n");
            }
            for(x = 0; x < g->nr_noduri; x++)
                free(mat_scc[x]);
            free(mat_scc);
            free(visited);
            free(lowLink);
            free(id);
            free(s->head);
            free(s);
        }
        if(i < nr_taskuri - 1)
            printf("\n");
    }
    free_graf(g);
    return 0;
}