#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX 1024

int ReadRows(const char* filename);

typedef struct {
	char ime[10];
	char prezime[10];
	int bodovi;
}listastudenata;

int main()
{
	int numberOfRows = 0, i, maxbodova = 0;
	FILE* fp = NULL;

	listastudenata* list;

	numberOfRows = ReadRows("dat.txt");
	
	printf("Broj redova u datoteci je %d\n\n", numberOfRows);

	list = (listastudenata*)malloc(numberOfRows * sizeof(listastudenata));
	fp = fopen("dat.txt", "r");

	for (i = 0; i < numberOfRows; i++)
	{
		fscanf(fp, "%s %s %d", list[i].ime, list[i].prezime, &list[i].bodovi);
		if (list[i].bodovi > maxbodova)
			maxbodova = list[i].bodovi;
	}

	for (i = 0; i < numberOfRows; i++)
	{
		printf("%s %s %d %2.1f\n", list[i].ime, list[i].prezime, list[i].bodovi, (double)list[i].bodovi / (double)maxbodova * 100);
	}

	fclose(fp);
	free(list);

	return 0;
}

int ReadRows(const char* filename)
{
	int numberOfRows = 0;
	FILE* fp = NULL;
	char buffer[MAX] = { '\0' };

	fp = fopen(filename, "r");
	if (!fp)
	{
		perror("fopen");
		return -1;
	}
	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		numberOfRows++;
	}
	fclose(fp);
	return numberOfRows;
}
