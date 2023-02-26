#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef struct Node {
    int timestamp;
    double value;
    int exceptie; 
    struct Node *prev;
    struct Node *next;
} Node;

void removeList(Node *curr) {
    while(curr != NULL) {
        Node *urm = curr->next;
        free(curr);
        curr = urm;
    }
}

Node* initList(Node *new, int x, double y) {
    new = (Node*) calloc(1, sizeof(Node));
    new->timestamp = x;
    new->value = y;
    new->exceptie = 1;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

Node* updateList(Node *anterior, int x, double y) {
    Node* curr = (Node*) calloc(1, sizeof(Node));
    anterior->next = curr;
    curr->timestamp = x;
    curr->value = y;
    curr->exceptie = 1;
    curr->prev = anterior;
    curr->next = NULL;
    return curr;
}

void printList(Node *curr, int n) {
    int i = 0;
    while(i < n) {
        printf("%d %.2lf\n", curr->timestamp, curr->value);
        curr = curr->next;
        i++;
    }
}

// eliminare de exceptii
Node* e1(Node *head, int *n) {
    int k = 5; // fereastra de 5 noduri
    Node *temp = head;
    Node *return_copy = temp; // head-ul listei rezultate
    temp = temp->next->next; // incepem cu al 3-lea element (mijlocul ferestrei 1)
    while(temp->next->next != NULL) {
        // calculam media ferestrei de 5 elemente
        Node *elem1 = temp->prev->prev; // primul element al ferestrei
        int j;
        double sum = 0;
        for(j = 0; j < k; j++) {
            sum += elem1->value;
            elem1 = elem1->next;
        }
        double medie = sum / k; // media aritmetica
        Node *elem_1 = temp->prev->prev; // acelasi prim element, acum ptr deviatie
        double deviatie = 0;
        for(j = 0; j < k; j++) {
            deviatie += pow(elem_1->value - medie, 2);
            elem_1 = elem_1->next;
        }
        deviatie /= k;
        deviatie = sqrt(deviatie);
        if(!(medie - deviatie <= temp->value && temp->value <= medie + deviatie))
            temp->exceptie = 0; // nod care va fi eliminat 
        temp = temp->next;
    }
    Node *copie2 = return_copy;
    while(copie2 != NULL) {
        if(copie2->exceptie == 0) {
            Node *elim = copie2;
            copie2->prev->next = copie2->next;
            copie2->next->prev = copie2->prev;
            copie2 = copie2->next;
            free(elim);
            (*n)--;
        }
        else
            copie2 = copie2->next;
    }
    return return_copy;
}

double mediana_e2(Node *head) {
    // sortez o lista temp
    Node *temp = (Node*)calloc(1, sizeof(Node)); // lista de 5 elemente, sublista a listei initiale
    Node *copie = temp;
    Node *curr = head;
    temp->value = curr->value;
    curr = curr->next;
    int k;
    for(k = 1; k < 5; k++) {
        Node *urm = (Node*) calloc(1, sizeof(Node));
        temp->next = urm;
        urm->prev = temp;
        urm->next = NULL;
        urm->value = curr->value;
        temp = temp->next;
        curr = curr->next;
    }
    Node *i = NULL, *j = NULL;
    for(i = copie; i->next != NULL; i = i->next)
        for(j = i->next; j != NULL; j = j->next)
            if(i->value > j->value) {
                double aux = i->value;
                i->value = j->value;
                j->value = aux;
            }
    Node *mid = copie->next->next; // al treilea nod (jumatatea ferestrei k = 5)
    double m = mid->value;
    removeList(copie);
    return m;
}
double medie_e3(Node *head) {
    Node *temp = head;
    double ma = 0;
    int i;
    for(i = 0; i < 5; i++, temp = temp->next)
        ma += temp->value;
    return ma / 5;
}

Node* e2(Node *head, int *n, double (*f)(Node*)) {
    int i;
    int k = 5;
    int p = k / 2;
    Node *new = (Node*)calloc(1, sizeof(Node)); // lista care va fi afisata
    Node *cop_new = new;
    Node *curr = head;
    for(i = p; i < *n - p; i++) {
        double y = f(curr);
        int x = curr->next->next->timestamp; // timestampurile din nodurile 3 -> n-2 // eu am 0 -> n-4 iteratii
        new->timestamp = x;
        new->value = y;
        Node *urm = (Node*)calloc(1, sizeof(Node));
        new->next = urm;
        urm->prev = new;
        urm->next = NULL;

        new = new->next;
        curr = curr->next;
    }
    *n = *n - 2 * p; // se elimina primele si ultimele p noduri
    return cop_new;
}

Node *u(Node *head) {
    Node *curr = head;
    curr = curr->next; // pornim de la nodul 2 ptr a vf cu precedentul
    while(curr != NULL) {
        if(curr->timestamp - curr->prev->timestamp >= 100 && curr->timestamp - curr->prev->timestamp <= 1000) {
            // modificam datele
            curr->timestamp = (curr->timestamp + curr->prev->timestamp) / 2;
            curr->value = (curr->value + curr->prev->value) / 2;
        }
        curr = curr->next;
    }
    return head;
}
double w(int i, int k) {
    // w(i,k) dupa formula
    k = 3;
    double numarator = pow((double)i / (k - 1), 2) * 0.9 + 0.1;
    double numitor = 0;
    int j;
    for(j = 0; j < 3; j++)
        numitor = numitor + (pow((double)j / (k - 1), 2) * 0.9 + 0.1);
    return numarator / numitor;
}

Node* c(Node *head, int *n) {
    int x, y;
    Node *curr = head;
    curr = curr->next; // pornim de la nodul 2 ptr a compara cu precedentul
    while(curr != NULL) {
        if(curr->timestamp - curr->prev->timestamp > 1000) {
            // adaug noduri
            x = curr->prev->timestamp;
            y = curr->timestamp;
            Node *left = (Node*) calloc(1, sizeof(Node));
            Node *value_left = curr->prev; // marginea lui left
            left->next = NULL;
            // creez lista LEFT
            int i;
            for(i = 0; i < 2; i++) {
                left->value = value_left->value;
                Node *new_left = (Node*) calloc(1, sizeof(Node));
                left->prev = new_left;
                new_left->next = left;

                left = new_left;
                value_left = value_left->prev;
            }
            left->value = value_left->value;
            left->prev = NULL;

            Node *right = (Node*) calloc(1, sizeof(Node));
            Node *value_right = curr; // marginea lui right
            right->prev = NULL;
            // creez lista RIGHT
            for(i = 0; i < 2; i++) {
                right->value = value_right->value;
                Node *new_right = (Node*) calloc(1, sizeof(Node));
                right->next = new_right;
                new_right->prev = right;

                right = new_right;
                value_right = value_right->next;
            }
            right->value = value_right->value;
            right->next = NULL;
            double aux = right->value;
            right->value = right->prev->prev->value;
            right->prev->prev->value = aux;
            right = right->prev->prev;

            //intre nodurile care corespund lui x si y se vor adauga noduri noi
            curr = curr->prev; // acum ma aflu la nodul dupa care inserez nodurile noi
            int time;
            for(time = x + 200; time < y; time += 200) {
                // fiecare iteratie va adauga un nou nod
                Node* new = (Node*) calloc(1, sizeof(Node));
                new->timestamp = time;
                new->prev = curr;
                new->next = curr->next;
                curr->next->prev = new;
                curr->next = new;
                double C = (double)(time - x) / (y - x);
                // calculez f (value ptr noul nod)
                double f = 0, f1 = 0, f2 = 0;
                int k = 3;
                Node *cop = left;
                int i;
                for(i = 0; i < 3; i++) {
                    f1 = f1 + cop->value * w(i, k);
                    cop = cop->next;
                }
                Node *cop2 = right;
                for(i = 0; i < 3; i++) {
                    f2 = f2 + cop2->value * w(i, k);
                    cop2 = cop2->next;
                }
                f = (1-C) * f1 + C * f2;
                new->value = f;
                curr = curr->next;
                (*n)++;
            }
            removeList(left);
            removeList(right);
        }
        curr = curr->next;
    }
    return head;
}

// statistici pe intervale de nr timestamps
void st(Node *head, int nr) {
    double min; // value minim
    double max; // value maxim
    Node *temp = head;
    Node *sorted = (Node*) calloc(1, sizeof(Node)); // creez o noua lista sortata crescator dupa values
    Node *copie_sorted = sorted;
    sorted->prev = NULL;
    while (temp != NULL) {
        sorted->timestamp = temp->timestamp;
        sorted->value = temp->value;

        Node *new_sorted = (Node*) calloc(1, sizeof(Node));
        new_sorted->prev = sorted;
        sorted->next = new_sorted;
        new_sorted->next = NULL;

        sorted = sorted->next;
        temp = temp->next;
    }
    // acum sortez copia listei
    Node *i = NULL, *j = NULL;
    for(i = copie_sorted; i->next != NULL; i = i->next)
        for(j = i->next; j != NULL; j = j->next)
            if(i->value > j->value) {
                double aux = i->value;
                i->value = j->value;
                j->value = aux;
            }
    sorted = copie_sorted;
    int x = (int)floor(sorted->value); // capatul inferior al primului interval
    int y = x + nr;
    int nr_val = 0;
    while(sorted != NULL) {
        if(sorted->value >= x && sorted->value < y) {
            if(sorted->value > x)
                nr_val ++;
            sorted = sorted->next;
            if(sorted == NULL && nr_val > 0)
                printf("[%d, %d] %d\n", x, y, nr_val);
        }
        else {
            if(nr_val != 0) // avem minim o valoare in interval deci afisam
                printf("[%d, %d] %d\n", x, y, nr_val);
            x += nr;
            y += nr;
            nr_val = 0;
        }
    }
    removeList(copie_sorted);
}

int main(int argc, char *argv[]) {
    int n, timp;
    double val;
    scanf("%d", &n);
    scanf("%d %lf", &timp, &val);
    Node *head = NULL, *tail = NULL;
    head = initList(head, timp, val); // lista va avea 1 element
    tail = head;
    int i;
    for(i = 1; i < n; i++) {
        // continui sa primesc input-urile
        scanf("%d %lf", &timp, &val);
        tail = updateList(tail, timp, val);
    }
    int ok = 0, ok2 = 0, ok3 = 0;
    Node *mediana = NULL;
    Node *medie = NULL;
    Node *aux = head;
    Node *tmp = NULL;
    Node *lista_noua = NULL;
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--e1") == 0) {
            aux = e1(aux, &n);
        }
        if(strcmp(argv[i], "--e2") == 0) {
            mediana = e2(aux, &n, mediana_e2);
            ok = 1;
            aux = mediana;
        }
        if(strcmp(argv[i], "--e3") == 0) {
            medie = e2(aux, &n, medie_e3);
            ok2 = 1;
            aux = medie;
        }
        if(strcmp(argv[i], "--u") == 0) {
            aux = u(aux);
        }
        if(strcmp(argv[i], "--c") == 0) {
            aux = c(aux, &n);
        }
        if(strstr(argv[i], "st") != NULL) {
            char s[1000];
            strcpy(s, argv[i]);
            strcpy(s, s+4); // s contine doar numarul din string
            int nr = atoi(s);

            st(aux, nr);
            removeList(head);
            exit(1);
        }
    }
    printf("%d\n", n);
    printList(aux, n);
    if(ok == 1)
        removeList(mediana);
    if(ok2 == 1)
        removeList(medie);
    removeList(head);
    return 0;
}