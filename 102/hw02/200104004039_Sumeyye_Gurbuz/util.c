#include <stdio.h>
#include <math.h>
#include "util.h"

// Provide your implementations for all the requested functions here
/*
	## FOR PROB-1: 
	$$ "s" for SETOSA
	$$ "V" for VERSICOLOR
	$$ "v" for VIRGINICA will be returned
*/
// first decision tree for PROB-1
char dt1a(float pl, float pw, float sl, float sw)
{
	if(pl < 2.45) return 's';
	else
	{
		if(pl < 4.95 && pw < 1.65) return 'V';
		else if(!(pw < 1.75)) return 'v'; 
		else if(pw < 1.75 && !(pl < 4.95)) return 'v';
		else if(pw < 1.75 && pl < 4.95 && !(pw < 1.65)) return 'v';
	}
}
// second decision tree for PROB-1
char dt1b(float pl, float pw, float sl, float sw)
{
	if(pl < 2.55) return 's';
	else
	{
		if(pw < 1.69 && pl < 4.85) return 'V';
		else if(!(pw < 1.69)) return 'v';
		else if(pw < 1.69 && !(pl < 4.85)) return 'v';
	}
}

// first decision tree for PROB-2
double dt2a(double x1, double x2, double x3, int x4, int x5)
{
	if(x1 < 31.5)
	{
		if(x2 > -2.5) return 5.0;
		else
		{
			if(x2-0.1 <= x1 && x1 <= x2+0.1) return 2.1;
			else return -1.1;
		}
	}
	else
	{
		if(-1 <= x3 && x3 <= 2) return 1.4;
		else
		{
			if(x4 && x5) return -2.23;
			else return 11.0;
		}
	}
}
// second decision tree for PROB-2
double dt2b(double x1, double x2, double x3, int x4, int x5)
{
	if(12 < x1 && x1 < 22)
	{
		if(x3 > 5.0/3.0) return -2.0;
		else
		{
			if(x1-0.1 <= x3 && x3 <= x1+0.1) return 1.01;
			else return -8;
		}
	}
	else
	{
		if(x4 && x5) return -1;
		else
		{
			if(-1 <= x2 && x2 <= 2) return -1.0/7.0;
			else return sqrt(2)/3.0;
		}
	}
}
/*	==========================================================================================================
	--> 3.bölümde ev tipi sınıflandırma üzerine iki fonksiyon yazdım.
	--> Kullanıcıdan alacağım input'lara göre kullanıcı için ideal olabilecek ev tipini/tiplerini tahmin edeceğim.
	==========================================================================================================	
	Inputs:
	----------------------------------------------------------------------------------------------------------
	1) salonun metrekaresi (reel number)
	2) yatak odasının metrekaresi (reel number)
	3) misafir odası isteniyor mu? (first categorical --> binary)
	4) istenen maks oda sayısı (second categorical)
	5) evde yaşayacak kişi sayısı (third categorical)
	===========================================================================================================
	Returns:
	-----------------------------------------------------------------------------------------------------------
	# 1+1 için --> return 1;
	# 2+1 için --> return 2;
	# 3+1 için --> return 3;
	# 4+1 için --> return 4;
	# 5+1 ve 
	  üstü için --> return 5;
	===========================================================================================================
	--> Kullanılan metrekare ölçülerinin kaynakçaları:
	-----------------------------------------------------------------------------------------------------------
	1) https://www.insaatim.com/forumlar/konu/ideal-bir-21-31-41-evin-olculeri-kac-m2-olmali/
	2) https://emlakkulisi.com/11-daire-en-az-kac-metrekare-olacak/546960
	===========================================================================================================
*/
// first decision tree for PROB-3
int dt3a(double salon, double yatak_odasi, int misafir_odasi, int toplam_oda, int kisi_sayisi)
{
	if(kisi_sayisi > 8) return 5;
	else
	{
		if(kisi_sayisi >= 5) return 4;
		else
		{
			if(toplam_oda >= 5) return 5;
			else
			{
				if(salon < 24) return 1;
				else
				{
					if(yatak_odasi < 14) return 1;
					else
					{
						if(salon <= 32 )
						{
							if(yatak_odasi > 18 && misafir_odasi != 0) return 4;
							else return 3;
							
							if(yatak_odasi <= 18 && misafir_odasi != 0) return 4;
							else
							{
								if(kisi_sayisi > 4) return 3;
								else return 2;
							}
						}
						else
						{
							if(misafir_odasi != 0) return 4;
							else return 3;
						}
					}
				}
			}
		}
	}
}

// second decision tree for PROB-3
int dt3b(double salon, double yatak_odasi, int misafir_odasi, int toplam_oda, int kisi_sayisi)
{
	if(salon < 12) return 1;
	else
	{
		if(yatak_odasi < 14) return 1;
		else
		{
			if(salon < 24) return 1;
			else
			{
				if(salon < 32)
				{
					if(kisi_sayisi > 8) return 5;
					else
					{
						if(toplam_oda >= 5) return 5;
						else
						{
							if(misafir_odasi != 0) return 4;
							else
							{
								if(kisi_sayisi > 4) return 3;
								else return 2;
							}
						}
					}
				}
				else
				{
					if(misafir_odasi == 0) return 3;
					else
					{
						if(kisi_sayisi > 8) return 5;
						else
						{
							if(toplam_oda < 5) return 4;
							else return 5;
						}
					}
				}
			}
			
		}
	}
}
