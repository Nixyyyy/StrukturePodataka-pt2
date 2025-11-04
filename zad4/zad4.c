#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct polynomial* Position;
typedef struct polynomial {
	int coeff, power;
	Position next;
}poly;

int readFile(Position, Position);
int sumPoly(Position, Position, Position);
int printResult(Position);
int multPoly(Position, Position, Position);
int main() {
	poly resultPoly = { .next = NULL, .coeff = 0, .power = 0 };
	poly firstPoly = { .next = NULL, .coeff = 0, .power = 0 };
	poly secondPoly = { .next = NULL, .coeff = 0, .power = 0 };

	if (readFile(&firstPoly, &secondPoly) < 0) {
		fprintf(stderr, "Greska: ne mogu procitati datoteku.\n");
		return 1;
	}

	//sumPoly(&firstPoly, &secondPoly, &resultPoly);

	multPoly(&firstPoly, &secondPoly, &resultPoly);

	printResult(&resultPoly);
	return 0;
}

int readFile(Position firstPoly, Position secondPoly) {
	FILE* fp = fopen("polinomi.txt", "r");
	if (!fp) { return -1; }
	int coeff, power, n;
	char buffer[1024];

	if (fgets(buffer, sizeof(buffer), fp)) {
		char* cur = buffer;

		Position tail = firstPoly;

		while (tail->next != NULL) {
			tail = tail->next;
		}

		while (sscanf(cur, "%d %d %n", &coeff, &power, &n) == 2) {
			Position q = (Position)malloc(sizeof(poly));
			if (!q) { return -1; }
			q->coeff = coeff;
			q->power = power;
			q->next = NULL;

			tail->next = q;
			tail = q;
			cur += n;
		}
	}
	else {
		fclose(fp);
		return 0;
	}

	if (fgets(buffer, sizeof(buffer), fp)) {
		char* cur = buffer;

		Position tail = secondPoly;

		while (tail->next != NULL) {
			tail = tail->next;
		}

		while (sscanf(cur, "%d %d %n", &coeff, &power, &n) == 2) {
			Position q = (Position)malloc(sizeof(poly));
			if (!q) return -1;
			q->coeff = coeff;
			q->power = power;
			q->next = NULL;

			tail->next = q;
			tail = q;
			cur += n;
		}
	}
	fclose(fp);
	return 0;
}

int sumPoly(Position firstPoly, Position secondPoly, Position resultPoly) {
	Position p1 = firstPoly->next;

	while (p1 != NULL) {
		int power = p1->power;
		int sumCoeff = p1->coeff;

		Position p2 = secondPoly->next;
		while (p2 != NULL) {
			if (p2->power == power) {
				sumCoeff += p2->coeff;
			}
			p2 = p2->next;
		}


		Position q = (Position)malloc(sizeof * q);
		if (!q) return -1;
		q->coeff = sumCoeff;
		q->power = power;
		q->next = NULL;

		Position tail = resultPoly;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = q;

		p1 = p1->next;
	}

	Position p2 = secondPoly->next;
	while (p2 != NULL) {
		int power = p2->power;
		int found = 0;

		Position r = resultPoly->next;
		while (r != NULL) {
			if (r->power == power) {
				found = 1;
				break;
			}
			r = r->next;
		}

		if (!found) {
			Position q = (Position)malloc(sizeof (poly));
			if (!q) return -1;
			q->coeff = p2->coeff;
			q->power = p2->power;
			q->next = NULL;

			Position tail = resultPoly;
			while (tail->next != NULL)
				tail = tail->next;
			tail->next = q;
		}
		p2 = p2->next;
	}

	return 0;
}

int printResult(Position P) {
	if (P->next == NULL) {
		printf("Lista je prazna \n");
	}
	P = P->next;
	while (P != NULL) {
		printf("%d %d ", P->coeff, P->power);
		P = P->next;
	}
	return 0;
}

int multPoly(Position firstPoly, Position secondPoly, Position resultPoly) {
	Position p1 = firstPoly->next;
	Position p2 = secondPoly->next;
	Position pocetna = resultPoly;
	while (p1 != NULL && p2 != NULL) {
		int prodCoeff = p1->coeff * p2->coeff;
		int prodPower = p1->power * p2->power;

		Position q = (Position)malloc(sizeof(poly));
		if (!q) return -1;
		q->coeff = prodCoeff;
		q->power = prodPower;
		q->next = NULL;

		Position r = resultPoly;
		while (resultPoly->next != NULL) {
			resultPoly = resultPoly->next;
		}
		resultPoly->next = q;
		resultPoly = r;
		p1 = p1->next;
		p2 = p2->next;
	}

	pocetna = pocetna->next;
	while (pocetna != NULL) {

		Position temp = pocetna->next;
		if (temp == NULL) { break; }

		if (temp->power == pocetna->power) {
			pocetna->coeff += temp->coeff;

			pocetna->next = temp->next;

			free(temp);
			temp = NULL;

			continue;
		}
		pocetna = pocetna->next;
	}
	return 0;
}

/*
* 
(4+3)*2 - infix
4 3 + 2 * - postfix

1)
td struct _postfix* Position;
td struct _postfix{
double number;
Position next;
}postfix;

head={.number = 0, .next=NULL;}

2)
ucitati dat
-pohraniti podatke
-zatvoriti dat

3) 
a)broj?
b)znak?
c)trash?

a) Push(Position head, double num);
-alocirati newEl 
newEl -> number = num;
newEl->next = NULL;
q=head;
newEl->next = q->next;
q->next = newEl;

b) 
-obavimo operaciju
rezultat

first = q->next;
second = first->next;

Pop(Position first, Position second)
q->next = second ->next;
first -> next = NULL;
free(first);
second -> next = NULL;
free(second);
Push(q, rezultat);
*/