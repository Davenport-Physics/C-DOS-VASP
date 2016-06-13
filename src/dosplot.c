/*
 * dosplot.c
 * 
 * Copyright 2016 Emma Davenport <Davenport.physics@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>

static char TotalDosFile[256] = "TotalDos";

static int IterationsPerSection = 0;

static double FermiEnergy     = 0.0;
static double **TotalDosArray = NULL;

static FILE *DOSCAR_fp;

void initialize();
void GetTotalDos();
void WriteTotalDos();

int main(int argc, char **argv)
{
	
	initialize();
	GetTotalDos();
	WriteTotalDos();
	
	free(TotalDosArray);
	fclose(DOSCAR_fp);
	
	return 0;
}

void initialize() {
	
	DOSCAR_fp = fopen("DOSCAR", "r");
	
	if (DOSCAR_fp == NULL) {
	
		printf("DOSCAR file not found. Halting execution.\n");
		exit(1);
		
	}
	
	int i;
	char buffer[256];
	for (i = 0;i < 5;i++)
		fgets(buffer, 256, DOSCAR_fp);
		
	fscanf(DOSCAR_fp, "%*f %*f %d %lf %*f\n", &IterationsPerSection, &FermiEnergy);
	
	TotalDosArray = malloc(IterationsPerSection * sizeof(double *));
	
	for (i = 0; i < IterationsPerSection;i++) {
	
		TotalDosArray[i] = malloc(3 * sizeof(double));
		
	}
	
}

void GetTotalDos() {
	
	int i;
	for (i = 0;i < IterationsPerSection;i++) {
	
		fscanf(DOSCAR_fp, "%lf %lf %lf %*f %*f\n", &TotalDosArray[i][0], &TotalDosArray[i][1], &TotalDosArray[i][2]);
		TotalDosArray[i][0] -= FermiEnergy;
		TotalDosArray[i][2]  = -TotalDosArray[i][2]; //Spin-down is considered negative
	}
	
}

void WriteTotalDos() {

	FILE *fp = fopen(TotalDosFile, "w");
	
	if (fp == NULL) {
	
		printf("Couldn't open up %s. Halting Execution.\n", TotalDosFile);
		exit(1);
		
	}
	
	int i;
	for (i = 0;i < IterationsPerSection;i++) {
	
		if ((i + 1) < IterationsPerSection) {
			
			fprintf(fp, "%lf\t%lf\t%lf\n", TotalDosArray[i][0], TotalDosArray[i][1], TotalDosArray[i][2]);
			
		} else {
			
			fprintf(fp, "%lf\t%lf\t%lf", TotalDosArray[i][0], TotalDosArray[i][1], TotalDosArray[i][2]);
			
		}
		
	}
	
}
