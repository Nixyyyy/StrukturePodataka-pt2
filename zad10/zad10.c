#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50


typedef struct grad {
    char naziv[MAX];
    int broj_stanovnika;
    struct grad* lijevo;
    struct grad* desno;
    struct grad* sljedeci;
} Grad;

typedef struct drzava {
    char naziv[MAX];
    char datoteka[MAX];
    Grad* gradovi_stablo;
    Grad* gradovi_lista;
    struct drzava* lijevo;
    struct drzava* desno;
    struct drzava* sljedeca;
} Drzava;


Grad* stvori_grad(char* naziv, int broj) {
    Grad* novi = (Grad*)malloc(sizeof(Grad));
    strcpy(novi->naziv, naziv);
    novi->broj_stanovnika = broj;
    novi->lijevo = novi->desno = novi->sljedeci = NULL;
    return novi;
}

Drzava* stvori_drzavu(char* naziv, char* datoteka) {
    Drzava* nova = (Drzava*)malloc(sizeof(Drzava));
    strcpy(nova->naziv, naziv);
    strcpy(nova->datoteka, datoteka);
    nova->gradovi_stablo = NULL;
    nova->gradovi_lista = NULL;
    nova->lijevo = nova->desno = nova->sljedeca = NULL;
    return nova;
}


Grad* dodaj_grad_u_stablo(Grad* korijen, Grad* novi) {
    if (korijen == NULL)
        return novi;

    if (novi->broj_stanovnika < korijen->broj_stanovnika ||
        (novi->broj_stanovnika == korijen->broj_stanovnika &&
            strcmp(novi->naziv, korijen->naziv) < 0))
        korijen->lijevo = dodaj_grad_u_stablo(korijen->lijevo, novi);
    else
        korijen->desno = dodaj_grad_u_stablo(korijen->desno, novi);

    return korijen;
}


void dodaj_grad_u_listu(Grad** glava, Grad* novi) {
    if (*glava == NULL ||
        novi->broj_stanovnika < (*glava)->broj_stanovnika ||
        (novi->broj_stanovnika == (*glava)->broj_stanovnika &&
            strcmp(novi->naziv, (*glava)->naziv) < 0)) {

        novi->sljedeci = *glava;
        *glava = novi;
        return;
    }

    Grad* temp = *glava;
    while (temp->sljedeci != NULL &&
        (temp->sljedeci->broj_stanovnika < novi->broj_stanovnika ||
            (temp->sljedeci->broj_stanovnika == novi->broj_stanovnika &&
                strcmp(temp->sljedeci->naziv, novi->naziv) < 0))) {
        temp = temp->sljedeci;
    }

    novi->sljedeci = temp->sljedeci;
    temp->sljedeci = novi;
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

Drzava* dodaj_drzavu_u_stablo(Drzava* korijen, Drzava* nova) {
    if (korijen == NULL)
        return nova;

    if (strcmp(nova->naziv, korijen->naziv) < 0)
        korijen->lijevo = dodaj_drzavu_u_stablo(korijen->lijevo, nova);
    else
        korijen->desno = dodaj_drzavu_u_stablo(korijen->desno, nova);

    return korijen;
}


void ucitaj_gradove_stablo(char* ime_datoteke, Grad** korijen) {
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

void ucitaj_gradove_lista(char* ime_datoteke, Grad** glava) {
    FILE* f = fopen(ime_datoteke, "r");
    if (!f) return;

    char naziv[MAX];
    int broj;

    while (fscanf(f, " %[^,],%d", naziv, &broj) == 2) {
        Grad* novi = stvori_grad(naziv, broj);
        dodaj_grad_u_listu(glava, novi);
    }

    fclose(f);
}

void ispisi_gradove_stablo(Grad* korijen, int min) {
    if (korijen == NULL) return;

    ispisi_gradove_stablo(korijen->lijevo, min);
    if (korijen->broj_stanovnika > min)
        printf("   %s (%d)\n", korijen->naziv, korijen->broj_stanovnika);
    ispisi_gradove_stablo(korijen->desno, min);
}

void ispisi_gradove_lista(Grad* glava, int min) {
    while (glava != NULL) {
        if (glava->broj_stanovnika > min)
            printf("   %s (%d)\n", glava->naziv, glava->broj_stanovnika);
        glava = glava->sljedeci;
    }
}

Drzava* pronadi_drzavu_stablo(Drzava* korijen, char* naziv) {
    if (korijen == NULL) return NULL;

    if (strcmp(naziv, korijen->naziv) == 0)
        return korijen;
    if (strcmp(naziv, korijen->naziv) < 0)
        return pronadi_drzavu_stablo(korijen->lijevo, naziv);
    return pronadi_drzavu_stablo(korijen->desno, naziv);
}


int main() {
    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    Drzava* lista_drzava = NULL;
    Drzava* stablo_drzava = NULL;

    char naziv[MAX], datoteka[MAX];

    while (fscanf(f, "%s %s", naziv, datoteka) == 2) {
        Drzava* nova1 = stvori_drzavu(naziv, datoteka);
        Drzava* nova2 = stvori_drzavu(naziv, datoteka);

        ucitaj_gradove_stablo(datoteka, &nova1->gradovi_stablo);
        ucitaj_gradove_lista(datoteka, &nova2->gradovi_lista);

        dodaj_drzavu_u_listu(&lista_drzava, nova1);
        stablo_drzava = dodaj_drzavu_u_stablo(stablo_drzava, nova2);
    }

    fclose(f);

    char trazena[MAX];
    int min;

    printf("Unesite naziv drzave: ");
    scanf("%s", trazena);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &min);

    Drzava* pronadena = pronadi_drzavu_stablo(stablo_drzava, trazena);

    if (pronadena) {
        printf("Gradovi u drzavi %s:\n", trazena);
        ispisi_gradove_lista(pronadena->gradovi_lista, min);
    }
    else {
        printf("Drzava nije pronadena.\n");
    }

    return 0;
}