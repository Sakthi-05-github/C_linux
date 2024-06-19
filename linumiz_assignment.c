#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#define MAX_STACK_SIZE 100
typedef struct
{
	char *paths[MAX_STACK_SIZE];
	int top;
}Stack;
void initStack(Stack *stack)
{
	stack->top = -1;
}
int isFULL(Stack *stack)
{
	return stack->top == MAX_STACK_SIZE - 1;
}
int isEmpty(Stack *stack)
{
	return stack->top == -1;
}
void pushd(Stack *stack,const char *path)
{
	if(isFULL(stack))
	{
		printf("Stack is full ,cannot push %s.\n",path);
		return;
	}
	//get the current directory
	char cwd[PATH_MAX];
	if(getcwd(cwd,sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return;
	}
	//Change to the new directory
	if(chdir(path) != 0)
	{
		perror("chdir()error");
		return;
	}
	//Save the old directory on the stack
	stack->top++;
	stack->paths[stack->top]=strdup(cwd);
	printf("Pushed: %s\n",path);
}
void popd(Stack *stack)
{
	if(isEmpty(stack))
	{
		printf("Stack is empty,cannot pop\n");
		return;
	}
	//change back to directory
	if(chdir(stack->paths[stack->top]) != 0)
	{
		perror("chdir()error");
		return;
	}
	printf("Popped: %s\n",stack->paths[stack->top]);
	free(stack->paths[stack->top]);
	stack->top--;
}
int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s<pushd|popd>[directory]\n",argv[0]);
		return 1;
	}
	static Stack directoryStack;
	static int stackInitialized = 0;
	if(!stackInitialized)
	{
		initStack(&directoryStack);
		stackInitialized = 1;
	}
	if(strcmp(argv[1],"pushd") == 0)
	{
		if(argc < 3)
		{
			fprintf(stderr,"Usage: %s pushd<directory>\n",argv[0]);
			return 1;
		}
		pushd(&directoryStack,argv[2]);
	}
	else if(strcmp(argv[1],"pop") == 0)
	{
		popd(&directoryStack);
	}
	else
	{
		fprintf(stderr,"Invalid command.Usage: %s<pushd|popd>[directory]\n",argv[0]);
		return 1;
	}
	return 0;
}


