/***************************************************************************************
Content: SOL1, Lab3, Exercise 1
Lecturer: Prof. Schoenen
Authors: Hilary Ogalagu, HAW Hamburg
Date: 20201216
Short description: Angle
****************************************************************************************/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>

#define _USE_MATH_DEFINES /* for M_PI */
#define PI 3.14159
#define COLUMNS 80
#define ROWS 40
#define DEBUG 0 /* 1==true to give a lot of output */
//#define DEBUG 0
#define FIXED_DEV 0.8
#define XMIN -PI
#define XMAX +PI
#define XSTEP (2*PI/ROWS)
#define XEXTRA 0.01
#define getch _getch



double norm_angle(double x);
double sinus(double x, double deviation);
void x_rad_con_fixed_dev(double x);
void do_plotting_frontend(); // prototype
void plot_function_by_key(char ch);
void plot_value(double value, double y_min, double y_max);
double zero_point(double lower, double upper);
double f(double x);
double d(double x, double h);
double extreme_value(double lower, double upper);


int main(void) {
	double x;
	for (x = -10; x < 10.1; x += 2.0) {
		printf(" The angle %12.8lf ( radiants ) corresponds to %12.8lf\n",
			x, norm_angle(x));
	}
	printf("\n");

	for (x = -PI; x <= PI+0.01; x += PI / 10) {
		printf("sinus(%lf) = %lf\n", x, sinus(x, 1e-6));
	}
	
	do_plotting_frontend();

	printf("Zero_point of Cosine : %15.10lf \n", zero_point(1.0, 2.0));
	//zero_point(1.0, 2.0);
	printf("Extrem: %lf", extreme_value(1.0, 2.0));
	
	return 0;
}

//Exercise 1.a
double norm_angle(double x) {
	double norm = x;
	while (norm > PI|| norm < -PI) {//Loop to set the input x into [pi,-pi]
		if (norm > PI)
			norm = -2 * PI + norm;
		else if (norm < -PI)
			norm = 2 * PI + norm;
	}
	return norm;
}

// Exercise 1b
double sinus(double x, double deviation) {
	x = norm_angle(x);
	double sk = x;
	double sum = sk;
	int k = 1;
	while (sk > 0 && sk > deviation || sk < 0 && sk < -deviation) {
		sk = sk * (-x * x / (2 * k * (2 * k + 1)));
		sum += sk;
		k++;
	}
	
	return sum;
}

//Exercise 1c
double f(double x) {
	return cos(x);//sinus(x,1e-6);
}


void do_plotting_frontend() {
	if (DEBUG) printf("### do_plotting_frontend()\n");
	char ch; // character from keyboard
	int key_pressed = ' '; // keyboard key value
	// loop:
	while (key_pressed != '.') {
		printf("Please select function to plot (keys a..z, .=quit)\n");
		key_pressed = getch();
		ch = key_pressed; // char <- int conversion
		if (DEBUG) printf("Key pressed was '%c'=%04d=0x%04X\n", ch, key_pressed, key_pressed);
		plot_function_by_key(ch);
	}
	if (DEBUG) printf("### do_plotting_frontend() finished.\n");
}

void plot_function_by_key(char ch) {
	if (DEBUG) printf("### plot_function_by_key('%c')\n", ch);
	int index = 0;
	double x, y, y_min = 0.0, y_max = 0.0;
	double x_array[100], y_array[100];
	// loop over range of x values:
	for (x = XMIN; x <= XMAX + XEXTRA; x = x + XSTEP) { // #steps is ROWS
		switch (ch) { // depend on key pressed
		case 's': y = sin(x); break;
		case 'c': y = cos(x); break;
		case 't': y = tan(x); break;
		case 'f': y = f(x); break;
		case 'l': y = x; break;
		case 'q': y = x * x; break;
		default: y = 0;
		}
		//if (DEBUG) printf("x=%.3lf y=%.3lf\n", x, y);
		x_array[index] = x; // store into table
		y_array[index] = y; // store into table
		y_min = (y < y_min) ? y : y_min;
		y_max = (y > y_max) ? y : y_max;
		index++; // increase the index
	}
	int num_values = index; // so many elements in the table
	if (DEBUG) printf("num_values=%d, y_min=%.3f, y_max=%.3f\n", num_values, y_min, y_max);
	for (index = 0; index < num_values; index++) { // #steps is ROWS
		x = x_array[index]; // read out values from table
		y = y_array[index];
		//if (DEBUG) printf("x=%.3lf y=%.3lf\n", x, y);
		printf("x=%6.3lf ", x);
		plot_value(y, y_min, y_max);
	}
	if (DEBUG) printf("### plot_function_by_key() finished.\n");
}

void plot_value(double value, double y_min, double y_max) {
	double range = y_max - y_min;
	int column = (value - y_min) / (range) * (COLUMNS - 10); // between 0 .. 80
	for (int i = 0; i < column; i++) {
		printf(" ");
	}
	printf("*\n");
}

double zero_point(double lower, double upper) {
	double middle, fmid, flow, fup, absdiff, maxdiff;
	flow = 1;
	maxdiff = 0.00000001;
	
	while (flow * flow > maxdiff * maxdiff) {
		if (DEBUG) printf("Lower: %lf Upper: %lf\n", lower, upper);		//DEBUG		//display lower and upper only in debbug mode
		flow = f(lower);
		fup = f(upper);
		if (DEBUG) printf("F lower: %lf F upper: %lf\n", flow, fup);	//DEBUG		//display flow and fup only in debbug mode
		absdiff = fabs(fabs(flow) - fabs(fup));
		if (DEBUG) printf("Absolute diff: %lf\n", absdiff);				//DEBUG		//display absdiff only in debbug mode

		middle = (lower + upper) / 2;
		if (DEBUG) printf("Middle: %15.10lf\n", middle);				//DEBUG		//display MIDDLE only in debbug mode
		fmid = f(middle);
		if (DEBUG) printf("F middle: %lf\n\n\n", fmid);					//DEBUG		//display fmid only in debbug mode

		if (fmid > 0)
			lower = middle;
		else
			upper = middle;
	}
	return middle;
}
double d(double x, double h) {
	return (f(x + h) - f(x - h)) / (2 * h);		//Derivated of a the function f
}

double extreme_value(double lower, double upper) {
	double mid, cond;
	mid = (lower + upper) / 2;
	cond = 0.00000001;

	while ((upper - lower) * (upper - lower) >= cond * cond) {
		mid = (lower + upper) / 2;
		if (d(lower, cond) * d(mid, cond) > 0) {
			lower = mid;
		}
		else {
			upper = mid;
		}
	}
	return mid;
}
/*double zero_point(double lower, double upper) {
	int Iteration = 6;
	printf("Iteration | lower | upper || f(lower) | f(upper) || middle | f(middle) \n");
	while (lower > 6 && lower > upper || lower < 0 && lower < -upper) {//Loop to set the input x into [pi,-pi]
		if (lower > PI)
			printf("%d",Iteration);
		    printf("%lf", lower);
		//else if (lower < -PI)
			//norm = 2 * PI + norm;
	}
	return zero_point(0,2); 
	
}*/