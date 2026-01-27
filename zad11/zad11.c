#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define VELICINA_TABLICE 11

typedef struct grad {
    char naziv[MAX];
    int broj_stanovnika;
    struct grad* lijevo;
    struct grad* desno;
} Grad;

typedef struct drzava {
    char naziv[MAX];
    char datoteka[MAX];
    Grad* gradovi_stablo;
    struct drzava* sljedeca;
} Drzava;


Grad* stvori_grad(char* naziv, int broj) {
    Grad* novi = (Grad*)malloc(sizeof(Grad));
    strcpy(novi->naziv, naziv);
    novi->broj_stanovnika = broj;
    novi->lijevo = novi->desno = NULL;
    return novi;
}

Drzava* stvori_drzavu(char* naziv, char* datoteka) {
    Drzava* nova = (Drzava*)malloc(sizeof(Drzava));
    strcpy(nova->naziv, naziv);
    strcpy(nova->datoteka, datoteka);
    nova->gradovi_stablo = NULL;
    nova->sljedeca = NULL;
    return nova;
}

int hash_funkcija(char* naziv) {
    int suma = 0;
    int i;

    for (i = 0; i < 5 && naziv[i] != '\0'; i++) {
        suma += (int)naziv[i];
    }

    return suma % VELICINA_TABLICE;
}


Grad* dodaj_grad_u_stablo(Grad* korijen, Grad* novi) {
    if (korijen == NULL)
        return novi;

    if (novi->broj_stanovnika < korijen->broj_stanovnika ||
        (novi->broj_stanovnika == korijen->broj_stanovnika &&
            strcmp(novi->naziv, korijen->naziv) < 0)) {

        korijen->lijevo = dodaj_grad_u_stablo(korijen->lijevo, novi);
    }
    else {
        korijen->desno = dodaj_grad_u_stablo(korijen->desno, novi);
    }

    return korijen;
}


void ucitaj_gradove(char* ime_datoteke, Grad** korijen) {
    FILE* f = fopen(ime_datoteke, "r");
    if (!f) return;

    char naziv[MAX];
    int broj;

    while (fscanf(f, " %[^,],%d", naziv, &broj) == 2) {
        Grad* novi = stvori_grad(naziv, broj);
        *korijen = dodaj_grad_u_stablo(*korijen, novi);
    }

    fclose(f);
}

void dodaj_drzavu_u_listu(Drzava** glava, Drzava* nova) {
    if (*glava == NULL || strcmp(nova->naziv, (*glava)->naziv) < 0) {
        nova->sljedeca = *glava;
        *glava = nova;
        return;
    }

    Drzava* temp = *glava;
    while (temp->sljedeca != NULL &&
        strcmp(temp->sljedeca->naziv, nova->naziv) < 0) {
        temp = temp->sljedeca;
    }

    nova->sljedeca = temp->sljedeca;
    temp->sljedeca = nova;
}


void ispisi_gradove(Grad* korijen, int min) {
    if (korijen == NULL) return;

    ispisi_gradove(korijen->lijevo, min);
    if (korijen->broj_stanovnika > min)
        printf("   %s (%d)\n", korijen->naziv, korijen->broj_stanovnika);
    ispisi_gradove(korijen->desno, min);
}


Drzava* pronadi_drzavu(Drzava* glava, char* naziv) {
    while (glava != NULL) {
        if (strcmp(glava->naziv, naziv) == 0)
            return glava;
        glava = glava->sljedeca;
    }
    return NULL;
}


int main() {
    Drzava* hash_tablica[VELICINA_TABLICE] = { NULL };

    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    char naziv[MAX], datoteka[MAX];

    while (fscanf(f, "%s %s", naziv, datoteka) == 2) {
        Drzava* nova = stvori_drzavu(naziv, datoteka);
        ucitaj_gradove(datoteka, &nova->gradovi_stablo);

        int index = hash_funkcija(naziv);
        dodaj_drzavu_u_listu(&hash_tablica[index], nova);
    }

    fclose(f);

    char trazena[MAX];
    int min;

    printf("Unesite naziv drzave: ");
    scanf("%s", trazena);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min);

    int index = hash_funkcija(trazena);
    Drzava* pronadena = pronadi_drzavu(hash_tablica[index], trazena);

    if (pronadena) {
        printf("Gradovi u drzavi %s:\n", trazena);
        ispisi_gradove(pronadena->gradovi_stablo, min);
    }
    else {
        printf("Drzava nije pronadena.\n");
    }

    return 0;
}
