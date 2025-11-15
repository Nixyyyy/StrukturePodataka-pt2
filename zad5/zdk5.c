#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALLOC_ISSUE -1
#define FILE_ISSUE -2
/*5. Napisati program koji iz datoteke ?ita postfiks izraz i zatim korištenjem stoga ra?una
rezultat. Stog je potrebno realizirati preko vezane liste*/

typedef struct Num* Position;

typedef struct Num {
	double elNum;
	Position next;
} Num;

int push(Position head, double value);// dodavanje elementa na po?etak liste
double pop(Position head); // uklanjanje elementa s pocetka liste
int getListFromFile(Position head); //ucitava postfiks izraz iz datoteke i racuna rezultat

int main() {
	Num head = { .elNum = 0, .next = NULL };

	getListFromFile(&head);

	printf("Rezultat postfiks izraza je: %.2f\n", pop(&head));
	return 0;
}

int getListFromFile(Position head) {
	FILE* file = fopen("postfiks.txt", "r");
	if (!file)
		return ALLOC_ISSUE;
	char buffer[40];
	fgets(buffer, sizeof(buffer), file); //u?itava postfiks izraz iz datoteke u buffer
	fclose(file);

	char* p = buffer;
	char current[8] = ""; //buffer za trenutni broj ili operator
	while (*p) {	
		int i = 0;
		while (*p != ' ' && *p != '\0') { //u?itaj cijeli broj/operator izmedju razmaka
			current[i++] = *p;
			p++;
		}
		for (i; i < (int)sizeof(current) - 1; i++) //isprazni ostatak buffera u slu?aju ostataka
			current[i] = '\0';

		if (*p == ' ') p++;       

		if (strcmp(current, "+") == 0) { //provjeri koji je operator ili broj
			double b = pop(head); double a = pop(head); //"pop-aj" zadanja dva broja sa stoga
			push(head, a + b);
		}
		else if (strcmp(current, "-") == 0) {
			double b = pop(head); double a = pop(head);
			push(head, a - b);
		}
		else if (strcmp(current, "*") == 0) {
			double b = pop(head); double a = pop(head);
			push(head, a * b);
		}
		else if (strcmp(current, "/") == 0) {
			double b = pop(head); double a = pop(head);
			push(head, a / b);
		}
		else if (current[0] != '\0') {  //ako je broj dodaj na stog
			push(head, atof(current));
		}
	}
	return 0;
}

int push(Position P, double value) {
	Position newNum = (Position)malloc(sizeof(Num));//dinami?ki alocira memoriju za novi element
	if (!newNum)
		return ALLOC_ISSUE;//provjera alokacije

	newNum->elNum = value;
	newNum->next = P->next; //novi el. pokazuje na prijašnje prvi
	P->next = newNum;//head pokazuje na novi el.

	return 0;
}

double pop(Position head) {
	if (head->next == NULL) {
		printf("Prazan stog!\n");
		return 0;
	}
	double x = 0.0;
	Position q = head->next;
	x = q->elNum;
	head->next = q->next;
	q->next = NULL;
	free(q); //oslobodi memoriju uklonjenog elementa
	return x; //vrati vrijednost uklonjenog elementa
}

