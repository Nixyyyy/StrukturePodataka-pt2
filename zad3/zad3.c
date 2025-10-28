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
int dodajIza(Pozicija, char* ime, char* prezime, int rodenje, char* Xime, char* Xprezime, int Xrodenje);
int dodajIspred(Pozicija P, char* ime, char* prezime, int rodenje, char* Xime, char* Xprezime, int Xrodenje);
int sortiraPrezimena(Pozicija);
int upisDat(Pozicija);
int citaDat(Pozicija);
int brisiListu(Pozicija);

int main() {
	osoba head;
	strcpy(head.ime, ""); strcpy(head.prezime, ""); head.rodenje = 0; head.next = NULL;

	dodajOsobu(&head, "Nikola", "Bareta", 04);
	ispis(head.next);
	dodajKraj(&head, "Nikola", "Kurt", 04);
	ispis(head.next);
	nadiPrezime(&head, "Bareta");

	BrisiOsobu(&head, "Nikola", "Bareta", 04);
	ispis(head.next);

	dodajKraj(&head, "Nikola", "Bareta", 04);
	dodajIza(&head, "Nikola", "Kurt", 04, "Slavko", "Kozina", 04);
	ispis(head.next);

	dodajIspred(&head, "Slavko", "Kozina", 04, "Ante", "Pirija", 04);
	ispis(head.next);

	sortiraPrezimena(&head);
	ispis(head.next);

	upisDat(head.next);

	citaDat(&head);
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
	if (P->next == NULL) {
		dodajOsobu(P, ime, prezime, rodenje);
		return 0;
	}
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
			temp->next = NULL;
			free(temp);
			return 0;
		}
		temp = temp->next;
		P = P->next;
	}
	return 0;
}

int dodajIza(Pozicija P, char* ime, char* prezime, int rodenje, char* Xime, char* Xprezime, int Xrodenje) {
	P = P->next;
	while (P != NULL) {
		if (strcmp(P->ime, ime) == 0 && strcmp(P->prezime, prezime) == 0 && P->rodenje == rodenje) {
			Pozicija q = (Pozicija)malloc(sizeof(osoba));
			if (!q) {
				printf("alokacija nevalja");
				return -1;
			}
			strcpy(q->ime, Xime);
			strcpy(q->prezime, Xprezime);
			q->rodenje = Xrodenje;

			q->next = P->next;
			P->next = q;

			return 0;
		}
		P = P->next;
	}
	if (P == NULL) {
		printf("Osoba %s %s %d nije nadena u listi\n", ime, prezime, rodenje);
	}
	return 0;
}

int dodajIspred(Pozicija P, char* ime, char* prezime, int rodenje, char* Xime, char* Xprezime, int Xrodenje) {
	Pozicija temp = P->next;
	while (temp != NULL) {
		if (strcmp(temp->ime, ime) == 0 && strcmp(temp->prezime, prezime) == 0 && temp->rodenje == rodenje) {
			Pozicija q = (Pozicija)malloc(sizeof(osoba));
			if (!q) {
				printf("alokacija nevalja");
				return -1;
			}
			strcpy(q->ime, Xime);
			strcpy(q->prezime, Xprezime);
			q->rodenje = Xrodenje;

			q->next = P->next;
			P->next = q;

			return 0;
		}
		temp = temp->next;
		P = P->next;
	}
	if (temp == NULL) {
		printf("Osoba %s %s %d nije nadena u listi\n", ime, prezime, rodenje);
	}
	return 0;
}

int sortiraPrezimena(Pozicija P) {
	Pozicija pret = P;
	Pozicija pocetna = P;
	Pozicija temp = P->next->next;
	P = P->next;
	while (temp != NULL) {
		if (strcmp(P->prezime, temp->prezime) > 0) {
			pret->next = P->next;
			P->next = temp->next;
			temp->next = P;

			pret = pocetna;
			P = pocetna->next;
			temp = P->next;
		}
		else {
			temp = temp->next;
			P = P->next;
			pret = pret->next;
		}
	}
	return 0;
}

int upisDat(Pozicija P) {
	FILE* fp = fopen("Osobe", "w");	
	if (!fp) {
		return -1;
	}
	
	while (P != NULL) {
		fprintf(fp, "%s %s %d\n" , P->ime ,P->prezime, P->rodenje);
		P = P->next;
	}
	fclose(fp);
	return 0;	
}

int brisiListu(Pozicija P) {
	Pozicija temp = NULL;
	while (P != NULL) {
		temp = P;
		P = P->next;
		free(temp);
	}
	return 0;
}

int citaDat(Pozicija P) {
	FILE* fp = fopen("Osobe", "r");
	if (!fp) {
		return -1;
	}
	char ime[MAX_CHAR], prezime[MAX_CHAR];
	int rodenje;
	brisiListu(P->next);
	P->next = NULL;
	while (fscanf(fp, "%s %s %d", ime, prezime, &rodenje) == 3) {
		dodajKraj(P, ime, prezime, rodenje);
	}	
	fclose(fp);
	return 0;
}
