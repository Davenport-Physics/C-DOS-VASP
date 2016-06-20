/*
 * totaldos.c
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

#include "totaldos.h"
#include "share.h"

static double **TotalDosArray = NULL;

void initialize_TotalDosArray() {
	
	int IterationsPerSection = GetIterationsPerSection();
	
	TotalDosArray = malloc(IterationsPerSection * sizeof(double *));
	int i;
	for (i = 0; i < IterationsPerSection;i++) {
	
		TotalDosArray[i] = malloc(3 * sizeof(double));
		
	}
	
}

void ReadTotalDos() {
	
	FILE   *DOSCAR_fp           = GetDoscarFilePointer();
	int    IterationsPerSection = GetIterationsPerSection();
	double FermiEnergy          = GetFermiEnergy();

	int i,x;
	char TempStrings[3][256];
	for (i = 0;i < IterationsPerSection;i++) {
		
		fscanf(DOSCAR_fp, "%lf %s %s %*s %*s\n", &TotalDosArray[i][0], TempStrings[1], TempStrings[2]);
		
		for (x = 1;x < 3;x++) {
		
			TotalDosArray[i][x] = GetCorrectString(TempStrings[x]);
			
		}
		TotalDosArray[i][0] -= FermiEnergy;
		TotalDosArray[i][2]  = -TotalDosArray[i][2]; //Spin-down is considered negative
	}	
	
}

void WriteTotalDos() {
	
	char *TotalDosFile = GetTotalDosFile();

	FILE *fp = fopen(TotalDosFile, "w");
	
	if (fp == NULL) {
	
		printf("Couldn't open up %s. Halting Execution.\n", TotalDosFile);
		exit(1);
		
	}
	
	int IterationsPerSection = GetIterationsPerSection();
	
	int i;
	for (i = 0;i < IterationsPerSection;i++) {
	
		if ((i + 1) < IterationsPerSection) {
			
			fprintf(fp, "%lf\t%lf\t%lf\n", TotalDosArray[i][0], TotalDosArray[i][1], TotalDosArray[i][2]);
			
		} else {
			
			fprintf(fp, "%lf\t%lf\t%lf", TotalDosArray[i][0], TotalDosArray[i][1], TotalDosArray[i][2]);
			
		}
		
	}
	
	fclose(fp);	
	
}

void free_TotalDosArray() {

	free(TotalDosArray);
	
}
