#include <stdio.h>

char buffer[5200];

//Legge riga per riga da input redirection e le passa, una per una, a input_handler
void input_reader(){
    while(fgets(buffer, 5200, stdin))
        input_handler(*buffer);
    return;        
}

//Gestisce i comandi in input chiamando le funzioni giuste in base al comando iniettato
void input_handler(char *input){
    if(input[0]=='a'&&input[9]=='s')
        aggiungi_stazione(*input);
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
void aggiungi_stazione(char *input){
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
    input_reader();
    return 0;
}