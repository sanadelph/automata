#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SNL 10 // maximum number of characters in a state's name
#define LSIZ 128
#define RSIZ 10

typedef struct transition {
    char c;
    char *newState;
    struct transition *nextPtr;
} Transition;

typedef struct state {
    char *sName;
    Transition *tList;
} State;



void addTransition(Transition **tList, char *newS, char ch) { // adds a new transition to the existing list, recursively
    if(*tList == NULL) {
        *tList = malloc(sizeof(Transition));
        if(tList == NULL) exit(1);

        (*tList)->newState = malloc(sizeof(char) * strlen(newS)); // allocate memory for new state's name
        if((*tList)->newState == NULL) exit(1);

        strcpy((*tList)->newState, newS); // initialize new transition
        (*tList)->c = ch;
        (*tList)->nextPtr = NULL;
    } else {
        addTransition(&((*tList)->nextPtr), newS, ch);
    }
}

State *move(State **automaton, int nStates, State *currState, char ch) { // returns a state if transition is found, NULL if no transition is found
    Transition *tList = currState->tList;
    if(tList == NULL) return NULL;

    while(tList != NULL) {
        if(tList->c == ch) { // found correct transition
            int i;
            for(i = 0; i < nStates; i++) {
                if(!strcmp(automaton[i]->sName, tList->newState)) { // find the correct state in the table
                    return automaton[i];
                }
            }
        }
        tList = tList->nextPtr; // update tList with the next transition in the list
    }

    return NULL;
}

int isValid(State **automaton, int nStates, char *str) { // evaluates the acceptance of the input string
    if(*automaton == NULL) {
        return 1;
    }

    State *st = automaton[0]; // initial state is the first state

    int i;
    for(i = 0; i < strlen(str); i++) {
        st = move(automaton, nStates, st, str[i]); // transition to the next state
        if(st == NULL) {
            return 0;
        }
    }

    if(st->sName[0] == 'F') { // if state name begins with 'F', it's a terminal state
        return 1;
    }

    return 0;
}

void main() {

    char stateName[SNL]; // to store current state's name
    char newState[SNL]; // to store newState's name for every transition
    char c; // to store character that leads to newState for every transition

    char str[100]; // to store string entered by user

    // read file line by line
    char line[RSIZ][LSIZ];
    FILE *fptr =  fopen("input.txt", "r");
    int i = 0;
    printf("\n\n Read the file and store the lines into an array :\n");
	printf("------------------------------------------------------\n");
    while(fgets(line[i], LSIZ, fptr))
	{
        line[i][strlen(line[i])] = '\0';
        i++;
    }

    fclose(fptr); // close file

    int nStates = i; //number of lines (states)
	printf("Number of states: %d\n", nStates);

    State **automaton = malloc(nStates*sizeof(State)); // create the automaton table
    if(automaton == NULL) exit(1);


	printf("\nThe content of the file are : \n");
    for(i = 0; i < nStates; ++i)
    {
        printf("%s\n", line[i]);
        automaton[i] = malloc(sizeof(State)); // allocate memory for new state
        if(automaton[i] == NULL) exit(1);

        char *token = strtok(line[i], ";"); // first token is the state name
        automaton[i]->sName = malloc(sizeof(char) * (strlen(token) + 1)); // allocate memory for state name
        if(automaton[i]->sName == NULL) exit(1);

        strcpy(automaton[i]->sName, token); // copy state name into the table
        automaton[i]->tList = NULL; // initialize current state transition table

        while(token = strtok(NULL, ";")) { // keep reading as long as there are transitions
            sscanf(token, "(%[^,],%c)", newState, &c);
            addTransition(&(automaton[i]->tList), newState, c); // add new transition to state's tList
        }

    }


    printf("\n\nPlease enter a string:\n");
    scanf("%100s", &str);     // get string from user
    if(isValid(automaton, nStates, str))
        // string accepted
        printf("\nAccepted: %s\n", str);
    else
        // string not accepted
        printf("\nNot accepted: %s\n", str);
}
