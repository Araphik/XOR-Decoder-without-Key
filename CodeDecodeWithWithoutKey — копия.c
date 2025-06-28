
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define BLOCK_SIZE 5
#define MAX_SIZE 30
#define PROBABILITY 0.046
#define GENERATION 5
#define ALPHABET_EN "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define ALPHABET_RU "������������������������������������������������������������������"
#define SPECIAL "123456789.,?!:;()[]{}~#�^\"\'-���������??`���<>?��&@#$%*+=_/\| \n"

enum { END = 0, CODE = 1, DECODE_KEY = 2, DECODE_NOKEY = 3, EXIT = 4 };

unsigned char mbkey[1024] = { 0 };
int p;
typedef struct
{
	unsigned char File;
	unsigned char Key;
	unsigned char Text_Generate;
} Symbol;

//��������� ������//
typedef struct
{
	FILE* Start;
	FILE* End;
	FILE* Coded;
	FILE* Key;
	FILE* Generate_Text;
	FILE* Probably_Keys;
} File;
//

void insertionSort(int arr1[], int arr2[], int n) 
{
	int i, key, j, key1;
	for (i = 1; i < n; i++) 
	{
		key = arr1[i];
		key1 = arr2[i];
		j = i - 1;

		// ���������� �������� arr[0..i-1], ������� ������ key, �� ���� ������� ������
		// ������������ ������� ������� j
		while (j >= 0 && arr1[j] < key) 
		{
			arr1[j + 1] = arr1[j];
			arr2[j+1] = arr2[j+1];
			j = j - 1;
		}
		arr1[j + 1] = key;
		arr2[j + 1] = key1;
	}
}

void removeSpaces(unsigned char* str) 
{
	int i, j;
	for (i = 0, j = 0; str[i] != '\0'; i++) 
	{
		if (str[i] != ' ') 
		{
			str[j++] = str[i];
		}
	}
	str[j] = '\0';
}




//�����������//
void Coding()
{
	Symbol Sym;
	File F;
	F.Start = fopen("Start.txt", "rb");;//�������� �����
	F.Coded = fopen("Coded.txt", "wb");
	F.Key = fopen("Key.txt", "rb");

	Sym.File = '\0', Sym.Key = '\0';

	fseek(F.Start, 0, SEEK_END);
	int buf1 = ftell(F.Start);//���� ����� ��������� ������
	fseek(F.Start, 0, SEEK_SET);

	fseek(F.Key, 0, SEEK_END);
	int buf2 = ftell(F.Key);//���� ����� �����
	fseek(F.Key, 0, SEEK_SET);


	while (ftell(F.Start) != buf1)
	{
		Sym.File = fgetc(F.Start);
		if (ftell(F.Key) < buf2)
			Sym.Key = fgetc(F.Key);
		else
		{
			fseek(F.Key, 0, SEEK_SET);
			Sym.Key = fgetc(F.Key);
		}
		Sym.File ^= Sym.Key;//������������ ����������
		fputc(Sym.File, F.Coded);
	}

	fclose(F.Coded);
	fclose(F.Start);
	fclose(F.Key);
}

//������������ � ������//
void Decoding_Key()
{
	Symbol Sym;
	File F;

	F.End = fopen("End.txt", "wb");
	F.Coded = fopen("Coded.txt", "rb");
	F.Key = fopen("Key.txt", "rb");

	Sym.File = '\0', Sym.Key = '\0';

	fseek(F.Coded, 0, SEEK_END);
	int buf1 = ftell(F.Coded);//���� ����� ��������� ������
	fseek(F.Coded, 0, SEEK_SET);

	fseek(F.Key, 0, SEEK_END);
	int buf2 = ftell(F.Key);//���� ����� �����
	fseek(F.Key, 0, SEEK_SET);

	while (ftell(F.Coded) != buf1)
	{
		Sym.File = fgetc(F.Coded);
		if (ftell(F.Key) < buf2)
			Sym.Key = fgetc(F.Key);
		else
		{
			fseek(F.Key, 0, SEEK_SET);
			Sym.Key = fgetc(F.Key);
		}
		Sym.File ^= Sym.Key;//������������ ������������
		fputc(Sym.File, F.End);
	}

	fclose(F.End);
	fclose(F.Coded);
	fclose(F.Key);
}

//������ ����������//
float coincidence_index(int len, unsigned char* block)
{
	long long freq[256] = { 0 }; // ������ ��� �������� ������ ��������
	float coincidence = 0.0;//����������


	for (int i = 0; i < len; i++)
	{
		freq[block[i]]++;
	}
	for (int i = 0; i < 256; i++)
	{
		coincidence += freq[i] * (freq[i] - 1);
	}
	coincidence /= len * (len - 1);
	return coincidence;
}
//

void print_pattern_chars(FILE* output, unsigned char* str, int distance) 
{
	int length = strlen(str);
	int i;

	for (i = 0; i < length - distance + 1; i += distance) 
	{
		int j;
		for (j = i; j < i + distance; j++) 
		{
			printf("%c", str[j]);
			fprintf(output, "%c", str[j]);
		}
		printf("\n");
		fprintf(output, "%c", '\n');
	}
}

//���������� ����� �����//
int LenKey()
{
	Symbol Sym;
	File F;

	F.Coded = fopen("Coded.txt", "rb");
	Sym.File = '\0', Sym.Key = '\0';

	fseek(F.Coded, 0, SEEK_END);
	int buf1 = ftell(F.Coded);//����� �������������� ������
	fseek(F.Coded, 0, SEEK_SET);

	float* coincidence_index_avg;//������������ ������ ��� �������� ����������
	coincidence_index_avg = (float*)malloc(MAX_SIZE * sizeof(float));
	memset(coincidence_index_avg, 0, MAX_SIZE);

	for (int key_len = 1; key_len <= MAX_SIZE; key_len++)//������� ����� �����
	{
		int count = 0;//��� �������� ���������� ������
		coincidence_index_avg[key_len - 1] = 0.0;//������� ���������� ������
		for (int i = 0; i < key_len; i++)
		{
			unsigned char block[BLOCK_SIZE] = { 0 };//������ ��� �������� �����
			int block_count = 0;//������� ������

			for (int j = i; j < buf1; j += key_len)
			{
				fseek(F.Coded, j, SEEK_SET);
				block[block_count++] = fgetc(F.Coded);
				if (block_count == BLOCK_SIZE)
				{
					coincidence_index_avg[key_len - 1] += coincidence_index(BLOCK_SIZE, block);
					count++;
					block_count = 0;
				}
			}
			rewind(F.Coded);
		}
		coincidence_index_avg[key_len - 1] /= count;//������� ���������� ���������� ��������
		printf("����� %d: %.3lf\n", key_len, coincidence_index_avg[key_len - 1]);
	}

	int MaxKeyLength = 1;
	float max_coincidence_index = 0.0;
	for (int i = 1; i < MAX_SIZE; i++)
	{
		if (coincidence_index_avg[i] >= PROBABILITY)
		{
			max_coincidence_index = coincidence_index_avg[i];
			MaxKeyLength = i + 1;
			break;
		}
	}

	fclose(F.Coded);
	return MaxKeyLength;
}

//���������� �����//
void FindKey(int Length_Key)
{
	Symbol Sym;
	File F;

	F.Coded = fopen("Coded.txt", "rb");
	Sym.File = '\0', Sym.Key = '\0';

	F.Probably_Keys = fopen("probably_keys.txt", "wb");

	fseek(F.Coded, 0, SEEK_END);
	int buf1 = ftell(F.Coded);//����� �������������� ������
	fseek(F.Coded, 0, SEEK_SET);
	p = 0;

	for (int i = 0; i < Length_Key; i++)
	{
		unsigned char* block;//��� �������������� ���������� ��������
		block = (unsigned char*)malloc((buf1 / Length_Key + 1) * sizeof(unsigned char));
		memset(block, 0, (buf1 / Length_Key + 1) * sizeof(unsigned char));
		int index = 0;

		for (int j = i; j < buf1; j += Length_Key)
		{
			fseek(F.Coded, j, SEEK_SET);
			block[index] = fgetc(F.Coded);
			block[index] ^= ' ';//xor c ����� ���������� ��������
			if (block[index] == 0)
				block[index] = '0';
			index++;
		}

		rewind(F.Coded);
		block[index] = '\0';
		unsigned char most_common = 0;//����� ���������� ������ �������������� ������
		int most_count = 0;//��� ������
		int j = 0;

		int Kol[256] = { 0 };
		int Alphabet[256] = {0};
		for (int t = 0; t < 256; t++) Alphabet[t] = t;

		while (j < 256)
		{
			int count = 0;
			for (int k = 0; k < strlen(block); k++)
			{
				if (block[k] == j) Kol[j] += 1;
			}
			j++;
		}

		insertionSort(Kol, Alphabet, 256);
		for (int m = 0; m < GENERATION; m++)
		{
			mbkey[p++] = Alphabet[m];
			if (m == 0)
				fputc(Alphabet[m], F.Probably_Keys);
		}
	}

	fputc('\n', F.Probably_Keys);
	fclose(F.Coded);
	fclose(F.Probably_Keys);
}

//������������ ��� �����//
void Decoding_NoKey()
{
	Symbol Sym;
	File F;
	F.Coded = fopen("Coded.txt", "rb");//��������� �������������� ���������

	Sym.File = '\0', Sym.Key = '\0';

	fseek(F.Coded, 0, SEEK_END);
	int buf1 = ftell(F.Coded);//������ ����� ��������������� ���������
	fseek(F.Coded, 0, SEEK_SET);

	int Length_Key = 0;
	Length_Key = LenKey();//����� �����
	printf("����� ����� = %d\n", Length_Key);

	FindKey(Length_Key);//������� ����

	mbkey[p] = '\0';
	int flag = 0;

	FILE* output;
	output = fopen("probably_keys.txt", "wb");

	printf("\n��������� �����:\n");

	int m = 0;

	for (int x = 0; x < GENERATION - 1; x++)
	{
		m = x;
		while(m < strlen(mbkey))
		{
			printf("%c", mbkey[m]);
			fputc(mbkey[m], output);
			m += GENERATION;
		}
		printf("\n");
		fputc('\n', output);
	}

    m = 0;

	for (m = 0; m < p; m++)
	{
		if ((strchr(ALPHABET_EN, mbkey[m]) == NULL) && (strchr(ALPHABET_RU, mbkey[m]) == NULL) && (strchr(SPECIAL, mbkey[m]) == NULL))
		{
			mbkey[m] = ' ';
		}
	}


	print_pattern_chars(output, mbkey , Length_Key);

	m = 0;
	int v = 0;

	char mb_ru[31] = { 0 };
	char mb_en[31] = { 0 };

	while (m < strlen(mbkey))
	{
		if (strchr(ALPHABET_EN, mbkey[m]) != NULL)
		{
			mb_en[v++] = mbkey[m];
		}	
		m++;
	}
	mb_en[v] = '\0';
	fprintf(output, "%s\n", mb_en);
	printf("%s\n", mb_en);

	m = 0;
	v = 0;
	
	while (m < strlen(mbkey))
	{
		if (strchr(ALPHABET_RU, mbkey[m]) != NULL)
		{
			mb_ru[v++] = mbkey[m];
		}
		m++;
	}
	mb_ru[v] = '\0';
	fprintf(output, "%s\n", mb_ru);
	printf("%s\n", mb_ru);


	if (flag = 1) removeSpaces(mbkey);

	mbkey[Length_Key] = '\0';

	printf("%s\n", mbkey);
	fprintf(output, "%s\n", mbkey);

	fclose(output);

	F.Probably_Keys = fopen("probably_keys.txt", "rb");
	fseek(F.Probably_Keys, 0, SEEK_END);
	int Size = ftell(F.Probably_Keys);
	rewind(F.Probably_Keys);

	unsigned char Final_Key[MAX_SIZE + 1] = { 0 };
	double MAX_Percent = 0.0;
	while (ftell(F.Probably_Keys) != Size)
	{
		unsigned char s[MAX_SIZE + 1] = { 0 };
	    fgets(s, 30, F.Probably_Keys);

		F.Generate_Text = fopen("Generate_TEXT.txt", "wb");

		int count = 0;
		double Percent = 0.0;
		int n = 0;
		while (ftell(F.Coded) != buf1)
		{
			Sym.File = fgetc(F.Coded);
			if (n < strlen(s) && s[n] != '\n')
				Sym.Key = s[n++];
			else
			{
				n = 0;
				Sym.Key = s[n++];
			}
			Sym.File ^= Sym.Key;//������������ ������������
			if ((strchr(ALPHABET_EN, Sym.File) != NULL) || (strchr(ALPHABET_RU, Sym.File) != NULL))
				count++;
			fputc(Sym.File, F.Generate_Text);
		}
		rewind(F.Coded);

		Percent = (double)count / (double)buf1;

		if (Percent >= MAX_Percent)
		{
			MAX_Percent = Percent;
			memset(Final_Key, 0, MAX_SIZE + 1);
			if (s[strlen(s) - 1] = '\n') s[strlen(s) - 1] = '\0';
			strcpy(Final_Key, s);
		}

		fclose(F.Generate_Text);
	}

	printf("\n�������� ����: %s", Final_Key);
	F.Key = fopen("Generate_KEY.txt", "wb");
	fputs(Final_Key,F.Key);
	fclose(F.Key);

	F.Key = fopen("Generate_KEY.txt", "rb");
	fseek(F.Key, 0, SEEK_END);
	int buf2 = ftell(F.Key);
	rewind(F.Key);

	F.Generate_Text = fopen("Generate_TEXT.txt", "wb");

	while (ftell(F.Coded) != buf1)
	{
		Sym.File = fgetc(F.Coded);
		if (ftell(F.Key) < buf2)
			Sym.Key = fgetc(F.Key);
		else
		{
			fseek(F.Key, 0, SEEK_SET);
			Sym.Key = fgetc(F.Key);
		}
		Sym.File ^= Sym.Key;//������������ ������������
		fputc(Sym.File, F.Generate_Text);
	}

	fclose(F.Coded);
	fclose(F.Generate_Text);
	fclose(F.Key);
	fclose(F.Probably_Keys);

	memset(mbkey, 0, 1024);

	printf("\n");
	system("pause");
	system("cls");
}

void Menu()
{
	int Select;
	while (1)
	{
		system("cls");
		printf("------------------------------------\n");
		printf("|                                  |\n");
		printf("|         ��� �� ��������?         |\n");
		printf("|                                  |\n");
		printf("|  1) ����������                   |\n");
		printf("|                                  |\n");
		printf("|  2) ������������ � ������        |\n");
		printf("|                                  |\n");
		printf("|  3) ������������ ��� �����       |\n");
		printf("|                                  |\n");
		printf("|  4) �����                        |\n");
		printf("|                                  |\n");
		printf("------------------------------------\n");
		printf("����: ");
		scanf_s("%d", &Select);
		while (getchar() != '\n');
		system("cls");


		if (Select == CODE)
		{
			Coding();
			continue;
		}
		else
			if (Select == DECODE_KEY)
			{
				Decoding_Key();
				continue;
			}
			else
				if (Select == DECODE_NOKEY)
				{
					Decoding_NoKey();
					continue;
				}
				else
					if (Select == EXIT)
					{
						return EXIT;
					}
					else
					{
						printf("�������� ��������! ������� �����\n");
						Sleep(1000);
						system("cls");
					}
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	Menu();
	return END;
}
