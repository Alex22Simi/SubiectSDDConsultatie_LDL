#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Consultatie
{
	char* data;
	char* nume;
	char* specialitate;
	char* diagnostic;
	int pret;
}Consultatie;
typedef struct NodLs
{
	Consultatie* info;
	struct NodLs* next;
}NodLs;
typedef struct NodLp
{
	NodLs* info;
	struct NodLp* next;
	char* specialitate;
}NodLp;
Consultatie* initializareConsultatie(const char* data, const char* nume, const char* specialitate, const char* diagnostic, int pret)
{
	Consultatie* consultatie = (Consultatie*)malloc(sizeof(Consultatie));
	consultatie->data = malloc(strlen(data) + 1);
	strcpy(consultatie->data, data);
	consultatie->nume = malloc(strlen(nume) + 1);
	strcpy(consultatie->nume, nume);
	consultatie->specialitate = malloc(strlen(specialitate) + 1);
	strcpy(consultatie->specialitate, specialitate);
	consultatie->diagnostic = malloc(strlen(diagnostic) + 1);
	strcpy(consultatie->diagnostic, diagnostic);
	consultatie->pret = pret;
	return consultatie;
}
NodLs* initializareNodLs(Consultatie* c)
{
	NodLs* nodNou = (NodLs*)malloc(sizeof(NodLs));
	nodNou->info = c;
	nodNou->next = NULL;
	return nodNou;
}
NodLp* initializareNodLp(const char* specialitate, NodLs* nodLs)
{
	NodLp* nodNou = (NodLp*)malloc(sizeof(NodLp));
	nodNou->specialitate = malloc(strlen(specialitate) + 1);
	strcpy(nodNou->specialitate, specialitate);
	nodNou->info = nodLs;
	nodNou->next = NULL;
	return nodNou;
}
NodLs* inserareInListaSecundara(NodLs* cap, Consultatie* consultatie)
{
	NodLs* nodNou = initializareNodLs(consultatie);
	if (cap == NULL)
	{
		return nodNou;
	}
	else
	{
		NodLs* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;

		}
		temp->next = nodNou;

	}
	return cap;
}
NodLp* inserareInListaPrincipala(NodLp* cap,const char* specialitate, NodLs* nodLs)
{
	NodLp* nodNou = initializareNodLp(specialitate, nodLs);
	if (cap == NULL)
	{
		return nodNou;
	}
	else
	{
		NodLs* temp = cap;
		while (temp->next != NULL)
		{
			temp = temp->next;

		}
		temp->next = nodNou;

	}
	return cap;
}
void afisareLs(NodLs* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)
		{
			printf("%s %s %s %s %d\n", cap->info->data, cap->info->nume, cap->info->specialitate,
				cap->info->diagnostic,cap->info->pret);
			cap = cap->next;
		}
	}
}
void afisareLp(NodLp* cap)
{
	if (cap != NULL)
	{
		while (cap != NULL)
		{
			printf("Specialitate: %s\n", cap->specialitate);
			afisareLs(cap->info);
			cap = cap->next;
		}
	}
}
void dezalocareLs(NodLs* cap)
{
	NodLs* temp = cap;
	while (temp != NULL)
	{
		NodLs* nodDeSters = temp;
		temp = temp->next;
		free(temp->info->data);
		free(temp->info->nume);
		free(temp->info->specialitate);
		free(temp->info->diagnostic);
		free(temp->info);
		free(temp);
		temp = nodDeSters;
	}
}
void dezalocareLp(NodLp* cap)
{
	NodLp* temp = cap;
	if (temp != NULL)
	{
		while (temp != NULL)
		{
			NodLp* temp2 = temp;
			temp = temp->next;
			dezalocareLs(temp->info);
			free(temp->specialitate);
			free(temp);
			temp = temp2;

		}
	}
}
int valTotalaSpecialitate(NodLp* cap, const char* specialitate)
{
	NodLp* temp = cap;
	while (temp != NULL && strcmp(temp->specialitate, specialitate) != 0)
	{
		
			temp = temp->next;
			
	}
	if (temp == NULL)
	{
		return 0;
	}
	int total = 0;
	NodLs* tempLs = temp->info;
	while (tempLs != NULL)
	{
		total += tempLs->info->pret;
		tempLs = tempLs->next;
	}
	return total;
}
void modificarePret(NodLp* cap, const char* data, int pretNou)
{
	NodLp* temp = cap;
	while (temp != NULL)
	{
		NodLs* tempLs = temp->info;//parcurgere lista secundare din lista principala
		while (tempLs != NULL)
		{
			if (strcmp(tempLs->info->data, data) == 0)
			{
				tempLs->info->pret = pretNou;
			}
			tempLs = tempLs->next;

		}
		temp = temp->next;//parcurgere liste principale
	}
	
}
//stergere consultatie ale caror diagnostic este dat ca parametru
void stergeConsultatiiDupaDiagnostic(NodLp* cap, const char* diagnostic)
{
	NodLp* tempLp = cap;
	NodLp* anteriorLp = NULL;

	while (tempLp != NULL)
	{
		NodLs* tempLs = tempLp->info;
		NodLs* anteriorLs = NULL;

		while (tempLs != NULL)
		{
			if (strcmp(tempLs->info->diagnostic, diagnostic) == 0)
			{
				if (anteriorLs == NULL) // dacă nodul de sters este primul in lista secundara
				{
					tempLp->info = tempLs->next;
				}
				else
				{
					anteriorLs->next = tempLs->next;
				}

				NodLs* nodDeSters = tempLs;
				tempLs = tempLs->next;

				// Eliberare memorie
				free(nodDeSters->info->data);
				free(nodDeSters->info->nume);
				free(nodDeSters->info->specialitate);
				free(nodDeSters->info->diagnostic);
				free(nodDeSters->info);
				free(nodDeSters);
			}
			else
			{
				anteriorLs = tempLs;
				tempLs = tempLs->next;
			}
		}

		// Verificăm dacă lista secundară este goală după ștergeri
		if (tempLp->info == NULL)
		{
			if (anteriorLp == NULL) // dacă nodul de șters este primul în lista principală
			{
				NodLp* nodDeSters = cap;
				cap = cap->next;

				// Eliberare memorie
				free(nodDeSters->specialitate);
				free(nodDeSters);
			}
			else
			{
				anteriorLp->next = tempLp->next;

				NodLp* nodDeSters = tempLp;
				tempLp = tempLp->next;

				// Eliberare memorie
				free(nodDeSters->specialitate);
				free(nodDeSters);
			}
		}
		else
		{
			anteriorLp = tempLp;
			tempLp = tempLp->next;
		}
	}
}



void main()
{
	FILE* f = fopen("Consultatie.txt", "r");
	if (f != NULL)
	{
		char bufferLine[300], sep_list[] = ",\n";
		NodLp* capLp = NULL;


		while (fgets(bufferLine, sizeof(bufferLine), f))
		{
			char* token = strtok(bufferLine, sep_list);
			char* data = (char*)malloc(strlen(token) + 1);
			strcpy(data, token);

			token = strtok(NULL, sep_list);
			char* nume = (char*)malloc(strlen(token) + 1);
			strcpy(nume, token);

			token = strtok(NULL, sep_list);
			char* specialitate = (char*)malloc(strlen(token) + 1);
			strcpy(specialitate, token);

			token = strtok(NULL, sep_list);
			char* diagnostic = (char*)malloc(strlen(token) + 1);
			strcpy(diagnostic, token);

			token = strtok(NULL, sep_list);
			int pret = (int)atoi(token);



			Consultatie* consultatie = initializareConsultatie(data, nume, specialitate, diagnostic, pret);
			NodLp* temp = capLp;
			NodLp* anterior = NULL;
			while (temp != NULL && strcmp(temp->specialitate, specialitate) != 0)
			{
				anterior = temp;
				temp = temp->next;

			}
			if (temp == NULL)
			{
				NodLs* capLs = NULL;
				capLs = inserareInListaSecundara(capLs, consultatie);
				capLp = inserareInListaPrincipala(capLp, specialitate, capLs);
			}
			else
			{
				temp->info = inserareInListaSecundara(temp->info, consultatie);
			}

		}
		fclose(f);
		afisareLp(capLp);
		
		printf("Cerinta 2: \n");
		int val = 0;
		val = valTotalaSpecialitate(capLp, "cardiologie");
		printf("%d", val);

		printf("Cerinta 3: \n");
		modificarePret(capLp, "12.02.2024", 800);
		afisareLp(capLp);

		printf("Cerinta 4: \n");
		stergeConsultatiiDupaDiagnostic(capLp, "rau");
		afisareLp(capLp);
		
	}
}