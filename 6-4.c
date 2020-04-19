#include <stdio.h>
#include <ctype.h>

#define MAXWORD 100
#define NDISTINCT 1000

typedef struct tnode tnode;
struct tnode {
	char* word;
	int count;
	tnode* left;
	tnode* right;
};

struct tnode* addtree(tnode*, char*);
int getword(char*, int);
void sortlist(void);
tnode* talloc(void);
void treestore(tnode*);

tnode* list[NDISTINCT];
int ntn = 0;

tnode* addtree(tnode* p, char* w) {
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0) {
		p->count++;
	}
	else if (cond < 0) {
		p->left = addtree(p->left, w);
	}
	else {
		p->right = addtree(p->right, w);
	}
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

void sortlist(void) {
	int gap, i, j;
	tnode* temp;

	for (gap = ntn / 2; gap > 0; gap /= 2) {
		for (i = gap; i < ntn; i++) {
			for (j = i - gap; j >= 0; j -= gap) {
				if ((list[j]->count) >= (list[j + gap]->count)) {
					break;
				}
				temp = list[j];
				list[j] = list[j + gap];
				list[j + gap] = temp;
			}
		}
	}
}

tnode* talloc(void) {
	return (tnode*)malloc(sizeof(tnode));
}

void treestore(tnode* p) {
	if (p != NULL) {
		treestore(p->left);
		if (ntn < NDISTINCT) {
			list[ntn++] = p;
		}
		treestore(p->right);
	}
}

int main(int argc, const char* argv[]) {
	tnode* root;
	char word[MAXWORD];
	int i;

	root = NULL;
	while (getword(word, MAXWORD) != EOF) {
		if (isalpha(word[0])) {
			root = addtree(root, word);
		}
	}
	treestore(root);
	sortlist();
	for (i = 0; i < ntn; i++) {
		printf("%2d:%20s\n", list[i]->count, list[i]->word);
	}
	return 0;
}