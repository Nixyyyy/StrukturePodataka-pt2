#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 10

typedef struct osoba* Pozicija;
typedef struct osoba {
	char ime[MAX_CHAR];
	char prezime[MAX_CHAR];
	int rodenje;
	Pozicija next;
}osoba;

int dodajOsobu(Pozicija, char* ime, char* prezime, int rodenje);
int ispis(Pozicija);
int dodajKraj(Pozicija, char* ime, char* prezime, int rodenje);
int nadiPrezime(Pozicija, char* prezime);
int BrisiOsobu(Pozicija, char* ime, char* prezime, int rodenje);
int main() {
	osoba head;
	strcpy(head.ime, ""); strcpy(head.prezime, "");head.rodenje = 0;head.next = NULL;
	
	
	
	dodajOsobu(&head, "Nikola", "Bareta", 04);
	ispis(head.next);
	dodajKraj(&head, "Nikola", "Kurt", 04);
	ispis(head.next);
	nadiPrezime(&head, "Bareta"); 

	BrisiOsobu(&head, "Nikola", "Bareta", 04);
	ispis(head.next);
	return 0;
}

int dodajOsobu(Pozicija P, char* ime, char* prezime, int rodenje) {
	Pozicija q;
	q = (Pozicija)malloc(sizeof(osoba));
	if (!q) {
		printf("alokacija nevalja");
		return -1;
	}
	strcpy(q->ime, ime);
	strcpy(q->prezime, prezime);
	q->rodenje = rodenje;
	q->next = P->next;
	P->next = q;
	return 0;
}

int ispis(Pozicija P) {
	if (P == NULL)
		printf("Prazna je lista");
	while (P != NULL) {
		printf("%s %s %d \n", P->ime, P->prezime, P->rodenje);
		P = P->next;
	}
	printf("\n");
	return 0;
}

int dodajKraj(Pozicija P, char* ime, char* prezime, int rodenje) {
	Pozicija q;
	q = (Pozicija)malloc(sizeof(osoba));
	if (!q) {
		printf("alokacija nevalja");
		return -1;
	}
	strcpy(q->ime, ime);
	strcpy(q->prezime, prezime);
	q->rodenje = rodenje;
	
	while (P->next != NULL) {
		P = P->next;
	}
	q->next = P->next;
	P->next = q;
	return 0;
}
int nadiPrezime(Pozicija P, char* prezime) {
	P = P->next;
	while (P != NULL) {
		if (strcmp(P->prezime, prezime) == 0) {
			printf("Nadena je osoba %s \n", P->prezime);
			return 0;
		}
		P = P->next;
	}
	if (P == NULL)
		printf("Osoba nije nadena\n");
	return 0;
}

int BrisiOsobu(Pozicija P, char* ime, char* prezime, int rodenje) {
	Pozicija temp = P->next;
	while (temp != NULL) {
		if (strcmp(temp->ime, ime) == 0 && strcmp(temp->prezime, prezime) == 0 && temp->rodenje == rodenje) {
			P->next = temp->next;
			printf("Osoba je izbrisana\n");
			free(temp);
			return 0;
		}
		temp = temp->next;
		P = P->next;
	}
	return 0;
}