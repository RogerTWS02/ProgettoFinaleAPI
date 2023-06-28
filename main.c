#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char buffer[5200];
int input_placeholder;

//Struttura del nodo dell'albero delle stazioni
typedef struct stazione{
    unsigned int distanza;
    struct automobile *parco_macchine;
    struct stazione *padre, *figlio_dx, *figlio_sx;
} *stazione;

//Dichiarazione della radice dell'albero delle stazioni come variabile globale
stazione root_tree_stazioni = NULL;

//Struttura del nodo dell'albero delle automobili
typedef struct automobile{
    unsigned int autonomia, quantity_exc;
    struct automobili *padre, *figlio_dx, *figlio_sx;
} *automobile;

//Legge riga per riga da input redirection e le passa, una per una, a input_handler
void input_reader(){
    while(fgets(buffer, 5200, stdin))
        input_placeholder = 0;
        input_handler(&buffer);
    return;        
}

//Gestisce i comandi in input chiamando le funzioni giuste in base al comando iniettato
void input_handler(char *input){
    if(input[0]=='a'&&input[9]=='s'){
        aggiungi_stazione();
        return;}
    if(input[0]=='a'&&input[9]=='a'){
        aggiungi_auto_B();
        return;}
    if(input[0]=='d'){
        demolisci_stazione();
        return;};
    if(input[0]=='r'){
        rottama_auto();
        return;}
    if(input[0]=='p'){
        pianifica_percorso();
        return;}
}

//Legge la stringa tenendo conto del placeholder e restituisce i valori numerici necessari per eseguire i comandi
unsigned int estrai_valore(char *input){
    unsigned int valore = 0;
    char str[32];
    sscanf(input+input_placeholder, "%u", &valore);
    sprintf(str, "%u", valore);
    input_placeholder += strlen(str) + 1;
    return valore;
}

//Ricerca elemento per chiave nell'albero, fornita la radice (STAZIONI)
stazione ricerca_stazione(stazione root_tree_stazioni, unsigned int valore){
    if(root_tree_stazioni == NULL)
        return NULL;
    stazione curr = root_tree_stazioni;
    while(curr != NULL){
        if(curr->distanza == valore)
            return curr;
        if(curr->distanza < valore)
            curr = curr->figlio_dx;
        else
            curr = curr->figlio_sx;
    }
    return NULL;
}

//Ricerca elemento per chiave nell'albero, fornita la radice (AUTO)
automobile ricerca_automobile(stazione root_tree_automobili, unsigned int valore){
    if(root_tree_automobili == NULL)
        return NULL;
    automobile curr = root_tree_automobili;
    while(curr != NULL){
        if(curr->autonomia == valore)
            return curr;
        if(curr->autonomia < valore)
            curr = curr->figlio_dx;
        else
            curr = curr->figlio_sx;
    }
    return NULL;
}

//AGGIUNGE STAZIONE
void aggiungi_stazione(){
    int i = 0;
    input_placeholder = 18;
    unsigned int valore = estrai_valore(&buffer);
    stazione prec = NULL;
    stazione curr = root_tree_stazioni;
    while(curr != NULL){
        prec = curr;
        if(valore == curr->distanza){
            printf("non aggiunta\n");
            return;}
        if(valore < curr->distanza)
            curr = curr->figlio_sx;
        else
            curr = curr->figlio_dx;
    }
    curr = malloc(sizeof(struct stazione));
    curr->distanza = valore;
    curr->parco_macchine = NULL;
    curr->figlio_dx = NULL;
    curr->figlio_sx = NULL;
    if(prec != NULL){
        curr->padre = prec;
        if(valore < prec->distanza)
            prec->figlio_sx = curr;
        else
            prec->figlio_dx = curr;
    }
    else{
        curr->padre = NULL;
        root_tree_stazioni = curr;
    }
    printf("aggiunta\n");
    int numero_stazioni = estrai_valore(&buffer);
    while(i < numero_stazioni){
        valore = estrai_valore(&buffer);
        aggiungi_auto_A(curr, valore, 0);
        i++;
    }
    return;
}

//DEMOLISCE STAZIONE
void demolisci_stazione(){
    //TODO
}

//AGGIUNGE AUTO DATA IN INPUT DA AGGIUNGI_STAZIONE
void aggiungi_auto_A(stazione current, unsigned int valore, uint8_t trigger){
    automobile curr = current->parco_macchine;
    automobile prec = NULL;
    while(curr != NULL){
        prec = curr;
        if(curr->autonomia == valore){
            curr->quantity_exc++;
            return;}
        if(curr->autonomia < valore)
            curr = curr->figlio_dx;
        else
            curr = curr->figlio_sx;
    }
    curr = malloc(sizeof(struct automobile));
    curr->autonomia = valore;
    curr->quantity_exc = 0;
    curr->figlio_dx = NULL;
    curr->figlio_sx = NULL;
    if(prec != NULL){
        curr->padre = prec;
        if(valore < prec->autonomia)
            prec->figlio_sx = curr;
        else
            prec->figlio_dx = curr;
    }
    else{
        curr->padre = NULL;
        current->parco_macchine = curr;
    }
    if(trigger == 1)
        printf("aggiunta\n");
    return;
}

//AGGIUNGE AUTO DATA IN INPUT DAL COMANDO DEDICATO
void aggiungi_auto_B(){
    input_placeholder = 16;
    stazione stazione_destinazione = ricerca_stazione(root_tree_stazioni, estrai_valore(&buffer));
    if(stazione_destinazione == NULL){
        printf("non aggiunta\n");
        return;}
    aggiungi_auto_A(stazione_destinazione, estrai_valore(&buffer), 1);
    return;
}

//ROTTAMA AUTO
void rottama_auto(){
    //TODO
}

//PIANIFICA PERCORSO
void pianifica_percorso(){
    input_placeholder = 19;
    unsigned int stazione_partenza = estrai_valore(&buffer);
    unsigned int stazione_arrivo = estrai_valore(&buffer);
    if(stazione_partenza == stazione_arrivo){
        printf("%u\n", stazione_partenza);
    }
    else{
        if(stazione_partenza < stazione_arrivo)
            pianifica_percorso_fwd(stazione_partenza, stazione_arrivo);
        else
            pianifica_percorso_bwd(stazione_partenza, stazione_arrivo);
    }
    return;
}

//PIANFICA PERCORSO "IN AVANTI"
void pianifica_percorso_fwd(unsigned int stazione_partenza, unsigned int stazione_arrivo){
    //TODO
}

//PIANIFICA PERCORSO "AL CONTRARIO"
void pianifica_percorso_bwd(unsigned int stazione_partenza, unsigned int stazione_arrivo){
    //TODO
}


int main() {
    input_reader();
    return 0;
}