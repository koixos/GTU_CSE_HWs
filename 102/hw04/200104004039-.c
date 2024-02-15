#include <stdio.h>

int display_menu()
{
	int selection;
	printf("-----------------------------------------------\nSelect an operation:\n\n0 --> Enter the coefficients\n1 --> Draw the graph\n2 --> Print the graph into a .txt file\n3 --> Exit\n\nChoice: ");
	scanf("%d", &selection);
	printf("-----------------------------------------------\n");
	return selection;
}
void write_coefficients(int a, int b, int c)								
{
	FILE *ptr;
	ptr = fopen("coefficients.txt", "w");
	fprintf(ptr, "%d\n%d\n%d", a, b, c);
	fclose(ptr);
	printf("\ncoefficients.txt file has been created.\n");
}
int main()
{
	FILE *ptr;
	int exit = 0, a, b, c;
	while (exit == 0)
	{
		int selection = display_menu();
		if(selection == 3)
		{
			printf("Exitting...\n");
			exit = 1;
		}	
		else if(selection == 0)
		{
			printf("Enter the coefficients for the following equation:\n\n\t\tx = a*(y*y) + b*y + c\n\na: ");
			scanf("%d", &a);
			printf("b: ");
			scanf("%d", &b);
			printf("c: ");
			scanf("%d", &c);
			write_coefficients(a, b, c);	// katsayıları dosyaya yazan fonksiyon çağrıldı
		}
		else if(selection == 1)
		{
			if((ptr = fopen("coefficients.txt", "r")) != NULL)	// Dosya var mı yok mu kontrol edilecek
			{
				fscanf(ptr, "%d %d %d", &a, &b, &c);
				fclose(ptr);
				
				int y_axis = 15, x_axis = -50;	// y ve x koordinatlarının başlangıç değerleri
				for (int column = 16; column >= -16; column--)	// 16 olmasının sebebi eksenin uçlarına "v" ve "^" gelecek olması
				{
					printf("\t\t");
					int result = a*column*column + b*column + c;
					for (int row = -56; row <= 56; row++)	// 56 olmasının sebebi eksenin uçlarına "<" ve ">" gelecek olması
					{
						if(column == 16 && row == 0) printf("^");
						else if(column == -16 && row == 0) printf("v");
						else if(column == 0 && row == -56) printf("<");
						else if(column == 0 && row == 56) printf(">");
						else
						{	
							if(row == -1 && column == -1 && row != result) printf("\033[1;34m0\033[0m");	// orijin noktasının 0'ı yazdırıldı
							else if(row == -1 && column % 5 == 0)	// sütunun sayı değerleri yazdırılacak
							{	// \b'ler gerekli durumlarda fazladan birim tutan karakterleri silecek 
								if(row != result && y_axis >= 10 || (y_axis < 0 && y_axis > -10)) printf("\b");
								else if(y_axis <= -10 && row != result) printf("\b\b");
								if(y_axis != 0 && row != result) printf("\033[1;34m%d\033[0m", y_axis);	// y_axis değeri 0 değilken değerler yazdırıldı
								else if(y_axis == 0 && row != result) printf("-");	// eğer 0'sa bastırmayacak çünkü 61.satırda koyduk zaten
								else if(row == result) printf("\e[0;31m#\033[0m");
								y_axis -= 5;
							}
							
							else if(column == -1 && row % 10 == 0)		// satırın sayı değerleri yazdırılacak
							{
								if(row != result && x_axis >= 10 || (x_axis < 0 && x_axis > -10)) printf("\b");
								else if(x_axis <= -10 && row != result) printf("\b\b");
								if(x_axis != 0 && row != result) printf("\033[1;34m%d\033[0m", x_axis);	// x_axis değeri 0 değilken değerler yazdırıldı
								else if(x_axis == 0 && row != result) printf("|");	// eğer 0'sa bastırmayacak çünkü 61.satırda 0 koyuldu
								else if(row == result) printf("\e[0;31m#\033[0m");
								x_axis += 10;
							}
							else if(row == 0 && row != result) printf("|");	// sütun yazdırıldı,eğer grafiğin noktası eksen üstündeyse | koymayacak
							else if(column == 0 && row != result) printf("-");// satır yazdırıldı,eğer grafiğin noktası eksen üstündeyse - koymayacak
							else if(row == result) printf("\e[0;31m#\033[0m");// eğer satır değeri grafik değerine eşitse # koyacak
							else printf(" ");				// diğer bütün durumlarda boşluk atacak	
						}
					}
					printf("\n");
				}
			}
			else printf("File does not exist.\n");	// eğer dosya yoksa uyarı verecek
		}
		else if(selection == 2)
		{
			if((ptr = fopen("coefficients.txt", "r")) == NULL) printf("File does not exist.\n");
			else
			{
				fscanf(ptr, "%d %d %d ", &a, &b, &c);
				fclose(ptr);
				printf("Coefficients has been read from the coeficient file.\n");
				
				ptr = fopen("graph.txt", "w");
				for (int column = 16; column >= -16; column--)
				{
					fprintf(ptr, "\t\t");
					int result = a*column*column + b*column + c;
					for (int row = -56; row <= 56; row++)
					{
						if(column == 16 && row == 0) fprintf(ptr, "^");
						else if(column == -16 && row == 0) fprintf(ptr, "v");
						else if(column == 0 && row == -56) fprintf(ptr, "<");
						else if(column == 0 && row == 56) fprintf(ptr, ">");
						else
						{
							if(row == 0 && row != result) fprintf(ptr, "|");
							else if(column == 0 && row != result) fprintf(ptr, "-");
							else if(row == result) fprintf(ptr, "#");
							else fprintf(ptr, " ");
						}
					}
					fprintf(ptr, "\n");
				}
				fclose(ptr);
				printf("The graph of x = a*(y*y) + b*y + c has been written to \"graph.txt\" file.\n");
			}
		}
		else printf("Invalid selection\n");
	}
	return 0;
}
