#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct _node;
typedef struct _node* Position;
typedef struct _node {
	int value;
	Position left;
	Position right;
}Node;

Position CreateNode(Position root, int value);
void UnosElemenata(Position root);
int InputValue(Position root, int num);
int PrintTree(Position node);
int Inorder(Position root);
int Preorder(Position root);
int Postorder(Position root);
int LevelOrder(Position root);
int SearchTreeDepth(Position root, int depth, int* maxDepth);
int PrintLevel(Position root, int maxDepth, int currentDepth);
Position FindValue(Position root, int value, int* prev);
int DeleteNode(Position root, int value);
int DeleteDoubleChild(Position node);
int FindSmallestValue(Position root, int minValue);

int main()
{
	Node root = { .value = NULL, .left = NULL, .right = NULL };
	int toDelete;

	UnosElemenata(&root);
	PrintTree(&root);

	printf("Koju vrijednost zelite ukloniti iz stabla?\n");
	scanf("%d", &toDelete);

	DeleteNode(&root, toDelete);
	PrintTree(&root);

	return 0;
}

Position CreateNode(Position root, int value)
{
	Position node = (Position)malloc(sizeof(Node));
	node = root;
	node->value = value;
	node->left = (Position)malloc(sizeof(Node));
	node->left->value = NULL;
	node->right = (Position)malloc(sizeof(Node));
	node->right->value = NULL;

	return node;
}

void UnosElemenata(Position root)
{
	int value, numel, i;
	printf("\nKoliko elemenata zelite unijeti u stablo?\n");
	scanf("%d", &numel);

	for (i = 0; i < numel; i++)
	{
		printf("\nUnesite vrijednost\n");
		scanf("%d", &value);
		printf("\n");

		InputValue(root, value);
	}
	return;
}

int InputValue(Position root, int num)
{
	if (root->value == NULL)
		root = CreateNode(root, num);
	else if (num < root->value)
		InputValue(root->left, num);
	else if (num > root->value)
		InputValue(root->right, num);

	return EXIT_SUCCESS;
}

int PrintTree(Position node)
{
	int choice;

	while (1)
	{
		printf("\nOdaberite nacin printanja stabla:\n1 - Inorder\n2 - Preoder\n3 - Postorder\n4 - Level order\n5 - Prestani printati\n\n");
		scanf("%d", &choice);
		printf("\n\n");

		switch (choice) {
		case 1:
			Inorder(node);
			printf("\n");
			break;
		case 2:
			Preorder(node);
			printf("\n");
			break;
		case 3:
			Postorder(node);
			printf("\n");
			break;
		case 4:
			LevelOrder(node);
			printf("\n");
			break;
		case 5:
		default:
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}

int Inorder(Position root)
{
	if (root->left->value != NULL)
	{
		Inorder(root->left);
		printf("%d ", root->value);

		if (root->right->value != NULL)
			Inorder(root->right);
		else return;
	}
	else
	{
		printf("%d ", root->value);
		if (root->right->value != NULL)
			Inorder(root->right);
		else return;
	}
}

int Preorder(Position root)
{
	printf("%d ", root->value);
	if (root->left->value != NULL)
		Preorder(root->left);
	if (root->right->value != NULL)
		Preorder(root->right);
	return;
}

int Postorder(Position root)
{
	if (root->left->value != NULL)
		Postorder(root->left);
	if (root->right->value != NULL)
		Postorder(root->right);
	printf("%d ", root->value);
	return;
}

int LevelOrder(Position root)
{
	int maxDepth = 0, i;
	SearchTreeDepth(root, 1, &maxDepth);
	printf("max depth: %d\n", maxDepth);

	for (i = 0; i < maxDepth; i++)
	{
		PrintLevel(root, maxDepth - i, 1);
		printf("\n");
	}
}

int SearchTreeDepth(Position root, int depth, int* maxDepth)
{
	int temp, deep = depth, maxdeep = *maxDepth;

	if (root->left->value != NULL)
	{
		temp = SearchTreeDepth(root->left, depth + 1, maxDepth);
		if (temp > *maxDepth)
			*maxDepth = temp;
		maxdeep = *maxDepth;
	}
	if (root->right->value != NULL)
	{
		temp = SearchTreeDepth(root->right, depth + 1, maxDepth);
		if (temp > *maxDepth)
			*maxDepth = temp;
		maxdeep = *maxDepth;
	}
	return depth;
}

int PrintLevel(Position root, int maxDepth, int currentDepth)
{
	if (currentDepth == maxDepth)
		printf("%d ", root->value);
	if (root->left->value != NULL)
		PrintLevel(root->left, maxDepth, currentDepth + 1);
	if (root->right->value != NULL)
		PrintLevel(root->right, maxDepth, currentDepth + 1);
	return;
}

Position FindValue(Position root, int value, int* prev)
{
	static Position foundValue;
	if (root->left->value == value)
	{
		foundValue = root->left;
		prev = root->value;
	}
	if (root->right->value == value)
	{
		foundValue = root->right;
	}
	if (root->left->value != NULL)
		FindValue(root->left, value, prev);
	if (root->right->value != NULL)
		FindValue(root->right, value, prev);
	return foundValue;
}

int DeleteNode(Position root, int value)
{
	int previous;
	Position temp = FindValue(root, value, &previous), temp2;

	if (temp->left->value == NULL && temp->right->value == NULL)
	{
		free(temp->left);
		temp->left = NULL;
		free(temp->right);
		temp->right = NULL;
		temp->value = NULL;
	}
	else if (temp->left->value != NULL && temp->right->value != NULL)
	{
		DeleteDoubleChild(temp);
	}
	else
	{
		if (root->left != NULL)
		{
			temp2 = temp->left;
			temp->value = temp2->value;
			temp->right = temp2->right;
			temp->left = temp2->left;
			free(temp2);
		}
		else
		{
			temp2 = temp->right;
			temp->value = temp2->value;
			temp->right = temp2->right;
			temp->left = temp2->left;
			free(temp2);
		}
	}
}

int DeleteDoubleChild(Position node)
{
	int* prev;
	int minValue = FindSmallestValue(node->right, node->right->value);
	node->value = minValue;
	DeleteNode(node, minValue);
}

int FindSmallestValue(Position root, int minValue)
{
	static int smallest;
	smallest = minValue;
	if (root->value < smallest)
		smallest = root->value;
	if (root->left->value != NULL)
		FindSmallestValue(root->left, smallest);
	if (root->right->value != NULL)
		FindSmallestValue(root->right, smallest);
	return smallest;
}