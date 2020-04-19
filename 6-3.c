#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

struct tnode {
    char *word;
    int linenums[1000];
    int ctr;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w, int lineCtr)
{
    int cond;

    if (p == NULL) {
        p = (struct tnode *) malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->ctr = 0;
        p->linenums[p->ctr++] = lineCtr;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->linenums[p->ctr++] = lineCtr;
    else if (cond < 0)
        p->left = addtree(p->left, w, lineCtr);
    else
        p->right = addtree(p->right, w, lineCtr);
    return p;
}

void treeprint(struct tnode *p)
{
    int i;
    if (p != NULL) {
        treeprint(p->left);
        printf("\t%s : ", p->word);
        for (i = 0; i < p->ctr; i++)
            printf("%d, ", p->linenums[i] + 1);
        printf("\n");
        treeprint(p->right);
    }
}

bool isOK(char *word){
    char *noisewords[] = {
        "and",  
        "as", 
        "at",
        "because",
        "but",
        "for",
        "has",
        "if",
        "in",
        "into",
        "is",
        "of",
        "on",
        "or", 
        "so",
        "the",
        "to",
        "too",
        "with"
    };
    for(int i = 0; i < 19; i++)
        if(strcmp(noisewords[i], word) == 0)
            return true;
    return false;
}

int main(int argc, const char* argv[]){
    struct tnode *words = NULL;
    int lineCtr = 0;
    int wCtr = 0;
    char *word = strdup("");
    char c;
    while((c = fgetc(stdin)) != EOF){
        if(c == '\t' || c == ' ' || c == '\n'){
            if(!isOK(word))
                words = addtree(words, word, lineCtr);
            if(c == '\n')
                lineCtr++;
            word = strdup("");
            wCtr = 0;
            continue;
        }
        word[wCtr++] = c;
    }
    treeprint(words);
    return 0;
}