//sequence.c
#include "sequence.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour gérer le N-gramme
typedef struct {
    const char *words[Lg_N_gramme]; // Tableau de mots du N-gramme
    int current_pos;                // Position de l'itérateur
    struct strhash_table *hash_table; // Table de hachage pour éviter les doublons
} Sequence;

static Sequence seq;


void sequence_initialize(struct strhash_table *ht) {
    for (int i = 0; i < Lg_N_gramme; i++) {
        seq.words[i] = NULL; // Initialise chaque mot à NULL
    }
    seq.current_pos = 0;
    seq.hash_table = ht; // Utilise la table de hachage fournie
}


// Initialise l'itérateur sur le premier mot du N-gramme
void sequence_itStart(void) {
    seq.current_pos = 0;
}

// Retourne le mot à la position actuelle de l'itérateur et avance l'itérateur
const char *sequence_itNext(void) {
    if (seq.current_pos < Lg_N_gramme && seq.words[seq.current_pos] != NULL) {
        return seq.words[seq.current_pos++];
    }
    return NULL; // Si on atteint la fin ou si le mot est NULL
}

// Vérifie si l'itérateur est à la fin du N-gramme
int sequence_itHasNext(void) {
    return (seq.current_pos < Lg_N_gramme && seq.words[seq.current_pos] != NULL);
}


// Ajoute un nouveau mot au N-gramme en utilisant la table de hachage
void sequence_addWord(const char *word, struct strhash_table *ht) {
    // Ajoute le mot dans la table de hachage si ce n'est pas un doublon
    const char *stored_word = strhash_wordAdd(ht, word);
    
    // Décale les mots pour faire de la place au nouveau mot
    for (int i = 0; i < Lg_N_gramme - 1; i++) {
        seq.words[i] = seq.words[i + 1];
    }
    seq.words[Lg_N_gramme - 1] = stored_word; // Ajoute le nouveau mot à la fin
}

// Retourne le prochain mot qui sera ajouté dans le N-gramme (dernier mot)
const char *sequence_nextWord(void) {
    return seq.words[Lg_N_gramme - 1];
}

// Avance le N-gramme pour intégrer le nouveau mot
void sequence_progress(void) {
    // Réinitialise l'itérateur pour repartir depuis le début lors du prochain parcours
    seq.current_pos = 0;
}


// Affiche le N-gramme actuel en séparant les mots par "/"
void sequence_print(void) {
    for (int i = 0; i < Lg_N_gramme; i++) {
        if (seq.words[i] != NULL) {
            printf("%s", seq.words[i]);
            if (i < Lg_N_gramme - 1) {
                printf(" / ");
            }
        }
    }
    printf("\n");
}

// Retourne le N-gramme sous forme de chaîne de caractères (pour les tests)
char *sequence_printInTab(void) {
    static char buffer[256]; // Taille arbitraire pour stocker la chaîne
    buffer[0] = '\0';
    for (int i = 0; i < Lg_N_gramme; i++) {
        if (seq.words[i] != NULL) {
            strcat(buffer, seq.words[i]);
            if (i < Lg_N_gramme - 1) {
                strcat(buffer, " ");
            }
        }
    }
    return buffer;
}


#include "sequence.h"
#include "hash.h"
#include <stdio.h>

int main() {
    struct strhash_table *ht = strhash_create(100);

    sequence_initialize(ht);

    sequence_addWord("Le", ht);
    sequence_addWord("chat", ht);
    sequence_addWord("mange", ht);

    printf("N-gramme actuel : ");
    sequence_print(); // Affiche "Le / chat / mange"

    sequence_addWord("poisson", ht);
    printf("Après ajout de 'poisson' : ");
    sequence_print(); // Affiche "chat / mange / poisson"

    strhash_free(ht); // Libère la table de hachage
    return 0;
}


// Plan de Test Unitaire pour sequence.c
// Nous allons créer un fichier test_sequence.c qui contiendra les tests unitaires pour chaque fonction du module sequence. Ce fichier comprendra des tests pour :

// L'initialisation du N-gramme.
// L'ajout de mots dans le N-gramme.
// Le parcours de la séquence avec l'itérateur.
// L'affichage du N-gramme (validation du contenu avec sequence_printInTab).
// Exemple de Code pour test_sequence.c
// Voici une implémentation de test_sequence.c avec des tests unitaires pour chaque fonction :
#include "sequence.h"
#include "hash.h"
#include <assert.h>
#include <stdio.h>

// Fonction principale pour exécuter les tests unitaires
int main() {
    // Création d'une table de hachage pour le test
    struct strhash_table *ht = strhash_create(100);
    assert(ht != NULL); // Vérifie que la table de hachage a été créée avec succès

    // Test de l'initialisation de la séquence
    sequence_initialize(ht);
    for (int i = 0; i < Lg_N_gramme; i++) {
        assert(sequence_itNext() == NULL); // Chaque position doit être NULL après initialisation
    }

    // Test de l'ajout de mots dans le N-gramme
    sequence_addWord("Le", ht);
    sequence_addWord("chat", ht);
    sequence_addWord("mange", ht);
    sequence_itStart(); // Initialise l'itérateur pour parcourir les mots

    // Vérification des mots ajoutés
    assert(sequence_itNext() != NULL && strcmp(sequence_itNext(), "Le") == 0);
    assert(sequence_itNext() != NULL && strcmp(sequence_itNext(), "chat") == 0);
    assert(sequence_itNext() != NULL && strcmp(sequence_itNext(), "mange") == 0);
    assert(sequence_itHasNext() == 0); // Vérifie qu'il n'y a plus de mots à parcourir

    // Test de la rotation du N-gramme après ajout d'un nouveau mot
    sequence_addWord("poisson", ht);
    sequence_itStart();
    assert(strcmp(sequence_itNext(), "chat") == 0);
    assert(strcmp(sequence_itNext(), "mange") == 0);
    assert(strcmp(sequence_itNext(), "poisson") == 0);

    // Test de la fonction sequence_nextWord
    assert(strcmp(sequence_nextWord(), "poisson") == 0); // "poisson" doit être le dernier mot ajouté

    // Test de sequence_progress
    sequence_progress();
    sequence_itStart(); // Réinitialisation de l'itérateur
    assert(strcmp(sequence_itNext(), "chat") == 0);  // Premier mot après progression
    assert(strcmp(sequence_itNext(), "mange") == 0); // Deuxième mot après progression
    assert(strcmp(sequence_itNext(), "poisson") == 0); // Troisième mot après progression

    // Test de sequence_printInTab (comparaison avec la chaîne attendue)
    const char *ngram_string = sequence_printInTab();
    assert(strcmp(ngram_string, "chat mange poisson") == 0); // Validation de la chaîne de caractères

    // Nettoyage de la table de hachage après les tests
    strhash_free(ht);

    printf("Tous les tests unitaires ont été réussis.\n");
    return 0;
}


//makefile modifier 
# Compilation du fichier de test
test_sequence: sequence.o hash_x86_64.o list_x86_64.o test_sequence.o
	gcc -o test_sequence sequence.o hash_x86_64.o list_x86_64.o test_sequence.o -Wall

# Règle pour exécuter les tests unitaires
run_test_sequence: test_sequence
	./test_sequence

# Nettoyage des fichiers objets et de l'exécutable de test
clean_test:
	rm -f test_sequence.o test_sequence

// Utilisation
// Pour exécuter le test unitaire :

// Compilez le test en utilisant make test_sequence.
// Exécutez le test avec make run_test_sequence.