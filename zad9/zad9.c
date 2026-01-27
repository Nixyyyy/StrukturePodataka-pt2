#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct _node;
typedef struct _node* Position;
typedef struct _node {
	int value;
	Position left;
	Position right;
}Node;

int EmptyFile(char* fn);
int InsertRandomly(Position* root, int numOfNodes);
int Insert(Position* root, int value);
Position AddNode(int value);
int Replace(Position* root);
int SumaStabla(Position root, int* sum);
int PrintToFile(Position root, char* fn);
int PrintInorder(Position root, FILE* f);

int main()
{
	EmptyFile("print.txt");
	Position root = NULL;
	int num;
	srand(time(NULL));

	printf("Unesite broj elemenata stabla\n");
	scanf("%d", &num);

	InsertRandomly(&root, num);
	PrintToFile(root, "print.txt");

	Replace(&root);
	PrintToFile(root, "print.txt");
}

int EmptyFile(char* fn)
{
	FILE* f = fopen(fn, "w");
	fprintf(f, "");
	fclose(f);
	return EXIT_SUCCESS;
}

int InsertRandomly(Position* root, int numOfNodes)
{
	int i;
	for (i = 0; i < numOfNodes; i++)
	{
		Insert(root, (rand() % 81) + 10);
	}
}

int Insert(Position* root, int value)
{
	if (*root == NULL)
	{
		*root = AddNode(value);
	}
	else if ((*root)->left == NULL && value >= (*root)->value)
	{
		(*root)->left = AddNode(value);
	}
	else if ((*root)->right == NULL && value < (*root)->value)
	{
		(*root)->right = AddNode(value);
	}
	else if (value >= (*root)->value)
		Insert(&(*root)->left, value);
	else if (value < (*root)->value)
		Insert(&(*root)->right, value);

	return EXIT_SUCCESS;
}

Position AddNode(int value)
{
	Position newNode = (Position)malloc(sizeof(Node));
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

int Replace(Position* root)
{
	int sum = 0;
	(*root)->value = SumaStabla(*root, &sum);
	if ((*root)->left != NULL)
		Replace(&((*root)->left));
	if ((*root)->right != NULL)
		Replace(&((*root)->right));
	return EXIT_SUCCESS;
}

int SumaStabla(Position root, int* sum)
{
	if (root->left != NULL)
	{
		*sum += root->left->value;
		SumaStabla(root->left, sum);
	}
	if (root->right != NULL)
	{
		*sum += root->right->value;
		SumaStabla(root->right, sum);
	}
	return *sum;
}

int PrintToFile(Position root, char* fn)
{
	FILE* f = NULL;
	f = fopen(fn, "a");
	if (f == NULL)
	{
		printf("Unable to open file\n");
		return -1;
	}

	PrintInorder(root, f);
	fprintf(f, "\n");
	fclose(f);
}

int PrintInorder(Position root, FILE* f)
{
	if (root->left != NULL)
	{
		PrintInorder(root->left, f);
		fprintf(f, "%d ", root->value);

		if (root->right != NULL)
			PrintInorder(root->right, f);
		else return;
	}
	else
	{
		fprintf(f, "%d ", root->value);
		if (root->right != NULL)
			PrintInorder(root->right, f);
		else return;
	}
}