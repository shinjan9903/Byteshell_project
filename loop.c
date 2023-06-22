#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>

#define extra 1024
#define func_size 7
#define extraword 64

// #include "./khash.h"

// KHA
// khash_t(map) *variables;


const char* breakd=" ";

char * builtins[func_size];


char ** split(char* line);
void add_hist(char **commands);
int my_history(char **commands);
char ** split(char* line);
int my_launch(char **commands);
int my_echo(char **commands);
int  cd_func(char **commands);
int my_execute(char **commands);
int builtin_call(char **commands, char *command);
int my_help(char **commands);
int my_exitf(char **commands);
int my_set(char **commands);
int my_result(char **commands);
void acm_loop();


#define MAX_MAP_SIZE 100

typedef struct {
    char* key;
    int value;
} KeyValuePair;

typedef struct {
    KeyValuePair pairs[MAX_MAP_SIZE];
    int size;
} Map;

void initMap(Map* map) {
    map->size = 0;
}

void insert(Map* map, char* key, int value) {
    if (map->size >= MAX_MAP_SIZE) {
        printf("Number of variables for this session exceeded.\n");
        return;
    }

    KeyValuePair* pair = &(map->pairs[map->size]);
    pair->key = strdup(key);
    pair->value = value;
    map->size++;
}

int get(const Map* map, char* key, int* value) {
    for (int i = 0; i < map->size; i++) {
        KeyValuePair* pair = &(map->pairs[i]);
        if (strcmp(pair->key, key) == 0) {
            *value = pair->value;
            return 1; 
        }
    }
    return 0; 
}
Map map;





typedef struct Command{
    char** args;
    int sz;
}Command;

typedef struct Stack {
    Command* elements[extra];
    int top;
} Stack;


Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = 0;
    return stack;
}

void push(Stack* stack,Command* command) {
    stack->top++;
    stack->elements[stack->top] = command;
}

Command* pop(Stack* stack) {
    Command* command=stack->elements[stack->top];
    stack->top--;
    return command;
}

Command* peek(Stack* stack) {
    return stack->elements[stack->top];
}

void EmptyStack(Stack* stack){
    free(stack);
}


int traverse(Stack* stack, int num){
    if(stack->top==0){
         printf("NO HISTORY!!");
        return 1;
    }
    int i=stack->top;
    i--;
    int c=0;
    printf(" Your history is as follows:\n ");
    for(;i>0;i--){
        c++;
        if(c>num) break;
        if((stack->elements[i])->sz==1){
            printf(" %s'\n'", ((stack->elements[i])->args)[0]);
            continue;
        }
        printf(" %s' '", ((stack->elements[i])->args)[0]);
        printf(" %s'\n'", ((stack->elements[i])->args)[1]);
    }
    return 0;
}

Stack* hist;


void add_hist(char **commands){
    Command * c=(Command *)malloc(sizeof(Command));
    c-> args=commands;
    if(commands[1]==NULL) c->sz=1;
    else c->sz=2;
     push(hist, c);
}

int my_history(char **commands){
    int x;
    if(commands[1]!=NULL)x=commands[1][0]-'0';
    else x=5;
    int sta=traverse(hist, x);

   return 1;
}

char** read_line(){
    int cont_size= 1024;
    int pos=0;
    char* container=malloc(sizeof(char)* cont_size);
    int temp;

    while(1){
        temp=getchar();
        if(temp=='\n'){
             container[pos]='\0';
             printf("%c ", container[pos]);
             char **args=split(container);
             return args;
        }
        else{
            container[pos]=temp;
            pos++;
        }
        if(pos>=cont_size){
            cont_size+=extra;
            container=realloc(container, cont_size);
            if(!container){
                fprintf(stderr, "ALLOCATION ERROR!!\n");
                exit(1);
            }
        }  

    }

}


char ** split(char* line){
    int word=64;
    int pos=0;
    char ** temp=malloc(word*sizeof(char *));
    char  *var;

    if(!temp){
        fprintf(stderr, "ALLOCATION ERROR!!\n");
        exit(1);
    }
    var=strtok(line, breakd);
    while(var!=NULL){
        temp[pos]=var;
        pos++;
        if(pos>=word){
            word+=extraword;
            temp=realloc(temp, word*sizeof(char *));
            if(!temp){
                fprintf(stderr, "ALLOCATION ERROR!!\n");
                exit(1);
            }
        }
        var=strtok(NULL, breakd);
    }
    temp[pos]=NULL;
    return temp;

}

int my_launch(char **commands){
     pid_t pid;
     int valid;
     pid=fork();
     if(pid==0){
        if(execvp(commands[0], commands)==-1){
           perror("Shinjanshell");
        }
        exit(1);
     }
     else{
         do{
           waitpid(pid, &valid, WUNTRACED);   
         } while(!WIFEXITED(valid)&&!WIFSIGNALED(valid));
     }
     return 1;
}


int  cd_func(char **commands){
    if(commands[1]==NULL){
        fprintf(stderr, "Byteshell: expected argument to \"cd\"\n");
    }
    else{
        if(chdir(commands[1])!=0){
            perror("ByteShell");
        }
    }
    return 1;
}

char* concatenateStrings(char** strings) {
    size_t totalLength=0;
    int i=0;
    while(strings[i]!=NULL){
        totalLength+=strlen(strings[i]);
        i++;
    }
    totalLength+=i-1;
    char* result=(char*)malloc((totalLength+1)*sizeof(char));
    if(result==NULL){
        fprintf(stderr,"Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    result[0]='\0';
    for(i=0;strings[i]!=NULL;i++){
        strcat(result, strings[i]);
        if(strings[i+1]!=NULL){
            strcat(result, " ");
        }
    }
    return result;
}

int my_execute(char **commands){
    if(commands[0]==NULL){
        return 1;
    }
    for(int i=0;i<func_size;i++){
       if(strcmp(commands[0], builtins[i])==0){
            return builtin_call(commands, builtins[i]);
       }
    }
    return my_launch(commands);
}

int my_echo(char **commands){
    char * temp_com=concatenateStrings(commands);
    int st=0;
    for(int i = 0; i < strlen(temp_com); i++) {
        st++;
        if(temp_com[i]=='"') break;
        //  printf("%c", full_name[i]); 
    } 
    for(int i = st; i < strlen(temp_com)-1; i++) {
         printf("%c", temp_com[i]); 
    } 
    printf("\n");
    return 1;
}


int builtin_call(char **commands, char *command){
    int x;
    if(command=="help"){
        x=my_help(commands);
    }
    else if(command=="exit"){
        x=my_exitf(commands);
    }
    else if(command=="cd"){
        x=cd_func(commands);
    }
    else if(command=="history"){
       x=my_history(commands);
    }
    else if(command=="echo"){
        x=my_echo(commands);
    }
     else if(command=="set"){
        x=my_set(commands);
    }
     else if(command=="result"){
        x=my_result(commands);
    }
    return x;

}

int my_help(char **commands){
   printf(" Following are builtin:\n ");
   for(int i=0; i<func_size;i++){
      if(i==5){
          printf(" %s :", builtins[i]);
           printf("(format) set var1 = value\n");
           continue;
      }
      else if(i==6){
         printf(" %s :", builtins[i]);
           printf("(format) result var1 + var2\n");
           continue;
      }
       printf(" %s\n", builtins[i]);
   }
   return 1;
}

int my_exitf(char **commands){
    exit(0);
    return 1;
}

int my_set(char **commands){
     if(commands[1]==NULL||commands[2]==NULL||commands[3]==NULL){
        fprintf(stderr, "Byteshell: expected argument to \"set\"\n");
    }
    char *var=commands[1];
    int value= atoi(commands[3]);
    insert(&map, var, value);
    //  printf("%d \n ", value);
   
    return 1;

}

int my_result(char **commands){
    if(commands[1]==NULL||commands[2]==NULL||commands[3]==NULL){
        fprintf(stderr, "Byteshell: expected argument to \"set\"\n");
    }
    char *var1=commands[1];
    char *var2=commands[3];
    int value1; int value2;
    if (get(&map, var1, &value1)) {
    
    } else {
        printf("No such variable.\n");
    }
    if (get(&map, var2, &value2)) {
    
    } else {
        printf("No such variable.\n");
    }
    

    
     
    //  printf("%s", commands[2]);
    //  printf("%i", value1);
    //  printf("%i", value2);


     if(strcmp(commands[2],"+")==0){
        value1=value1+value2;
        // print()
     }
     else if(strcmp(commands[2],"-")==0){
        value1=value1-value2;
     }
     else if(strcmp(commands[2],"*")==0){
         value1=value1*value2;
     }
     else if(strcmp(commands[2],"/")==0){
         value1=value1/value2;
     }
     else{
         printf("This operation is not supported:\n");
         return 1;
     }
     printf("%d \n ", value1);
    return 1;
}
void acm_loop(){
    char *line;
    char **args;
    printf("TerminalShinjan ");
    int valid;
    do{
        printf("-> ");
        args=read_line();
        add_hist(args);
        valid=my_execute(args);
        // free(line);
        // free(args);
    }
    while(valid);

}




int main(){
    initMap(&map);
    hist=(Stack *)malloc(sizeof(Stack));
   builtins[0]="help";
   builtins[1]="exit";
   builtins[2]="cd";
   builtins[3]="history";
   builtins[4]="echo";
   builtins[5]="set";
   builtins[6]="result";

   acm_loop();


}