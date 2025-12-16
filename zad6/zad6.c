#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -100
#define MAX_FILENAME 256
#define ARTLENGTH 1024

struct _article;
typedef struct _article* art;

typedef struct _article {
    char name[20];
    int quant;
    double price;
    art next;
} Article;

struct _receipt;
typedef struct _receipt* rec;

typedef struct _receipt {
    char fileName[MAX_FILENAME];
    int date;           // YYYYMMDD
    rec next;
    art articleHead;    
} Receipt;

static void trim_newline(char* s);

int CreateReceiptList(rec head, const char* receiptList);
rec CreateNewReceipt(rec head, int date, const char* fn);
int FindDate(const char* fn);

int CreateArticleList(rec r);
art CreateNewArticle(const char* artName, int artQuant, double artPrice, rec r);

int MergeDays(rec head);
void PrintReceipts(rec head);
int MergeArticles(rec r);

double MoneySpent(rec head);
int QuantityBought(rec head);

void FreeAll(rec head);

/* MAIN */
int main(void)
{
    double sumSpent;
    int quantBought;

    Receipt head = { .fileName = "", .date = 0, .next = NULL, .articleHead = NULL };

    if (CreateReceiptList(&head, "racuni.txt") == ERROR) {
        printf("Greska pri citanju racuni.txt ili racuna.\n");
        FreeAll(&head);
        return 1;
    }

    sumSpent = MoneySpent(&head);
    if (sumSpent != ERROR)
        printf("\nKroz odabrani period je na odabrani artikl potroseno %.2lf eura\n", sumSpent);

    quantBought = QuantityBought(&head);
    if (quantBought != ERROR)
        printf("\nKroz odabrani period je prodano %d odabranih artikala\n", quantBought);

    FreeAll(&head);
    return 0;
}

/* Helpers */
static void trim_newline(char* s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

// Kreira listu racuna iz racuni.txt 
int CreateReceiptList(rec head, const char* receiptList)
{
    FILE* rl = fopen(receiptList, "r");
    if (!rl) return ERROR;

    char line[MAX_FILENAME];

    while (fgets(line, (int)sizeof(line), rl)) {
        trim_newline(line);
        if (line[0] == '\0') continue; // preskoci prazne linije

        int datescan = FindDate(line);
        if (datescan == ERROR) continue;

        rec currentRec = CreateNewReceipt(head, datescan, line);
        if (!currentRec) {
            fclose(rl);
            return ERROR;
        }

        if (CreateArticleList(currentRec) == ERROR) {
            fclose(rl);
            return ERROR;
        }
    }

    fclose(rl);

    MergeDays(head);

    for (rec r = head->next; r != NULL; r = r->next)
        MergeArticles(r);

    PrintReceipts(head);
    return EXIT_SUCCESS;
}

// Ubacuje receipt sortirano po datumu 
rec CreateNewReceipt(rec head, int date, const char* fn)
{
    rec prev = head;

    while (prev->next != NULL) {
        if (date < prev->next->date) break;
        prev = prev->next;
    }

    rec newR = (rec)malloc(sizeof(Receipt));
    if (!newR) return NULL;

    newR->next = prev->next;
    prev->next = newR;

    newR->date = date;
    strncpy(newR->fileName, fn, MAX_FILENAME - 1);
    newR->fileName[MAX_FILENAME - 1] = '\0';

    newR->articleHead = (art)malloc(sizeof(Article));
    if (!newR->articleHead) {
        // rollback
        prev->next = newR->next;
        free(newR);
        return NULL;
    }

    newR->articleHead->price = 0.0;
    newR->articleHead->quant = 0;
    newR->articleHead->next = NULL;
    newR->articleHead->name[0] = '\0';

    return newR;
}

// Cita prvi red racuna i izvuce 8 znamenaka (YYYYMMDD)
int FindDate(const char* fn)
{
    FILE* f = fopen(fn, "r");
    if (!f) return ERROR;

    char line[256];
    if (!fgets(line, (int)sizeof(line), f)) {
        fclose(f);
        return ERROR;
    }
    fclose(f);

    int date = 0;
    int digits = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            date = date * 10 + (line[i] - '0');
            digits++;
            if (digits == 8) break;
        }
    }

    if (digits != 8) return ERROR;
    return date;
}

// Kreira listu artikala za zadani receipt 
int CreateArticleList(rec r)
{
    FILE* f = fopen(r->fileName, "r");
    if (!f) return ERROR;

    char line[ARTLENGTH];
    char tempArtName[20];
    int tempArtQuant;
    double tempArtPrice;

    // preskoci prvi red (datum)
    if (!fgets(line, (int)sizeof(line), f)) {
        fclose(f);
        return ERROR;
    }

    while (fgets(line, (int)sizeof(line), f)) {
        if (sscanf(line, " %19s %d %lf", tempArtName, &tempArtQuant, &tempArtPrice) == 3) {
            if (!CreateNewArticle(tempArtName, tempArtQuant, tempArtPrice, r)) {
                fclose(f);
                return ERROR;
            }
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}

art CreateNewArticle(const char* artName, int artQuant, double artPrice, rec r)
{
    art tail = r->articleHead;
    while (tail->next != NULL) tail = tail->next;

    art newA = (art)malloc(sizeof(Article));
    if (!newA) return NULL;

    newA->next = NULL;
    strncpy(newA->name, artName, sizeof(newA->name) - 1);
    newA->name[sizeof(newA->name) - 1] = '\0';
    newA->price = artPrice;
    newA->quant = artQuant;

    tail->next = newA;
    return newA;
}

/* Spaja racune istog datuma (spaja liste artikala i brise visak receipt cvor) */
int MergeDays(rec head)
{
    rec prev = head;

    while (prev->next && prev->next->next) {
        rec first = prev->next;
        rec second = first->next;

        if (first->date == second->date) {
            // nadji kraj artikala prvog
            art a = first->articleHead;
            while (a->next) a = a->next;

            a->next = second->articleHead->next;

            // izbaci drugi receipt
            first->next = second->next;

            free(second->articleHead); 
            free(second);
            // ne micemo prev - mozda ima vise istih zaredom
        }
        else {
            prev = prev->next;
        }
    }

    return EXIT_SUCCESS;
}

void PrintReceipts(rec head)
{
    rec r = head->next;
    while (r != NULL) {
        printf("%d\n", r->date);

        for (art a = r->articleHead->next; a != NULL; a = a->next) {
            printf("%s %d %.2lf\n", a->name, a->quant, a->price);
        }

        printf("\n");
        r = r->next;
    }
}

/* Spaja istoimene artikle unutar jednog receipt-a (zbraja kolicine) */
int MergeArticles(rec r)
{
    art a1 = r->articleHead->next;

    while (a1 != NULL) {
        art prev = a1;
        art a2 = a1->next;

        while (a2 != NULL) {
            if (strcmp(a1->name, a2->name) == 0) {
                a1->quant += a2->quant;
                prev->next = a2->next;
                free(a2);
                a2 = prev->next;
            }
            else {
                prev = a2;
                a2 = a2->next;
            }
        }

        a1 = a1->next;
    }

    return EXIT_SUCCESS;
}

double MoneySpent(rec head)
{
    int start, end, choice;
    double sum = 0.0;
    char odabir[20];

    printf("Unesite pocetni datum u formatu YYYYMMDD\n");
    if (scanf("%d", &start) != 1) return ERROR;

    printf("Unesite zavrsni datum u formatu YYYYMMDD\n");
    if (scanf("%d", &end) != 1) return ERROR;

    printf("Odaberite artikl\nJabuke - 1\nKruske - 2\nMeso - 3\n");
    if (scanf("%d", &choice) != 1) return ERROR;

    if (choice == 1) strcpy(odabir, "Jabuke");
    else if (choice == 2) strcpy(odabir, "Kruske");
    else if (choice == 3) strcpy(odabir, "Meso");
    else {
        printf("Nevaljani unos\n");
        return ERROR;
    }

    rec r = head->next;
    while (r && r->date < start) r = r->next;

    while (r && r->date <= end) {
        for (art a = r->articleHead->next; a; a = a->next) {
            if (strcmp(a->name, odabir) == 0) {
                sum += a->price * (double)a->quant;
            }
        }
        r = r->next;
    }

    return sum;
}

int QuantityBought(rec head)
{
    int start, end, choice, sum = 0;
    char odabir[20];

    printf("Unesite pocetni datum u formatu YYYYMMDD\n");
    if (scanf("%d", &start) != 1) return ERROR;

    printf("Unesite zavrsni datum u formatu YYYYMMDD\n");
    if (scanf("%d", &end) != 1) return ERROR;

    printf("Odaberite artikl\nJabuke - 1\nKruske - 2\nMeso - 3\n");
    if (scanf("%d", &choice) != 1) return ERROR;

    if (choice == 1) strcpy(odabir, "Jabuke");
    else if (choice == 2) strcpy(odabir, "Kruske");
    else if (choice == 3) strcpy(odabir, "Meso");
    else {
        printf("Nevaljani unos\n");
        return ERROR;
    }

    rec r = head->next;
    while (r && r->date < start) r = r->next;

    while (r && r->date <= end) {
        for (art a = r->articleHead->next; a; a = a->next) {
            if (strcmp(a->name, odabir) == 0) {
                sum += a->quant;
            }
        }
        r = r->next;
    }

    return sum;
}

/* Oslobadja svu memoriju */
void FreeAll(rec head)
{
    rec r = head->next;
    head->next = NULL;

    while (r) {
        rec nextR = r->next;

        if (r->articleHead) {
            art a = r->articleHead;
            while (a) {
                art nextA = a->next;
                free(a);
                a = nextA;
            }
        }

        free(r);
        r = nextR;
    }
}
