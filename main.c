#include <stdio.h>

char buffer[5200];

//Struttura del nodo dell'albero delle stazioni
typedef struct stazione{
    unsigned int distanza;
    struct automobile *parco_macchine;
    struct stazione *figlio_dx, *figlio_sx;
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
        input_handler(*buffer);
    return;        
}

//Gestisce i comandi in input chiamando le funzioni giuste in base al comando iniettato
void input_handler(char *input){
    if(input[0]=='a'&&input[9]=='s')
        aggiungi_stazione(*input, root_tree_stazioni);
    if(input[0]=='a'&&input[9]=='a')
        aggiungi_auto(*input);
    if(input[0]=='d')
        demolisci_stazione(*input);
    if(input[0]=='r')
        rottama_auto(*input);
    if(input[0]=='p')
        pianifica_percorso(*input);
    return;
}

//AGGIUNGE STAZIONE
void aggiungi_stazione(char *input, stazione root_tree_stazioni){
    //TODO
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
    stazione root_tree_stazioni = NULL;
    input_reader();
    return 0;
}