#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

struct tnode {
    char *word;
    int ctr;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) {
        p = (struct tnode *) malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->ctr = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->ctr++;
    else if (cond < 0)
        p->left = addtree(p->left, w);
    else
        p->right = addtree(p->right, w);
    return p;
}

void treeprint(struct tnode *p)
{
    int i;
    if (p != NULL) {
        treeprint(p->left);
        printf("%2d %s\n", p->ctr, p->word);
        treeprint(p->right);
    }
}

int main(int argc, const char* argv[]){
    struct tnode *words = NULL;
    int wCtr = 0;
    char *word = strdup("");
    char c;
    while((c = fgetc(stdin)) != EOF){
        if(c == '\t' || c == ' ' || c == '\n'){
            words = addtree(words, word);
            word = strdup("");
            wCtr = 0;
            continue;
        }
        word[wCtr++] = c;
    }
    treeprint(words);
    return 0;
}