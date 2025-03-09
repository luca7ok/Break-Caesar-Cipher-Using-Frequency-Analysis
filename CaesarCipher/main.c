#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <float.h>
#include <math.h>

void print_main_menu()
{
	printf("\nChoose one of the options:\n");
	printf("0) Exit\n");
	printf("1) Read text from keyboard\n");
	printf("2) Read text from file\n");
	printf("3) Encrypt text\n");
	printf("4) Decrypt text\n");
	printf("5) Compute frequency of text\n");
	printf("6) Break the cypher\n");
}

void print_second_menu()
{
	printf("Choose a method:\n");
	printf("1) Chi-squared\n");
	printf("2) Euclidian distance\n");
	printf("3) Cosine distance\n");
}

void read_distribution(const char* filename, double distribution[])
{
	FILE* fptr = fopen(filename, "r");
	char line[10];
	double value;
	int p = 0;
	while (fgets(line, 10, fptr)) {
		value = atof(line);
		distribution[p++] = value;
	}
	fclose(fptr);
}

void compute_histogram(const char text[], double histogram[])
{
	int length = 0;
	for (int i = 0; text[i]; i++) {
		if (islower(text[i])) {
			length++;
			histogram[text[i] - 'a']++;
		}
		else if (isupper(text[i])) {
			length++;
			histogram[text[i] - 'A']++;
		}
	}
	for (int i = 0; i < 26; i++)
		histogram[i] = histogram[i] * 100 / length;

}

void encrypt(char text[], int shift)
{
	for (int i = 0; text[i]; i++)
		if (isupper(text[i]))
		{
			text[i] -= 'A';
			text[i] = (text[i] + 26 + shift) % 26;
			text[i] += 'A';
		}
		else if (islower(text[i]))
		{
			text[i] -= 'a';
			text[i] = (text[i] + 26 + shift) % 26;
			text[i] += 'a';
		}

}

void decrypt(char text[], int shift)
{
	for (int i = 0; text[i]; i++)
		if (isupper(text[i]))
		{
			text[i] -= 'A';
			text[i] = (text[i] + 26 - shift) % 26;
			text[i] += 'A';
		}
		else if (islower(text[i]))
		{
			text[i] -= 'a';
			text[i] = (text[i] + 26 - shift) % 26;
			text[i] += 'a';
		}
}

double chi_squared_distance(const double hist1[], const double hist2[])
{
	double sum = 0;
	for (int i = 0; i < 26; i++)
		sum = sum + (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]) / hist2[i];
	return sum;
}

double cosine_distance(const double hist1[], const double hist2[])
{
	double sum1 = 0, sum2 = 0, sum3 = 0;
	for (int i = 0; i < 26; i++) {
		sum1 = sum1 + hist1[i] * hist2[i];
		sum2 = sum2 + hist1[i] * hist1[i];
		sum3 = sum3 + hist2[i] * hist2[i];
	}
	sum2 = sqrt(sum2);
	sum3 = sqrt(sum3);
	return 1 - sum1 / (sum2 * sum3);
}

double euclidean_distance(const double hist1[], const double hist2[])
{
	double sum = 0;
	for (int i = 0; i < 26; i++)
		sum = sum + (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
	sum = sqrt(sum);
	return sum;
}

void break_cyper(const char text[], int top_shifts[], double top_distances[], double (*distance_function)(const double[], const double[]))
{
	const char* filename = "english_distribution.txt";
	double* eng_hist = (double*)calloc(26, sizeof(double));
	read_distribution(filename, eng_hist);

	char new_text[1001] = "";
	strcpy(new_text, text);

	for (int shift = 0; shift < 26; shift++)
	{
		decrypt(new_text, 1);

		double* new_text_hist = (double*)calloc(26, sizeof(double));
		compute_histogram(new_text, new_text_hist);
		double value = distance_function(new_text_hist, eng_hist);

		if (value <= top_distances[0]) {
			top_distances[2] = top_distances[1];
			top_distances[1] = top_distances[0];
			top_distances[0] = value;
			top_shifts[2] = top_shifts[1];
			top_shifts[1] = top_shifts[0];
			top_shifts[0] = (shift + 1) % 26;
		}
		else if (value <= top_distances[1]) {
			top_distances[2] = top_distances[1];
			top_distances[1] = value;
			top_shifts[2] = top_shifts[1];
			top_shifts[1] = (shift + 1) % 26;
		}
		else if (value <= top_distances[2]) {
			top_distances[2] = value;
			top_shifts[2] = (shift + 1) % 26;

		}
		free(new_text_hist);
	}
	free(eng_hist);


}
int main()
{
	char text[1001] = "";
	double* text_hist = (double*)calloc(26, sizeof(double));

	while (true)
	{
		print_main_menu();
		printf("Your option: ");
		int option;
		scanf_s("%d", &option);
		while (getchar() != '\n');

		switch (option) {
		case 0: {
			exit(-1);
			break;
		}

		case 1: {
			printf("Your text: ");
			fgets(text, 1001, stdin);
			break;
		}

		case 2: {
			FILE* fptr;
			fptr = fopen("input.txt", "r");
			fgets(text, 1001, fptr);
			strcat(text, "\n");
			printf("Your text: %s", text);
			fclose(fptr);
			break;
		}

		case 3: {
			if (text[0] == '\0') {
				FILE* fptr;
				fptr = fopen("input.txt", "r");
				fgets(text, 1001, fptr);
				strcat(text, "\n");
				fclose(fptr);
			}

			int shift;
			printf("Your text: ");
			printf("%s", text);
			printf("Shift: ");
			scanf_s("%i", &shift);

			encrypt(text, shift);

			printf("Your encrypted text: ");
			printf("%s", text);

			break;
		}

		case 4: {
			if (text[0] == '\0') {
				FILE* fptr;
				fptr = fopen("input.txt", "r");
				fgets(text, 1001, fptr);
				strcat(text, "\n");
				fclose(fptr);
			}

			int shift;
			printf("Your text: ");
			printf("%s", text);
			printf("Shift: ");
			scanf_s("%i", &shift);

			decrypt(text, shift);

			printf("Your decrypted text: ");
			printf("%s", text);

			break;
		}

		case 5: {
			if (text[0] == '\0') {
				FILE* fptr;
				fptr = fopen("input.txt", "r");
				fgets(text, 1001, fptr);
				strcat(text, "\n");
				fclose(fptr);
			}
			compute_histogram(text, text_hist);

			for (int i = 0; i < 26; i++)
				if (text_hist[i] != 0)
					printf("%c/%c: %.2f%%\n", i + 'a', i + 'A', text_hist[i]);
			free(text_hist);
			break;
		}

		case 6: {
			if (text[0] == '\0') {
				FILE* fptr;
				fptr = fopen("input.txt", "r");
				fgets(text, 1001, fptr);
				strcat(text, "\n");
				fclose(fptr);
			}

			int method;

			printf("\nYour text: ");
			printf("%s", text);
			print_second_menu();
			printf("Your method: ");
			scanf_s("%d", &method);

			int* top_shifts = (int*)calloc(3, sizeof(int));
			double* top_distances = (double*)calloc(3, sizeof(double));
			top_distances[0] = DBL_MAX;
			top_distances[1] = DBL_MAX;
			top_distances[2] = DBL_MAX;
			char new_text[1001] = "";
			bool valid = true;

			switch (method) {
			case 1:
				printf("\nMethod: Chi-Squared Distance\n");
				break_cyper(text, top_shifts, top_distances, chi_squared_distance);
				break;
			case 2:
				printf("\nMethod: Euclidean Distance\n");
				break_cyper(text, top_shifts, top_distances, euclidean_distance);
				break;
			case 3:
				printf("\nMethod: Cosine Distance\n");
				break_cyper(text, top_shifts, top_distances, cosine_distance);
				break;

			default:
				printf("Invalid method\n");
				valid = false;
				break;
			}
			if (valid) {
				printf("Top 3:\n");
				printf("1) \nDistance: %.2f \nShift: %d \nDecryptd text:\n", top_distances[0], top_shifts[0]);
				strcpy(new_text, text);
				decrypt(new_text, top_shifts[0]);
				printf("%s\n", new_text);

				printf("2) \nDistance: %.2f \nShift: %d \nDecrypted text:\n", top_distances[1], top_shifts[1]);
				strcpy(new_text, text);
				decrypt(new_text, top_shifts[1]);
				printf("%s\n", new_text);

				printf("3) \nDistance: %.2f \nShift: %d \nDerypted text:\n", top_distances[2], top_shifts[2]);
				strcpy(new_text, text);
				decrypt(new_text, top_shifts[2]);
				printf("%s\n", new_text);
			}

			free(top_distances);
			free(top_shifts);
			break;
		}

		default:
			printf("Invalid option\n");
			break;
		}

	}
}