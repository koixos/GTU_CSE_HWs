#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main() {

    	// Ask for the problem selection (1,2,3) .....  	
	int exit = 0;
	do
	{
		int prob;
	  	printf("------------------------------------------------------\n MENU:\n\n (1) PART-1\n (2) PART-2\n (3) PART-3\n (0) EXIT\n\n $ Selection: ");
	  	scanf("%d", &prob);
	  	printf("------------------------------------------------------");
	  	
	  	if(prob == 0)
		{
			printf("------------------------------------------------------\nQuitting...\n");
			exit = 1;
		}
		else if(prob == 1)
		{
			// Get the input from the user for the first problem, ie, to test dt1a and dt1b
			float pl, pw, sl, sw;
			char out1, out2;
			printf("\nEnter the pedal-length: ");
			scanf("%f", &pl);
			printf("Enter the pedal-width: ");
			scanf("%f", &pw);
			printf("Enter the sepal-length: ");
			scanf("%f", &sl);
			printf("Enter the sepal-width: ");
			scanf("%f", &sw);
			
	  		// Compare performances and print results 
			char returned_first = dt1a(pl, pw, sl, sw);
			char returned_second = dt1b(pl, pw, sl, sw);
			
			if(returned_first == returned_second)
			{
				printf("\n");
				if(returned_first == 's') printf("Answer: Setosa\n");
				else if(returned_first == 'V') printf("Answer: Versicolor\n");
				else if(returned_first == 'v') printf("Answer: Virginica\n");
			}
			else
			{
				printf("\nAnswers: ");
				if(returned_first == 's') printf("Setosa, ");
				else if(returned_first == 'V') printf("Versicolor, ");
				else printf("Virginica, ");
				if(returned_second == 's') printf("Setosa\n");
				else if(returned_second == 'V') printf("Versicolor\n");
				else printf("Virginica\n");
			}
		}
		else if(prob == 2)
		{
			// Get the input from the user for the second problem, i.e., to test dt2a and dt2b 
			double x1, x2, x3;
			int x4, x5;
			printf("\nEnter x1: ");
			scanf("%lf", &x1);
			printf("Enter x2: ");
			scanf("%lf", &x2);
			printf("Enter x3: ");
			scanf("%lf", &x3);
			printf("Enter x4 (enter 0 or 1): ");
			scanf("%d", &x4);
			printf("Enter x5 (enter 0 or 1): ");
			scanf("%d", &x5);
			
	    		// Compare performances and print results 
	    		double returned_first = dt2a(x1, x2, x3, x4, x5);
	    		double returned_second = dt2b(x1, x2, x3, x4, x5);
	    		// we'll substract return values from each other to compare with CLOSE_ENOUGH
	    		double result_first = returned_first - returned_second;
	    		double result_second = returned_second - returned_first;
	    		// we'll use "<=" because as the difference between numbers getting smaller than CLOSE_ENOUGH, numbers become more similar 
	    		if(0 <= result_first && result_first <= CLOSE_ENOUGH || 0 <= result_second && result_second <= CLOSE_ENOUGH)
	    		{
	    			double average = (returned_first + returned_second)/2;
	    			printf("\nAnswer: %f\n", average);
	    		}
	    		else printf("\nAnswers: %f, %f\n", returned_first, returned_second);
		}
		else if(prob == 3)
		{
	    		// Get the input from the user for the third problem, i.e., to test dt3a and dt3b 
			double salon, yatak_odasi;
			int misafir_odasi, toplam_oda, kisi_sayisi;
			
			printf("\nSalonun metrekaresini giriniz: ");
			scanf("%lf", &salon);
			printf("Yatak odasının metrekaresini giriniz: ");
			scanf("%lf", &yatak_odasi);
			printf("Misafir odası istiyorsanız 1 istemiyorsanız 0 giriniz: ");
			scanf("%d", &misafir_odasi);
			printf("Toplam oda sayısını giriniz: ");
			scanf("%d", &toplam_oda);
			printf("Yaşayacak kişi sayısını giriniz: ");
			scanf("%d", &kisi_sayisi);
		
		    	// Compare performances and print results 
			int returned_first = dt3a(salon, yatak_odasi, misafir_odasi, toplam_oda, kisi_sayisi);
			int returned_second = dt3b(salon, yatak_odasi, misafir_odasi, toplam_oda, kisi_sayisi);
			
			if(returned_first == returned_second)
			{
				if(returned_first == 5)
					printf("\nIdeal eviniz %d+1 veya üstü olmalı.\n", returned_first);
				else
					printf("\nIdeal eviniz %d+1 olmalı.\n", returned_first);
			}
			else	// Eğer kıyaslamada return'ler farklıysa kullanıcıya ikisini de rapor edecek.
			{
				if(returned_first == 5 || returned_second == 5)
					printf("\nIdeal eviniz %d+1 veya %d+1 üstü olmalı.\n", returned_first, returned_second);
				else
					printf("\nIdeal eviniz %d+1 veya %d+1 olmalı.\n", returned_first, returned_second);
			}
		}
		else printf("\nINVALID SELECTION\n");
		
	} while (exit != 1);
	
    return 0;
}
