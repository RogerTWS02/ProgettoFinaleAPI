// "Ex nihilo nihil fit"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[5200];
int input_placeholder;
short int useless = 1;

// Struttura del nodo dell'albero delle stazioni
typedef struct stazione
{
    unsigned int distanza;
    int parco_macchine[513];
    struct stazione *padre, *figlio_dx, *figlio_sx;
} *stazione;

// Struttura del nodo della lista delle stazioni di un percorso
typedef struct stazione_percorso
{
    unsigned int distanza;
    struct stazione_percorso *next, *padre;
} *stazione_percorso;

typedef struct stazione_percorso_bwd
{
    unsigned int distanza;
    struct stazione_percorso_bwd *next;
} *stazione_percorso_bwd;

// Dichiarazione della radice dell'albero delle stazioni come variabile globale
stazione root_tree_stazioni = NULL;

// Legge la stringa tenendo conto del placeholder e restituisce i valori numerici necessari per eseguire i comandi
unsigned int estrai_valore(char *input)
{
    unsigned int valore = 0;
    char str[32];
    sscanf(input + input_placeholder, "%u", &valore);
    sprintf(str, "%u", valore);
    input_placeholder += strlen(str) + 1;
    return valore;
}

// Ricerca elemento per chiave nell'albero, fornita la radice (STAZIONI)
stazione ricerca_stazione(stazione root_tree_stazioni, unsigned int valore)
{
    if (root_tree_stazioni == NULL)
        return NULL;
    stazione curr = root_tree_stazioni;
    while (curr != NULL)
    {
        if (curr->distanza == valore)
            return curr;
        if (curr->distanza < valore)
            curr = curr->figlio_dx;
        else
            curr = curr->figlio_sx;
    }
    return NULL;
}

// Ricerca del minimo di un albero
stazione minimo(stazione current)
{
    while (current->figlio_sx != NULL)
        current = current->figlio_sx;
    return current;
}

// Ricerca successore di un determinato nodo dell'albero delle stazioni
stazione successore(stazione current)
{
    if (current->figlio_dx != NULL)
        return minimo(current->figlio_dx);
    stazione temp = current->padre;
    while (temp != NULL && current == temp->figlio_dx)
    {
        current = temp;
        temp = temp->padre;
    }
    return temp;
}

//Trova il massimo di un BST
stazione massimo(stazione current)
{
    while (current->figlio_dx != NULL)
        current = current->figlio_dx;
    return current;
}

//Trova il predecessore di un determinato nodo in un BST
stazione predecessore(stazione current)
{
    if (current->figlio_sx != NULL)
        return massimo(current->figlio_sx);
    stazione temp = current->padre;
    while (temp != NULL && current == temp->figlio_sx)
    {
        current = temp;
        temp = temp->padre;
    }
    return temp;
}

stazione ricerca_ausiliaria(stazione current, unsigned int valore)
{
    stazione temp = current;
    current = predecessore(current);
    while(current != NULL)
    {
        if(current->distanza < valore)
            return temp;
        temp = current;
        current = predecessore(current);
    }
    return temp;
}

//Cerca stazione candidata per pianifica bwd
stazione ricerca_stazione_candidata(stazione current, unsigned int valore)
{
    current = predecessore(current);
    if(current == NULL)
        return NULL;
    if(current->distanza < valore)
        return NULL;
    stazione migliore_stazione = current;
    int limite_stazione = predecessore(migliore_stazione)->distanza + 1;
    int temp;
    current = predecessore(current);
    while(current != NULL)
    {
        temp = current->distanza - current->parco_macchine[512];
        if(current->distanza < valore)
            return migliore_stazione;
        if(temp == migliore_stazione->distanza - migliore_stazione->parco_macchine[512])
            migliore_stazione = current;
        else if(temp < migliore_stazione->distanza - migliore_stazione->parco_macchine[512])
        {
            migliore_stazione = current;
            limite_stazione = ricerca_ausiliaria(migliore_stazione, temp)->distanza + 1;
        }
        else
        {
            if(temp < limite_stazione)
            {
                migliore_stazione = current;
                limite_stazione = ricerca_ausiliaria(migliore_stazione, temp)->distanza + 1;
            }
        }
        current = predecessore(current);
    }
    return NULL;
}

// Aggiunge un nodo ad una lista
stazione_percorso aggiungi_in_lista(stazione_percorso ultimo_nodo, unsigned int valore)
{
    ultimo_nodo->next = malloc(sizeof(struct stazione_percorso));
    stazione_percorso temp = ultimo_nodo->next;
    temp->distanza = valore;
    temp->next = NULL;
    temp->padre = ultimo_nodo;
    return ultimo_nodo->next;
}

stazione_percorso_bwd aggiungi_in_lista_bwd(stazione_percorso_bwd ultimo_nodo, unsigned int valore)
{
    ultimo_nodo->next = malloc(sizeof(struct stazione_percorso_bwd));
    stazione_percorso_bwd temp = ultimo_nodo->next;
    temp->distanza = valore;
    temp->next = NULL;
    return ultimo_nodo->next;
}

// Dealloca un'intera lista, data la testa
void dealloca_lista(stazione_percorso nodo)
{
    stazione_percorso temp = nodo->next;
    while (temp != NULL)
    {
        free(nodo);
        nodo = temp;
        temp = temp->next;
    }
    free(nodo);
    return;
}

void dealloca_lista_bwd(stazione_percorso_bwd nodo)
{
    stazione_percorso_bwd temp = nodo->next;
    while (temp != NULL)
    {
        free(nodo);
        nodo = temp;
        temp = temp->next;
    }
    free(nodo);
    return;
}

// Stampa gli elementi di una lista, data la testa
void stampa_percorso_fwd(stazione_percorso nodo, unsigned int partenza, unsigned int arrivo)
{
    printf("%u ", partenza);
    while (nodo->distanza != arrivo)
    {
        printf("%u ", nodo->distanza);
        if(nodo->padre == NULL)
            break;
        nodo = nodo->padre;
    }
    printf("%u\n", arrivo);
    return;
}

void stampa_percorso_bwd(stazione_percorso_bwd current, unsigned int arrivo)
{
    while(current != NULL)
    {
        printf("%u ", current->distanza);
        current = current->next;
    }
    printf("%u\n", arrivo);
    return;
}

// AGGIUNGE STAZIONE
void aggiungi_stazione()
{
    int i = 0;
    input_placeholder = 18;
    unsigned int valore = estrai_valore(buffer);
    stazione prec = NULL;
    stazione curr = root_tree_stazioni;
    while (curr != NULL)
    {
        prec = curr;
        if (valore == curr->distanza)
        {
            printf("non aggiunta\n");
            return;
        }
        if (valore < curr->distanza)
            curr = curr->figlio_sx;
        else
            curr = curr->figlio_dx;
    }
    curr = malloc(sizeof(struct stazione));
    curr->distanza = valore;
    curr->parco_macchine[0] = -1;
    curr->parco_macchine[512] = 0;
    curr->figlio_dx = NULL;
    curr->figlio_sx = NULL;
    if (prec != NULL)
    {
        curr->padre = prec;
        if (valore < prec->distanza)
            prec->figlio_sx = curr;
        else
            prec->figlio_dx = curr;
    }
    else
    {
        curr->padre = NULL;
        root_tree_stazioni = curr;
    }
    printf("aggiunta\n");
    int numero_auto = estrai_valore(buffer);
    if(numero_auto == 0)
    {
        curr->parco_macchine[0] = -1;
        return;
    }
    int migliore_auto = 0;
    while (i < numero_auto)
    {
        valore = estrai_valore(buffer);
        if(valore > migliore_auto)
            migliore_auto = valore;
        curr->parco_macchine[i] = valore;
        i++;
    }
    if(i<512)
        curr->parco_macchine[i] = -1;
    curr->parco_macchine[512] = migliore_auto;
    return;
}

//Copia il parco macchine
void trasferisci_parco_macchine(stazione current, stazione successore)
{
    if(successore->parco_macchine[0] == -1)
    {
        current->parco_macchine[0] = -1;
        return;
    }
    current->parco_macchine[512] = successore->parco_macchine[512];
    int i = 0;
    while(successore->parco_macchine[i] != -1 && i < 512)
    {
        current->parco_macchine[i] = successore->parco_macchine[i];
        i++;
    }
    if(i < 512)
        current->parco_macchine[i] = -1;
    return;
}

//DEMOLISCE STAZIONE IN CASO SPECIFICO
void demolisci_stazione_B(stazione current)
{
    if (current == NULL)
    {
        printf("non demolita\n");
        return;
    }
    if(current->figlio_sx == NULL && current->figlio_dx == NULL)
    {
        if(root_tree_stazioni == current)
        {
            root_tree_stazioni = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
        if(current == current->padre->figlio_sx)
        {
            current->padre->figlio_sx = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
        else
        {
            current->padre->figlio_dx = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
    }
    if(current->figlio_sx == NULL || current->figlio_dx == NULL)
    {
        if(current->figlio_dx == NULL)
        {
            if(current == root_tree_stazioni)
            {
                root_tree_stazioni = current->figlio_sx;
                current->figlio_sx->padre = NULL;
                free(current);
                printf("demolita\n");
                return;
            }
            if(current == current->padre->figlio_sx)
            {
                current->padre->figlio_sx = current->figlio_sx;
                current->figlio_sx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_sx;
                free(current);
                printf("demolita\n");
                return;
            }
            else
            {
                current->padre->figlio_dx = current->figlio_sx;
                current->figlio_sx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_sx;
                free(current);
                printf("demolita\n");
                return;
            }
        }
        else
        {
            if(current == root_tree_stazioni)
            {
                root_tree_stazioni = current->figlio_dx;
                current->figlio_dx->padre = NULL;
                free(current);
                printf("demolita\n");
                return;
            }
            if(current == current->padre->figlio_sx)
            {
                current->padre->figlio_sx = current->figlio_dx;
                current->figlio_dx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_dx;
                free(current);
                printf("demolita\n");
                return;
            }
            else
            {
                current->padre->figlio_dx = current->figlio_dx;
                current->figlio_dx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_dx;
                free(current);
                printf("demolita\n");
                return;
            }
        }
    }
    stazione temp = successore(current);
    current->distanza = temp->distanza;
    trasferisci_parco_macchine(current, temp);
    demolisci_stazione_B(temp);
    return;
}

// DEMOLISCE STAZIONE DA COMANDO DEDICATO
void demolisci_stazione_A()
{
    input_placeholder = 19;
        stazione current = ricerca_stazione(root_tree_stazioni, estrai_valore(buffer));
    if (current == NULL)
    {
        printf("non demolita\n");
        return;
    }
    if(current->figlio_sx == NULL && current->figlio_dx == NULL)
    {
        if(root_tree_stazioni == current)
        {
            root_tree_stazioni = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
        if(current == current->padre->figlio_sx)
        {
            current->padre->figlio_sx = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
        else
        {
            current->padre->figlio_dx = NULL;
            free(current);
            printf("demolita\n");
            return;
        }
    }
    if(current->figlio_sx == NULL || current->figlio_dx == NULL)
    {
        if(current->figlio_dx == NULL)
        {
            if(current == root_tree_stazioni)
            {
                root_tree_stazioni = current->figlio_sx;
                current->figlio_sx->padre = NULL;
                free(current);
                printf("demolita\n");
                return;
            }
            if(current == current->padre->figlio_sx)
            {
                current->padre->figlio_sx = current->figlio_sx;
                current->figlio_sx->padre = current->padre;
                free(current);
                printf("demolita\n");
                return;
            }
            else
            {
                current->padre->figlio_dx = current->figlio_sx;
                current->figlio_sx->padre = current->padre;
                free(current);
                printf("demolita\n");
                return;
            }
        }
        else
        {
            if(current == root_tree_stazioni)
            {
                root_tree_stazioni = current->figlio_dx;
                current->figlio_dx->padre = NULL;
                free(current);
                printf("demolita\n");
                return;
            }
            if(current == current->padre->figlio_sx)
            {
                current->padre->figlio_sx = current->figlio_dx;
                current->figlio_dx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_dx;
                free(current);
                printf("demolita\n");
                return;
            }
            else
            {
                current->padre->figlio_dx = current->figlio_dx;
                current->figlio_dx->padre = current->padre;
                if(current == root_tree_stazioni)
                    root_tree_stazioni = current->figlio_dx;
                free(current);
                printf("demolita\n");
                return;
            }
        }
    }
    stazione temp = successore(current);
    current->distanza = temp->distanza;
    trasferisci_parco_macchine(current, temp);
    demolisci_stazione_B(temp);
    return;
}

// AGGIUNGE AUTO DATA IN INPUT DAL COMANDO DEDICATO
void aggiungi_auto()
{
    input_placeholder = 14;
    int i = 0;
    stazione current = ricerca_stazione(root_tree_stazioni, estrai_valore(buffer));
    if (current == NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    int valore = estrai_valore(buffer);
    if(valore > current->parco_macchine[512])
        current->parco_macchine[512] = valore;
    while(i < 512 && current->parco_macchine[i] != -1)
        i++;
    current->parco_macchine[i] = valore;
    if(i < 511)
        current->parco_macchine[i+1] = -1;
    printf("aggiunta\n");
    return;
}

// ROTTAMA AUTO
void rottama_auto()
{
    input_placeholder = 13;
    stazione current = ricerca_stazione(root_tree_stazioni, estrai_valore(buffer));
    if (current == NULL)
    {
        printf("non rottamata\n");
        return;
    }
    if (current->parco_macchine[0] == -1)
    {
        printf("non rottamata\n");
        return;
    }
    int valore = estrai_valore(buffer);
    int i = 0;
    if(valore < current->parco_macchine[512])
    {
        while (current->parco_macchine[i] != valore && i < 512 && current->parco_macchine[i] != -1)
            i++;
        if (i == 512 || current->parco_macchine[i] == -1)
        {
            printf("non rottamata\n");
            return;
        }
        if (i == 511)
        {
            current->parco_macchine[i] = -1;
            printf("rottamata\n");
            return;
        }
        valore = current->parco_macchine[i + 1];
        if (valore == -1)
        {
            current->parco_macchine[i] = -1;
            current->parco_macchine[i + 1] = 0;
            printf("rottamata\n");
        }
        else
        {
            while (valore != -1 && i < 511)
            {
                current->parco_macchine[i] = valore;
                valore = current->parco_macchine[i + 2];
                i++;
            }
            current->parco_macchine[i] = -1;
            printf("rottamata\n");
        }
        return;
    }
    else
    {
        int migliore_auto = 0;
        while (current->parco_macchine[i] != valore && i < 512 && current->parco_macchine[i] != -1)
        {
            if (current->parco_macchine[i] > migliore_auto)
                migliore_auto = current->parco_macchine[i];
            i++;
        }
        if (i == 512 || current->parco_macchine[i] == -1)
        {
            printf("non rottamata\n");
            return;
        }
        if (i == 511)
        {
            current->parco_macchine[i] = -1;
            current->parco_macchine[512] = migliore_auto;
            printf("rottamata\n");
            return;
        }
        valore = current->parco_macchine[i + 1];
        if (valore == -1)
        {
            current->parco_macchine[i] = -1;
            current->parco_macchine[i + 1] = 0;
            current->parco_macchine[512] = migliore_auto;
            printf("rottamata\n");
        }
        else
        {
            while (valore != -1 && i < 511)
            {
                current->parco_macchine[i] = valore;
                if(valore > migliore_auto)
                    migliore_auto = valore;
                valore = current->parco_macchine[i + 2];
                i++;
            }
            current->parco_macchine[i] = -1;
            current->parco_macchine[512] = migliore_auto;
            printf("rottamata\n");
        }
        return;
    }
}

// PIANFICA PERCORSO "IN AVANTI"
void pianifica_percorso_fwd(unsigned int valore_stazione_partenza, unsigned int valore_stazione_arrivo)
{
    stazione stazione_partenza = ricerca_stazione(root_tree_stazioni, valore_stazione_partenza);
    stazione current = ricerca_stazione(root_tree_stazioni, valore_stazione_arrivo);    
    if(current == NULL || stazione_partenza == NULL)
    {
        printf("nessun percorso\n");
        return;
    }
    if(current->parco_macchine[512] >= valore_stazione_arrivo - valore_stazione_partenza)
    {
        printf("%u %u", valore_stazione_partenza, valore_stazione_arrivo);
        return;
    }
    stazione temp = predecessore(current);
    if(temp != NULL)
    {
        if(temp->distanza == valore_stazione_partenza)
        {
            printf("nessun percorso\n");
            return;
        }
    }
    else
    {
        printf("nessun percorso\n");
        return;
    }
    stazione_percorso testa = malloc(sizeof(struct stazione_percorso));
    testa->distanza = valore_stazione_arrivo;
    testa->next = NULL;
    testa->padre = NULL;
    stazione_percorso ultimo_nodo = testa;
    unsigned int migliore_stazione = 0;
    while(current->distanza > valore_stazione_partenza && temp->distanza > valore_stazione_partenza)
    {
        if(stazione_partenza->distanza + stazione_partenza->parco_macchine[512] >= current->distanza)
        {
            stampa_percorso_fwd(ultimo_nodo, valore_stazione_partenza, valore_stazione_arrivo);
            dealloca_lista(testa);
            return;
        }
        while(temp->distanza > valore_stazione_partenza)
        {
            if(temp->distanza + temp->parco_macchine[512] >= current->distanza)
                migliore_stazione = temp->distanza;
            temp = predecessore(temp);
            if(temp == NULL)
            {
                printf("nessun percorso\n");
                dealloca_lista(testa);
                return;
            }
        }
        if(migliore_stazione == ultimo_nodo->distanza)
        {
            printf("nessun percorso\n");
            dealloca_lista(testa);
            return;
        }
        ultimo_nodo = aggiungi_in_lista(ultimo_nodo, migliore_stazione);
        current = ricerca_stazione(root_tree_stazioni, migliore_stazione);
        if(current == NULL)
        {
            printf("nessun percorso\n");
            dealloca_lista(testa);
            return;
        }
        temp = predecessore(current);
        if(temp == NULL)
        {
            printf("nessun percorso\n");
            dealloca_lista(testa);
            return;
        }
    }
    if(temp->distanza + temp->parco_macchine[512] >= current->distanza)
        stampa_percorso_fwd(ultimo_nodo, valore_stazione_partenza, valore_stazione_arrivo);
    else
        printf("nessun percorso\n");
    dealloca_lista(testa);
    return;
}

// PIANIFICA PERCORSO "AL CONTRARIO"
void pianifica_percorso_bwd(unsigned int valore_stazione_partenza, unsigned int valore_stazione_arrivo)
{
    stazione current = ricerca_stazione(root_tree_stazioni, valore_stazione_partenza);
    stazione stazione_arrivo = ricerca_stazione(root_tree_stazioni, valore_stazione_arrivo);    
    if(current == NULL || stazione_arrivo == NULL)
    {
        printf("nessun percorso\n");
        return;
    }
    if(current->parco_macchine[512] >= valore_stazione_partenza - valore_stazione_arrivo)
    {
        printf("%u %u", valore_stazione_partenza, valore_stazione_arrivo);
        return;
    }
    stazione_percorso_bwd testa = malloc(sizeof(struct stazione_percorso_bwd));
    testa->distanza = valore_stazione_partenza;
    testa->next = NULL;
    stazione_percorso_bwd ultimo_nodo = testa;
    stazione stazione_candidata;
    int temp;
    while(current->distanza > valore_stazione_arrivo)
    {
        temp = current->distanza - current->parco_macchine[512];
        if(temp < 0 || temp <= valore_stazione_arrivo)
        {
            stampa_percorso_bwd(testa, valore_stazione_arrivo);
            dealloca_lista_bwd(testa);
            return;
        }
        stazione_candidata = ricerca_stazione_candidata(current, temp);
        if(stazione_candidata == NULL)
        {
            printf("nessun percorso\n");
            dealloca_lista_bwd(testa);
            return;
        }
        current = stazione_candidata;
        ultimo_nodo = aggiungi_in_lista_bwd(ultimo_nodo, current->distanza);
    }
    stampa_percorso_bwd(testa, valore_stazione_arrivo);
    dealloca_lista_bwd(testa);
    return;
}

// PIANIFICA PERCORSO
void pianifica_percorso()
{
    input_placeholder = 19;
    unsigned int stazione_partenza = estrai_valore(buffer);
    unsigned int stazione_arrivo = estrai_valore(buffer);
    if (stazione_partenza == stazione_arrivo)
        printf("%u\n", stazione_partenza);
    else
    {
        if (stazione_partenza < stazione_arrivo)
            pianifica_percorso_fwd(stazione_partenza, stazione_arrivo);
        else
            pianifica_percorso_bwd(stazione_partenza, stazione_arrivo);
    }
    return;
}

// Gestisce i comandi in input chiamando le funzioni giuste in base al comando iniettato
void input_handler(char *input)
{
    if (input[0] == 'a' && input[9] == 's')
    {
        aggiungi_stazione();
        return;
    }
    if (input[0] == 'a' && input[9] == 'a')
    {
        aggiungi_auto();
        return;
    }
    if (input[0] == 'd')
    {
        demolisci_stazione_A();
        return;
    };
    if (input[0] == 'r')
    {
        rottama_auto();
        return;
    }
    if (input[0] == 'p')
    {
        pianifica_percorso();
        return;
    }
}

// Legge riga per riga da input redirection e le passa, una per una, a input_handler
void input_reader()
{
    if (fgets(buffer, 5200, stdin))
    {
        useless = 1;
        input_placeholder = 0;
        input_handler(buffer);
    }
    else
        useless = 0;
    return;
}

int main()
{
    int k = 1;
    while(useless == 1)
    {
        input_reader();
        k++;
    }
    return 0;
}