#include <stdio.h>
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
    unsigned int autonomia;
    struct automobili *figlio_dx, *figlio_sx;
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
        aggiungi_stazione(&buffer);
        return;}
    if(input[0]=='a'&&input[9]=='a'){
        aggiungi_auto(&buffer);
        return;}
    if(input[0]=='d'){
        demolisci_stazione(&buffer);
        return;};
    if(input[0]=='r'){
        rottama_auto(&buffer);
        return;}
    if(input[0]=='p'){
        pianifica_percorso(&buffer);
        return;}
}

//Legge la stringa tenendo conto del placeholder e restituisce i valori numerici necessari per eseguire i comandi
unsigned int estrai_valore(char *input){
    unsigned int valore = 0;
    char str[25];
    sscanf(input+input_placeholder, "%u", &valore);
    sprintf(str, "%u", valore);
    input_placeholder += strlen(str) + 1;
    return valore;
}

//AGGIUNGE STAZIONE
void aggiungi_stazione(char *input){
    input_placeholder = 18;
    unsigned int valore_distanza = estrai_valore(&buffer);
    stazione prec = NULL;
    stazione curr = root_tree_stazioni;
    while(curr != NULL){
        prec = curr;
        if(valore_distanza == curr->distanza){
            printf("non aggiunta\n");
            return;}
        if(valore_distanza < curr->distanza)
            curr = curr->figlio_sx;
        else
            curr = curr->figlio_dx;
    }
    curr = malloc(sizeof(struct stazione));
    curr->distanza = valore_distanza;
    curr->parco_macchine = NULL;
    curr->figlio_dx = NULL;
    curr->figlio_sx = NULL;
    if(prec != NULL){
        curr->padre = prec;
        if(valore_distanza < prec->distanza)
            prec->figlio_sx = curr;
        else
            prec->figlio_dx = curr;
    }
    else{
        curr->padre = NULL;
        root_tree_stazioni = curr;
    }
    printf("aggiunta\n");
    return;
}

//DEMOLISCE STAZIONE
void demolisci_stazione(char *input){
    //TODO
}

//AGGIUNGE AUTO
void aggiungi_auto(char *input){
    //TODO
}

//ROTTAMA AUTO
void rottama_auto(char *input){
    //TODO
}

//PIANFIICA PERCORSO
void pianifica_percorso(char *input){
    //TODO
}

int main() {
    input_reader();
    return 0;
}