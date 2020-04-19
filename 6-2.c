#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define YES 1
#define NO 0

typedef struct tnode tnode;
struct tnode {
	char* word;
	int match;
	tnode *left;
	tnode* right;
};

struct tnode* addtree(tnode*, char*, int, int*);
int compare(char*, tnode*, int, int*);
int getword(char*, int);
tnode* talloc(void);
void treeprint(tnode*);

tnode* addtree(tnode* p, char* w, int num, int* found) {
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = _strdup(w);
		p->match = *found;
		p->left = p->right = NULL;
	}
	else if ((cond = compare(w, p, num, found)) < 0) {
		p->left = addtree(p->left, w, num, found);
	}
	else if (cond > 0) {
		p->right = addtree(p->right, w, num, found);
	}
	return p;
}

int compare(char* s, tnode* p, int num, int* found) {
	int i; 
	char* t = p->word;

	for (i = 0; *s == *t; i++, s++, t++) {
		if (*s == '\0') {
			return 0;
		}
	}
	if (i >= num) {
		*found = YES;
		p->match = YES;
	}
	return *s - * t;
}

int getword(char* word, int lim) {
	int c, getch(void);
	void ungetch(int);
	char* w = word;

	while (isspace(c = getch()))
		;
	if (c != EOF) {
		*w++ = c;
	}
	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++) {
		if (!isalnum(*w = getch())) {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
}

tnode* talloc(void) {
	return (tnode*)malloc(sizeof(tnode));
}

void treeprint(tnode* p) {
	if (p != NULL) {
		treeprint(p->left);
		if (p->match) {
			printf("%s\n", p->word);
		}
		treeprint(p->right);
	}
}

int main(int argc, const char* argv[]) {
	tnode* root;
	char word[MAXWORD];
	int found = NO;
	int num;

	num = (--argc && (*++argv)[0] == '-') ? atoi(argv[0] + 1) : 6;
	root = NULL;
	while (getword(word, MAXWORD) != EOF) {
		if (isalpha(word[0]) && strlen(word) >= num) {
			root = addtree(root, word, num, &found);
		}
		found = NO;
	}
	treeprint(root);
	return 0;
}