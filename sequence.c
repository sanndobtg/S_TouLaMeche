#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "sequence.h"


//Structure pour gerer le N-gramme

typedef struct{
    //tableau du mot de N-gramme 
    const char* words[Lg_N_gramme];
    //position de l'iterateur
     int current_add_pos;           
    int current_iter_pos; 
    //table de hachage pour eviter les doublons 
    struct strhash_table* hash_table;
}sequence;

static sequence seq;

//initialiser le N-gramme avec les mots vides
void sequence_initialize(struct strhash_table *ht) {
    for (int i = 0; i < Lg_N_gramme; i++) {
        seq.words[i] = NULL;
    }
    seq.current_add_pos = 0;
    seq.current_iter_pos = 0;
    seq.hash_table = ht;
}


//Gestion de l'iterateur permettant de parcourir le N-gramme 
//Initialiser l'iterateur sur le premier mot du N-gramme courrant 
void sequence_itStart(void) {
    seq.current_iter_pos = seq.current_add_pos % Lg_N_gramme;
}


//Retourne le mot correspondant a la position courrante de l'iterateur 
//puis avance la position de l'iterateur d'une case du tableau 
const char *sequence_itNext(void) {
    if (seq.words[seq.current_iter_pos] != NULL) {
        const char *word = seq.words[seq.current_iter_pos];
        seq.current_iter_pos = (seq.current_iter_pos + 1) % Lg_N_gramme;
        return word;
    }
    return NULL;
}


// Vérifie si l'itérateur est à la fin du N-gramme
int sequence_itHasNext(void) {
    return seq.words[seq.current_iter_pos] != NULL;
}


//-------------------------------------------------------------------------------
//Nouveau mot de char fin du prochain N-gramme 
//definit le nouveau mot qui entrera dans le N-gramme 
void sequence_addWord(const char *word, struct strhash_table *ht) {
    // Ajoute le mot dans la table de hachage si ce n'est pas un doublon
    const char *stored_word = strhash_wordAdd(ht, word);
    
    // Place le mot dans le tableau à la position actuelle
    seq.words[seq.current_add_pos] = stored_word;

    // Avance la position d'ajout de manière circulaire
    seq.current_add_pos = (seq.current_add_pos + 1) % Lg_N_gramme;
}


//returne le nouveau mot qui entrera dans le N-gramme 
//sanndo: ou je retourne le mot se trouvant a la position actuelle
const char *sequence_nextWord(void) {
    int pos = (seq.current_add_pos - 1 + Lg_N_gramme) % Lg_N_gramme;
    return seq.words[pos];
}

//avance le N-gramme pour integrer le nouveau mot ecrit
void sequence_progress(void) {
    // Avance `current_add_pos` de manière circulaire pour préparer la position pour le prochain mot
    seq.current_add_pos = (seq.current_add_pos + 1) % Lg_N_gramme;
}


//-----------------------------------------------------------------------------------------------
//Debug 
//affiche le N-gamme courrant, les mots sont separer par des '/'
void sequence_print(void) {
    for (int i = 0; i < Lg_N_gramme; i++) {
        int pos = (seq.current_add_pos + i) % Lg_N_gramme;
        if (seq.words[pos] != NULL) {
            printf("%s ", seq.words[pos]);
            if (i < Lg_N_gramme - 1) {
                printf("/");
            }
        }
    }
    printf("\n");
}

//sequence sous forme d'une chaine pour le teste
char * sequence_printInTab( void )
{
    static char buffer[256]; //taille arbitraire pour stocker la chaine
    buffer[0]  ='\0';
    for(int i=0; i< Lg_N_gramme ; i++)
    {
        if(seq.words[i] != NULL)
        {
            strcat(buffer, seq.words[i]);
            if(i < Lg_N_gramme -1 )
            {
                strcat(buffer," ");
            }
        }
    }
    return buffer;
}

