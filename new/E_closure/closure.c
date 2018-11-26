
#include <stdio.h>     
#include <string.h>     //REMEMBER ME WHILE I'M GONE 
#include <errno.h>
#include <stdlib.h>

FILE *file;
int  numberOfStates = 0;
int  flag = 0;
int  states[20];

int  j = 0;
int  i = 0;
int  k = 0;

char a[20];

int  transitions[4][5];
int  visited[10];

int  MAXSIZE = 8;       
int  stack[8];     
int  top = -1;            

int isempty()
{
   if(top == -1)
      return 1;
   else
      return 0;
}

int isfull()
{
   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int pop() 
{
   int data;

   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   }
   else
         printf("Could not retrieve data, Stack is empty.\n");
 }


int push(int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   }
   else 
      printf("Could not insert data, Stack is full.\n");
}

int IsVisited(int edge)
{
    for(int i = 0; i < 10; i++)
        if(visited[edge] == 1)
            return 1;

    return 0;
}

void epsilon_closure(int state)
{
    int  e_closure[10];


    for(int i = 0; i < 10; i++ )
    {   e_closure[i] = -1;
        visited[i] = 0;
    }

    push(state);
    visited[state] = 1;

    while(top != -1)
    {
        int u = pop();

        j = 1;
        while(j < 5)
        {
            //if there is an epsilon transition from the state 'u' to 'v'
            if(transitions[j][0] == u && transitions[j][4] != 232) //ASCII of Φ = 232
            {
                if(! IsVisited(transitions[j][4]))
                {
                    visited[transitions[j][4]] = 1;
                    push(transitions[j][4]);
                }

            }
            j++;
        }
    }

    j = 0;
    for(int edge = 0; edge < 10; edge++)
    {
        if(visited[edge] == 1)
            e_closure[j++] = edge;
    }
    printf("e-closure(%d) : ",state);
    for (i = 0; e_closure[i] != -1; ++i)
        printf("%d ", e_closure[i]);
    printf("\n");
} 

int main()
{
    file = fopen("nfa.txt","r");

    if (file == NULL) {
        perror("fopen");
        return -1;
    }


    //Reading the states 
    while(!feof(file))
    {
        fscanf(file,"%s",a);
        if(strcmp("//states",a) == 0)
            flag = 1;
        else if(strcmp("//input_symbols",a) == 0)
            break;
        if (flag == 1 && a[0] != '/')
        {
            states[i++] = a[1] - '0';

        }
        numberOfStates = i; 
    }

    //Display the states of the e-NFA
    printf("\nStates : \n");
    for(i = 0; i < numberOfStates; i++ )
    {   
        printf("q%d\n",states[i]);
    }

    i = 1;
    flag = 0;
    //Reading the transition table

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++)
        {
            transitions[i][j] = 232;
        }

    }   
    while(!feof(file))
    {
        fgets(a,100,file);
        if(a[0] == '/')
        {   
            flag = 1;
        }
        if(flag == 1 && a[0] != '/')
        {   
            j = 0;
            //found a way to store the transition table in a matrix
            if(a[3] == 'e')
                transitions[(a[1] - '0') + 1][4] = a[6] - '0';

            else    
                transitions[(a[1] - '0') + 1][(a[3] - '0') + 1] = a[6] - '0';
            if(a[3] != 'e')
                transitions[0][a[3] - '0' + 1] = a[3] - '0';        //input
            else
                transitions[0][4] = -1;     // epsilon input
            transitions[(a[1] - '0') + 1][0] = a[1] - '0'; //initial state

        }
    }
   /* printf("\nTransitions read\n");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++)
        {
            printf("%d\t",transitions[i][j]);
        }
        printf("\n"); //detected segmentation fault here
    }   */

    //Calling e-closure for all the states  
    for(k = 0; k < numberOfStates; k++)
    {
        epsilon_closure(states[k]);
    }

    return 0;
}