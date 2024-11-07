#compilation du fichier de teste 
test_sequence: sequence.o hash-macos-arm.o list-macos-arm.o test_sequence_module.o
	gcc -o test_sequence sequence.o hash-macos-arm.o list-macos-arm.o test_sequence_module.o -Wall

# Nom de l'exécutable
prog: hash-macos-arm.o list-macos-arm.o test_hash_exemple.o
	gcc -o prog hash-macos-arm.o list-macos-arm.o test_hash_exemple.o -Wall

# Compilation du fichier de test
test_hash_exemple.o: test_hash_exemple.c hash.h list.h sequence.h
	gcc -c test_hash_exemple.c -Wall

# Règle pour exécuter les tests unitaires
run_test_sequence: test_sequence
	./test_sequence

# Exécution du programme
run: prog
	./prog

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f prog test_hash_exemple.o
