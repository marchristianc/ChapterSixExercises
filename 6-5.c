#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

struct hash_node {
    struct hash_node *next;
    char *name;
    char *defn;
} hash_node;

static struct hash_node *hashtab[HASHSIZE];

unsigned hash(char *s){
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct hash_node *lookup(char *s){
    struct hash_node *np;
    for(np = hashtab[hash(s)]; np != NULL; np = np->next)
        if(strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

struct hash_node *hash_install(char *name, char *defn){
    struct hash_node *np;
    unsigned hashval;
    
    if((np = lookup(name)) == NULL) {
        np = (struct hash_node *) malloc(sizeof(*np));
        if(np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }else{
        free((void *) np->defn);
    }
    if((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

void undef(char *name){
    struct hash_node *np;
    unsigned hashval;

    if((np = lookup(name)) != NULL){
        hashval = hash(name);
        free(hashtab[hashval]);
        free((void *) np->defn);
    }
}

void print_defn (char *name){
    struct hash_node *np;
    unsigned hashval;

    if((np = lookup(name)) != NULL){
        hashval = hash(name);
        np = hashtab[hashval];
        printf("%s\n", np->defn);
    }else{
        printf("%s not found\n", name);
    }
}

void hash_clear(){
    memset(hashtab, 0, sizeof(hashtab));
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
    
    hash_clear();
    printf("\n%s\n", "// ----------------END OF TESTING HASH_TABLE ---------------------");
}
//-----------------------------------------------------------
int main(int argc, const char * argv[]) {
    test_hash_table();
    return 0;
}
