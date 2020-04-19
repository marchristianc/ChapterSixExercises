#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

bool isVariable(char* word){
    char *varTypes[] = {
        "int",
        "char",
        "long",
        "double",
        "float",
        "short",
        "struct",
        "void",
        "bool"
    };
    for(int i = 0; i < 9; i++){
        if(strcmp(word, varTypes[i]) == 0)
            return true;
    }
    return false;
}

int compare_function(const void * a, const void * b ) {
    const char *pa = *(const char**)a;
    const char *pb = *(const char**)b;
    return strcmp(pa,pb);
}

bool isGood(char* word, char* target, int len){
    if (strlen(word) > len && strncmp(word, target, len) == 0 &&
        strncmp(word, target, strlen(word)) != 0)
        return true; 
    return false;
}

int main(int argc, const char* argv[]){
    int wLen = 6;
    if(argc == 2)
        wLen = atoi(argv[1]);
    if(wLen < 0)
        wLen = 6;
    int ctr = 0;
    char *vars[2048];
    memset(vars, 0, sizeof(vars));

    bool inComment = false;
    bool inMultiComment = false;
    bool inDQuote = false;
    bool inSQuote = false;
    int wCtr = 0;
    char *word = strdup("");
    char c;
    while((c = fgetc(stdin)) != EOF){
        if(!inDQuote && (c == '\'' && !inSQuote)){
            inSQuote = true;
        }else if(!inDQuote && (c == '\'' && inSQuote)){
            inSQuote = false;
        }

        if(!inSQuote && (c == '"' && !inDQuote)){
            inDQuote = true;
        }else if(!inSQuote && (c == '"' && inDQuote)){
            inDQuote = false;
        }

        if(!inMultiComment && (!inComment && (c == '/' && ((c = fgetc(stdin)) == '/')))){
            inComment = true;
        }else if(!inMultiComment && (inComment && c == '\n')){
            inComment = false;
        }

        if(!inComment && (!inMultiComment && (c == '/' && ((c = fgetc(stdin)) == '*')))){
            inMultiComment = true;
        }else if(!inComment && (inMultiComment && (c == '*' && ((c = fgetc(stdin)) == '/')))){
            inMultiComment = false;
        }

        if(inMultiComment || inComment || inDQuote || inSQuote)
            continue;
        
        if(c == ' '){
            if(isVariable(word)){
                word = strdup("");
                wCtr = 0;
                while((c = fgetc(stdin)) == '*' || c == ' ');
                do{
                    word[wCtr++] = c;
                    c = fgetc(stdin);
                }while(c != ' ' && c != '\n' && c != ';'
                    && c != '(' && c != ')'
                    && c != '[' && c != ','
                    && c != '=');
                vars[ctr++] = word;
            }
            word = strdup("");
            wCtr = 0;
            continue;
        }
        if(c != '\t' && c != ' ' && c != '\n')
            word[wCtr++] = c;
    }
    qsort(&vars, ctr, sizeof(*vars), compare_function);
    for(int i = 0; i < ctr; i++){
        printf("%s:\n", vars[i]);
        for(int j = 0; j < ctr; j++){
            if(isGood(vars[i], vars[j], wLen))
                printf("\t%s\n", vars[j]);
        }
        printf("\n");
    }

    return 0;
}