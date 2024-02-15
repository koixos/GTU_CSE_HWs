
/* PLEASE LOOK AT < SELECT_SHAPE FUNCT. > AND < SHAPE: TRIANGLE, CASE 1 > FOR MOST DETAILED EXPLANATION */

#include <stdio.h>
#include <math.h>
#define PI 3.14

enum Shapes{triangle = 1, quadrilateral, circle, pyramid, cylinder};
enum Calculators{area = 1, perimeter, volume};

int is_triangle_valid(float a, float b, float c)
{	/* triangle inequality */
	/* if sides don't fit with this ineaqulity, funct. will return -1 error code to main() */
	if(c >= a+b || b >= a+c || a >= b+c) return -1;
	else return 0;
} /* END of is_triangle_valid */
int is_quadrilateral_valid(float a, float b, float c, float d)
{	/* quadrilateral inequality --- https://en.wikipedia.org/wiki/Quadrilateral#Sides */
	/* If sides don't fit with this ineaqulity, funct. will return -1 error code to main() */
	if((a*a+b*b+c*c) <= (d*d/3.0) || (a*a+b*b+d*d) <= (c*c/3.0) || (a*a+c*c+d*d) <= (b*b/3.0) || (b*b+c*c+d*d) <= (a*a/3.0)) return -1;
	else return 0;
} /* END of is_quadrilateral_valid */
int select_shape()
{	
	enum Shapes shape;
	int flag = 0;
	printf(" $$ Select a shape to calculate:\n-----------------------------------------------------------------\n 1. Triangle\n 2. Quadrilateral\n 3. Circle\n 4. Pyramid\n 5. Cylinder\n 0. Exit\n-----------------------------------------------------------------\n--> Selection: ");
	if(scanf("%u", &shape) != 1) /* If return value of scanf != 1 means --> If the input is not an unsigned integer ... */
	{
		while (flag == 0)
			if(getchar() == 10) flag = 1; /* This loop will get all the input characters till the enter which means we are flushing scanf(enter button's ASCII: 10) */
		printf("-----------------------------------------------------------------\n");
		return -1; /* If input is not an unsig. integer, funct. will return -1 error code to main() */
	}
	printf("-----------------------------------------------------------------\n");
	return shape; /* If value is an unsigned integer, return itself */
} /* END of select_shape */
int select_calc()
{	/* We'll do same things with select_shape() funct. for select_calc() funct. too */
	enum Calculators calc;
	int flag = 0;
	printf(" $$ Select a calculator:\n-----------------------------------------------------------------\n1. Area\n2. Perimeter\n3. Volume\n0. Exit\n-----------------------------------------------------------------\n--> Selection: ");
	if(scanf("%u", &calc) != 1)
	{
		while (flag == 0)
			if(getchar() == 10) flag = 1;
		printf("-----------------------------------------------------------------\n");
		return -1;
	}
	printf("-----------------------------------------------------------------\n");
	return calc;
} /* END of select_calc */
int calc_triangle(int calc)
{
	int exit = 0, returned, flag = 0;
	float a, b, c, s, calc_area, calc_perimeter;
	switch(calc)
	{
		case 1: /* case AREA */
			/* In this while, we'll read scanf's return value for every side and if input is not valid, then we'll return error immediately and ask again for input */
			/* And we'll make this check for all the inputs in each case */
			while (exit == 0)
			{
				printf("--> Enter three sides of the triangle:\n");
				if(scanf("%f", &a) == 1 && a > 0) /* If input is an float int. and also bigger than zero, we'll ask for other side */
				{				 /* If it is not valid, print error message in else */
					if(scanf("%f", &b) == 1 && b > 0)
					{
						if(scanf("%f", &c) == 1 && c > 0)
						{	/* If all inputs are valid, then we'll check for if these sides can make a triangle */
							returned = is_triangle_valid(a, b, c);
							if(returned == -1) printf("-----------------------------------------------------------------\nERROR! Please enter a valid triangle\n-----------------------------------------------------------------\n");
							else exit = 1; /* If inputted sides can make a triangle, then quit while */
						}
						else
						{
							flag = 0;
							printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
							/* Same operation with select_shape and select_calc */
							/*  If input is invalid, flush (via getchar till enter) scanf and ask for it again */
							while (flag == 0)
								if(getchar() == 10) flag = 1; /* till 10 means --> till "enter" character */
						}
					}
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					flag = 0;
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			s = (a+b+c)/2.0; /* semi perimeter */
			calc_area = sqrt(s*(s-a)*(s-b)*(s-c)); /* Heron's formula */
			printf("-----------------------------------------------------------------\n:: Area of the triangle: %.2f\n", calc_area);
			break;
		case 2: /* case PERIMETER */
			/* Same operation with CASE 1 --- for detailed explanation please look at CASE 1 */
			/* In this while, we'll read scanf's return value for every side and if input is not valid, then we'll return error immediately and ask again for input */
			while (exit == 0)
			{
				printf("--> Please enter three sides of the triangle:\n");
				if(scanf("%f", &a) == 1 && a > 0)
				{
					if(scanf("%f", &b) == 1 && b > 0)
					{
						if(scanf("%f", &c) == 1 && c > 0)
						{
							returned = is_triangle_valid(a, b, c);
							if(returned == -1) printf("-----------------------------------------------------------------\nERROR! Please enter a valid triangle\n-----------------------------------------------------------------\n");
							else exit = 1;
						}
						else
						{
							flag = 0;
							printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
							while (flag == 0)
								if(getchar() == 10) flag = 1;
						}
					}
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_perimeter = a+b+c;
			printf("-----------------------------------------------------------------\n:: Perimeter of the triangle: %.2f\n", calc_perimeter);
			break;
		case 3: /* case VOLUME */
			return -998; /* "volume of a triangle cannot be calculated" error code */
	}
	return 1;
} /* END of calc_triangle */
int calc_quadrilateral(int calc)
{
	int exit = 0, returned, flag = 0;
	float a, b, c, d, s, calc_area, calc_perimeter;
	switch(calc)
	{
		case 1: /* case AREA */
			/* In this while, we'll read scanf's return value for every side and if input is not valid, then we'll return error immediately and ask again for input */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf("--> Please enter four sides of the quadrilateral:\n");
				if(scanf("%f", &a) == 1 && a > 0)
				{
					if(scanf("%f", &b) == 1 && b > 0)
					{
						if(scanf("%f", &c) == 1 && c > 0)
						{
							if(scanf("%f", &d) == 1 && d > 0)
							{	/* If all inputs are valid, then we'll check for if these sides can make a quadrilateral */
								returned = is_quadrilateral_valid(a, b, c, d);
								if(returned == -1) printf("-----------------------------------------------------------------\nERROR! Please enter a valid quadrilateral\n-----------------------------------------------------------------\n");
								else exit = 1; /* If inputted sides can make a quadrilateral, then quit while */
							}
							else
							{
								flag = 0;
								printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
								while (flag == 0)
									if(getchar() == 10) flag = 1;
							}
						}
						else
						{
							flag = 0;
							printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
							while (flag == 0)
								if(getchar() == 10) flag = 1;
						}
					}
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			s = (a+b+c+d)/2.0; /* semi perimeter */
			calc_area = sqrt((s-a)*(s-b)*(s-c)*(s-d)); /* Brahmagupta's formula */
			printf("-----------------------------------------------------------------\n:: Area of the quadrilateral: %.2f\n", calc_area);
			break;
		case 2: /* case PERIMETER */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf("--> Please enter four sides of the quadrilateral:\n");
				if(scanf("%f", &a) == 1 && a > 0)
				{
					if(scanf("%f", &b) == 1 && b > 0)
					{
						if(scanf("%f", &c) == 1 && c > 0)
						{
							if(scanf("%f", &d) == 1 && d > 0)
							{
								returned = is_quadrilateral_valid(a, b, c, d);
								if(returned == -1) printf("-----------------------------------------------------------------\nERROR! Please enter a valid quadrilateral\n-----------------------------------------------------------------\n");
								else exit = 1;
							}
							else
							{
								flag = 0;
								printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
								while (flag == 0)
									if(getchar() == 10) flag = 1;
							}
						}
						else
						{
							flag = 0;
							printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
							while (flag == 0)
								if(getchar() == 10) flag = 1;
						}
					}
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_perimeter = a+b+c+d;
			printf("-----------------------------------------------------------------\n:: Perimeter of the quadrilateral: %.2f\n", calc_perimeter);
			break;
		case 3: /* case VOLUME */
			return -997; /* "volume of a quadrilateral cannot be calculated" error code */
	}
	return 1;
} /* END of calc_quadrilateral */
int calc_circle(int calc)
{
	int exit = 0, flag = 0;
	float radius, calc_area, calc_perimeter;
	switch(calc)
	{
		case 1: /* case AREA */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf("--> Please enter the radius of the circle: ");
				if(scanf("%f", &radius) == 1 && radius > 0) exit = 1;
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_area = PI*radius*radius;
			printf("-----------------------------------------------------------------\n:: Area of the circle: %.2f\n", calc_area);
			break;
		case 2: /* case PERIMETER */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf("--> Please enter the radius of the circle: ");
				if(scanf("%f", &radius) == 1 && radius > 0) exit = 1;
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_perimeter = 2*PI*radius;
			printf("-----------------------------------------------------------------\n:: Perimeter of the circle: %.2f\n", calc_perimeter);
			break;
		case 3: /* case VOLUME */
			return -996; /* "volume of a circle cannot be calculated" error code */
	}
	return 1;
} /* END of calc_circle */
int calc_pyramid(int calc)
{
	int exit = 0, flag = 0;
	float base, height, slant_height, calc_base_area, calc_lateral_area, calc_area, calc_base_perimeter, calc_lateral_perimeter, calc_perimeter, calc_volume;
	switch(calc)
	{
		case 1: /* case AREA */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Please enter the base side and slant height of the pyramid:\n--> Base side: ");
				if(scanf("%f", &base) == 1 && base > 0)
				{
					printf("--> Slant height: ");
					if(scanf("%f", &slant_height) == 1 && slant_height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_base_area = base*base;
			calc_lateral_area = 2*base*slant_height;
			calc_area = calc_base_area+calc_lateral_area;
			printf("-----------------------------------------------------------------\n:: Base surface area of the pyramid\t: %.2f\n:: Lateral surface area of the pyramid\t: %.2f\n:: Total surface area of the pyramid\t: %.2f\n", calc_base_area, calc_lateral_area, calc_area);
			break;
		case 2: /* case PERIMETER */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Please enter the base side and slant height of the pyramid:\n--> Base side: ");
				if(scanf("%f", &base) == 1 && base > 0)
				{
					printf("--> Slant height: ");
					if(scanf("%f", &slant_height) == 1 && slant_height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_base_perimeter = 4*base;
			calc_lateral_perimeter = 4*sqrt((base/2.0)*(base/2.0)+(slant_height*slant_height));
			calc_perimeter = calc_base_perimeter+calc_lateral_perimeter;
			printf("-----------------------------------------------------------------\n:: Base perimeter of the pyramid\t: %.2f\n:: Lateral perimeter of the pyramid\t: %.2f\n:: Total perimeter of the pyramid\t: %.2f\n", calc_base_perimeter, calc_lateral_perimeter, calc_perimeter);
			break;
		case 3: /* case VOLUME */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Please enter the base side and height of the pyramid:\n--> Base side: ");
				if(scanf("%f", &base) == 1 && base > 0)
				{
					printf("--> Height: ");
					if(scanf("%f", &height) == 1 && height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_volume = (1.0/3.0)*base*base*height;
			printf("-----------------------------------------------------------------\n:: Volume of the pyramid: %.2f\n", calc_volume);
			break;
	}
	return 1;
} /* END of calc_pyramid */
int calc_cylinder(int calc)
{
	int exit = 0, flag = 0;
	float radius, height, calc_base_area, calc_lateral_area, calc_base_perimeter, calc_lateral_perimeter, calc_area, calc_perimeter, calc_volume;
	switch(calc)
	{
		case 1: /* case AREA */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Enter the radius and height of the cylinder:\n");
				printf("--> Radius: ");
				if(scanf("%f", &radius) == 1 && radius > 0)
				{
					printf("--> Height: ");
					if(scanf("%f", &height) == 1 && height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_base_area = PI*radius*radius;
			calc_lateral_area = 2*PI*radius*height;
			calc_area = 2*PI*radius*(radius+height);
			printf("-----------------------------------------------------------------\n:: Base surface area of the cylinder\t: %.2f\n:: Lateral surface area of the cylinder\t: %.2f\n:: Total surface area of the cylinder\t: %.2f\n", calc_base_area, calc_lateral_area, calc_area);
			break;
		case 2: /* case PERIMETER */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Enter the radius and height of the cylinder:\n");
				printf("--> Radius: ");
				if(scanf("%f", &radius) == 1 && radius > 0)
				{
					printf("--> Height: ");
					if(scanf("%f", &height) == 1 && height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_base_perimeter = 2*PI*radius;
			calc_lateral_perimeter = 2*PI*radius*height;
			calc_perimeter = 2*calc_base_perimeter+calc_lateral_perimeter;
			printf("-----------------------------------------------------------------\n:: Base perimeter of the cylinder\t: %.2f\n:: Lateral perimeter of the cylinder\t: %.2f\n:: Total perimeter of the cylinder\t: %.2f\n", calc_base_perimeter, calc_lateral_perimeter, calc_perimeter);
			break;
		case 3: /* case VOLUME */
			/* for detailed explanation please look at SHAPE: TRIANGLE --> CASE 1 */
			while (exit == 0)
			{
				printf(" $$ Enter the radius and height of the cylinder:\n");
				printf("--> Radius: ");
				if(scanf("%f", &radius) == 1 && radius > 0)
				{
					printf("--> Height: ");
					if(scanf("%f", &height) == 1 && height > 0) exit = 1;
					else
					{
						flag = 0;
						printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
						while (flag == 0)
							if(getchar() == 10) flag = 1;
					}
				}
				else
				{
					flag = 0;
					printf("-----------------------------------------------------------------\nERROR! Please enter a valid entry\n-----------------------------------------------------------------\n");
					while (flag == 0)
						if(getchar() == 10) flag = 1;
				}
			}
			calc_volume = PI*radius*radius*height;
			printf("-----------------------------------------------------------------\n:: Volume of the cylinder: %.2f\n", calc_volume);
			break;
	}
	return 1;							
} /* END of calc_cylinder */
int calculate(int (*select_shape)(), int (*select_calc)())
{
	enum Shapes shape;
	enum Calculators calc;
	int returned;
	printf("=================================================================\n\t\tWelcome to Geometric Calculator!\n=================================================================\n");
	/* We'll cal select_shape for validity check --- If it is one of the enum types, it is valid; if it is not, return error code to main() */
	shape = select_shape();
	switch(shape)
	{
		case 0:
			return 0;
		case triangle:
		case quadrilateral:
		case circle:
		case pyramid:
		case cylinder:
			break;
		default:
			return -1;
	}
	calc = select_calc(); /* validity check for select_calc */
	switch(calc)
	{
		case 0:
			return 0;
		case area:
		case perimeter:
		case volume:
			break;
		default:
			return -1;
	}
	/* If select_shape and select_calc cannot pass validity check, then main() will ask for input till they are valid */
	/* After validity check for both funct., we'll use valid inputs for operations */
	switch(shape)
	{
		case triangle:
			returned = calc_triangle(calc);
			break;
		case quadrilateral:
			returned = calc_quadrilateral(calc);
			break;
		case circle:
			returned = calc_circle(calc);
			break;
		case pyramid:
			returned = calc_pyramid(calc);
			break;
		case cylinder:
			returned = calc_cylinder(calc);
			break;
	}
	return returned; /* We'll return error codes from calculation funct.s if there is any */
} /* END of calc_calculate */
int main()
{
	int exit = 0;
	while(exit == 0)
	{
		int returned = calculate(select_shape, select_calc);
		if(returned == 0) /* exit return */
		{			  
			printf("Exitting...\n=================================================================\n");
			exit = 1;
		}
		else if(returned == -1) printf("Invalid selection\n"); /* invalid selection for menu */
		else if(returned == -998) printf("ERROR! You cannot calculate the volume of a triangle\n");
		else if(returned == -997) printf("ERROR! You cannot calculate the volume of a quadrilateral\n");
		else if(returned == -996) printf("ERROR! You cannot calculate the volume of a circle\n");
	}
	return 0;
}
