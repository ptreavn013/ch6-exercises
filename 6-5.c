#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASHSIZE 101

static struct hash_node *hashtab[HASHSIZE];

typedef struct hash_node hash_node;
struct hash_node {
	hash_node * next;
	char* name;
	char* defn;
};

unsigned hash(char*);
void undef(char*);

unsigned hash(char* s) {
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++) {
		hashval = *s + 31 * hashval;
	}
	return hashval % HASHSIZE;
}

hash_node* hash_lookup(char* s) {
	hash_node* np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			return np;
		}
	}

	return NULL;
}

hash_node* hash_install(char* name, char* defn) {
	hash_node* np;
	unsigned hashval;

	if ((np = hash_lookup(name)) == NULL) {
		np = (struct hash_node *)malloc(sizeof(*np));
		if (np == NULL || (np->name = _strdup(name)) == NULL) {
			return NULL;
		}
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else {
		free((void*)np->defn);
	}
	if ((np->defn = _strdup(defn)) == NULL) {
		return NULL;
	}
	return np;
}

void print_defn(const char* s) {
	struct hash_node* p = hash_lookup(s);
	printf("%s: ", s);
	printf("%s\n", p ? p->defn : "not found");
}

void undef(char* s) {
	int h;
	hash_node* prev, *np;

	prev = NULL;
	h = hash(s);
	for (np = hashtab[h]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			break;
		}
		prev = np;
	}
	if (np != NULL) {
		if (prev == NULL) {
			hashtab[h] = np->next;
		}
		else {
			prev->next = np->next;
		}
		free((void*)np->name);
		free((void*)np->defn);
		free((void*)np);
	}
}

void test_hash_table() {
	printf("%s\n", "// ---------------------- TESTING HASH_TABLE ---------------------");
	hash_install("cat", "animal that likes fish");
	hash_install("dog", "animal that hates cats");
	hash_install("mouse", "animal eaten by cats");
	hash_install("virus", "pain in the neck");
	print_defn("cat");
	print_defn("pig");
	print_defn("dog");
	print_defn("pig");
	print_defn("mouse");
	print_defn("bacteria");
	print_defn("amoeba");
	print_defn("paramecium");
	print_defn("virus");
	printf("\nredefining cat...\n");
	printf("redefining virus...\n");
	hash_install("cat", "animal that likes fish and mice and birds");
	hash_install("virus", "VERY EXPENSIVE pain in the neck");
	print_defn("cat");
	print_defn("virus");
	printf("\n%s\n", "// ----------------END OF TESTING HASH_TABLE ---------------------");
}

int main(int argc, const char* argv[]) {
	test_hash_table();

	return 0;
}