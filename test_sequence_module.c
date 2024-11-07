#include <stdio.h>
#include "hash.h"
#include "sequence.h"

int main(void) {
    // Crée une table de hachage avec une taille de 100
    struct strhash_table *ht = strhash_create(100);
    if (ht == NULL) {
        fprintf(stderr, "Erreur lors de la création de la table de hachage\n");
        return 1;
    }

    // Initialise le N-gramme avec la table de hachage
    sequence_initialize(ht);

    // Ajoute des mots au N-gramme
    sequence_addWord("le", ht);
    sequence_addWord("chat", ht);
    sequence_addWord("mange", ht);

    // Affiche l'état actuel du N-gramme
    printf("N-gramme actuel : ");
    sequence_print(); // Doit afficher "le chat mange"

    // Ajoute un autre mot pour faire avancer le N-gramme
    sequence_addWord("poisson", ht);
    printf("Après ajout de 'poisson' : ");
    sequence_print(); // Doit afficher "chat mange poisson"

    // Libère la mémoire de la table de hachage
    strhash_free(ht);

    return 0;
}
