#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

typedef struct linklist linklist;
struct linklist {
	int lnum; 
	linklist* ptr;
};

typedef struct tnode tnode;
struct tnode {
	char* word;
	linklist* lines;
	tnode* left;
	tnode* right;
};

void addln(tnode*, int);
struct tnode* addtree(tnode*, char*, int);
int getword(char*, int);
linklist* lalloc(void);
int noiseword(char*);
tnode* talloc(void);
void treeprint(tnode*);

void addln(tnode* p, int linenum) {
	linklist* temp;

	temp = p->lines;
	while (temp->ptr != NULL && temp->lnum != linenum) {
		temp = temp->ptr;
	}
	if (temp->lnum != linenum) {
		temp->ptr = lalloc();
		temp->ptr->lnum = linenum;
		temp->ptr->ptr = NULL;
	}
}

tnode* addtree(tnode* p, char* w, int linenum) {
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = _strdup(w);
		p->lines = lalloc();
		p->lines->lnum = linenum;
		p->lines->ptr = NULL;
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0) {
		addln(p, linenum);
	}
	else if (cond < 0) {
		p->left = addtree(p->left, w, linenum);
	}
	else {
		p->right = addtree(p->right, w, linenum);
	}
	return p;
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

linklist* lalloc(void) {
	return (linklist*)malloc(sizeof(linklist));
}

int noiseword(char* w) {
	static char* nw[] = { "a", "an", "and", "are", "in", "is", "of", "or", "that", "the", "this", "to" };

	int cond, mid;
	int low = 0;
	int high = sizeof(nw) / sizeof(char*) - 1;

	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(w, nw[mid])) > 0) {
			high = mid - 1;
		}
		else if (cond > 0) {
			low = mid + 1;
		}
		else {
			return mid;
		}
	}
	return -1;
}

tnode* talloc(void) {
	return (tnode*)malloc(sizeof(tnode));
}

void treeprint(tnode* p) {
	linklist* temp;

	if (p != NULL) {
		treeprint(p->left);
		printf("%10s: ", p->word);
		for (temp = p->lines; temp != NULL; temp = temp->ptr) {
			printf("%4d ", temp->lnum);
		}
		printf("\n");
		treeprint(p->right);
	}
}

int main(int argc, const char* argv[]) {
	tnode* root;
	char word[MAXWORD];
	int linenum = 1;

	root = NULL;
	while (getword(word, MAXWORD) != EOF) {
		if (isalpha(word[0]) && noiseword(word) == -1) {
			root - addtree(root, word, linenum);
		}
		else if (word[0] == '\n') {
			linenum++;
		}
	}
	treeprint(root);
	return 0;
}