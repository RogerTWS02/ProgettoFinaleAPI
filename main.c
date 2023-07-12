// "Ex nihilo nihil fit"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char buffer[5200];
int input_placeholder;
uint8_t useless = 1;

// Struttura del nodo dell'albero delle stazioni
typedef struct stazione
{
    unsigned int distanza;
    unsigned int parco_macchine[513];
    struct stazione *padre, *figlio_dx, *figlio_sx;
} *stazione;

// Struttura del nodo della lista delle stazioni di un percorso
typedef struct stazione_percorso
{
    unsigned int distanza;
    struct stazione_percorso *next;
} *stazione_percorso;

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

// Aggiunge un nodo ad una lista
stazione_percorso aggiungi_in_lista(stazione_percorso ultimo_nodo, unsigned int valore)
{
    ultimo_nodo->next = malloc(sizeof(struct stazione_percorso));
    stazione_percorso temp = ultimo_nodo->next;
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

// Stampa gli elementi di una lista, data la testa
void stampa_percorso(stazione_percorso nodo, unsigned int stazione_arrivo)
{
    printf("%u", nodo->distanza);
    nodo = nodo->next;
    while (nodo != NULL)
    {
        printf(" %u", nodo->distanza);
        nodo = nodo->next;
    }
    printf(" %u\n", stazione_arrivo);
    return;
}

// AGGIUNGE AUTO DATA IN INPUT DA AGGIUNGI_STAZIONE
void aggiungi_auto_A(stazione current, unsigned int valore)
{
    int i = 0;
    while (current->parco_macchine[i] != 0 && current->parco_macchine[i] >= valore)
        i++;
    if (i == 0 && current->parco_macchine[i+1] == 0)
    {
        current->parco_macchine[0] = valore;
        current->parco_macchine[1] = 0;
        return;
    }
    if(current->parco_macchine[i] == 0)
    {
        current->parco_macchine[i] = valore;
        current->parco_macchine[i+1] = 0;
        return;
    }
    unsigned int temp = current->parco_macchine[i];
    while (valore != 0 && i < 512)
    {
        current->parco_macchine[i] = valore;
        valore = temp;
        temp = current->parco_macchine[i+1];
        i++;
    }
    if (i != 512)
        current->parco_macchine[i] = 0;
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
    curr->parco_macchine[0] = 0;
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
    int numero_stazioni = estrai_valore(buffer);
    while (i < numero_stazioni)
    {
        valore = estrai_valore(buffer);
        if (valore == 0)
        {
            curr->parco_macchine[512] += 1;
        }
        else
        {
            aggiungi_auto_A(curr, valore);
            i++;
        }
    }
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
    demolisci_stazione_B(temp);
    return;
}

// AGGIUNGE AUTO DATA IN INPUT DAL COMANDO DEDICATO
void aggiungi_auto_B()
{
    input_placeholder = 14;
    stazione current = ricerca_stazione(root_tree_stazioni, estrai_valore(buffer));
    if (current == NULL)
    {
        printf("non aggiunta\n");
        return;
    }
    unsigned int valore = estrai_valore(buffer);
    if (valore == 0)
    {
        current->parco_macchine[512] += 1;
        printf("aggiunta\n");
        return;
    }
    aggiungi_auto_A(current, valore);
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
    if (current->parco_macchine[0] == 0)
    {
        printf("non rottamata\n");
        return;
    }
    unsigned int valore = estrai_valore(buffer);
    if (valore == 0)
    {
        if (current->parco_macchine[512] > 0)
            current->parco_macchine[512] -= 1;
        else
            printf("non rottamata\n");
        return;
    }
    int i = 0;
    while (current->parco_macchine[i] != valore && i < 512)
        i++;
    if (i == 512)
    {
        printf("non rottamata\n");
        return;
    }
    if (i == 511)
    {
        current->parco_macchine[i] = 0;
        printf("rottamata\n");
        return;
    }
    valore = current->parco_macchine[i + 1];
    if (valore == 0)
    {
        current->parco_macchine[i] = 0;
        printf("rottamata\n");
    }
    else
    {
        while (valore != 0 && i < 512)
        {
            current->parco_macchine[i] = valore;
            valore = current->parco_macchine[i + 1];
            i++;
        }
        current->parco_macchine[i] = 0;
        printf("rottamata\n");
    }
    return;
}

// PIANFICA PERCORSO "IN AVANTI"
void pianifica_percorso_fwd(unsigned int valore_stazione_partenza, unsigned int valore_stazione_arrivo)
{
    /*stazione current = ricerca_stazione(root_tree_stazioni, valore_stazione_partenza);
    stazione stazione_arrivo = ricerca_stazione(root_tree_stazioni, valore_stazione_arrivo);    
    if(current == NULL || stazione_arrivo == NULL)
    {
        printf("nessun percorso\n");
        return;
    }
    if(current->parco_macchine[0] >= valore_stazione_arrivo - valore_stazione_partenza)
    {
        printf("%u %u", valore_stazione_partenza, valore_stazione_arrivo);
        return;
    }*/
    printf("PIANIFICA PERCORSO\n");
    return;
}

// PIANIFICA PERCORSO "AL CONTRARIO"
void pianifica_percorso_bwd(unsigned int valore_stazione_partenza, unsigned int valore_stazione_arrivo)
{
    /*stazione current = ricerca_stazione(root_tree_stazioni, valore_stazione_partenza);
    stazione stazione_arrivo = ricerca_stazione(root_tree_stazioni, valore_stazione_arrivo);    
    if(current == NULL || stazione_arrivo == NULL)
    {
        printf("nessun percorso\n");
        return;
    }
    if(current->parco_macchine[0] >= valore_stazione_partenza - valore_stazione_arrivo)
    {
        printf("%u %u", valore_stazione_partenza, valore_stazione_arrivo);
        return;
    }*/
    printf("PIANIFICA PERCORSO INVERSO\n");
    return;
}

// PIANIFICA PERCORSO
void pianifica_percorso()
{
    input_placeholder = 19;
    unsigned int stazione_partenza = estrai_valore(buffer);
    unsigned int stazione_arrivo = estrai_valore(buffer);
    if (stazione_partenza == stazione_arrivo)
    {
        printf("%u\n", stazione_partenza);
    }
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
        aggiungi_auto_B();
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
    int k = 0;
    while (useless == 1)
    {
        input_reader();
        k++;
    }
    return 0;
}