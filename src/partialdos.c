/*
 * partialdos.c
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
#include <string.h>

#include "partialdos.h"
#include "share.h"

static double ***PartialDosArray = NULL;

void initialize_PartialDosArray() {

	int y, x;
	int TotalNumberOfAtoms   = GetTotalNumberOfAtoms();
	int IterationsPerSection = GetIterationsPerSection();
	
	PartialDosArray = malloc(TotalNumberOfAtoms * sizeof(double **));
	for (y = 0;y < TotalNumberOfAtoms;y++) {
	
		PartialDosArray[y] = malloc(IterationsPerSection*sizeof(double *));
		
	}
	for (y = 0;y < TotalNumberOfAtoms;y++) {
	
		for (x = 0;x < IterationsPerSection;x++) {
		
			PartialDosArray[y][x] = malloc(7 * sizeof(double));
		
		}
		
	}

}

void ReadPartialDos() {

	int z,y,x;
	int TotalNumberOfAtoms   = GetTotalNumberOfAtoms();
	int IterationsPerSection = GetIterationsPerSection();
	
	char buffer[256];
	char TempStrings[7][256];
	
	FILE *DOSCAR_fp = GetDoscarFilePointer();
	
	double FermiEnergy = GetFermiEnergy();
	for (z = 0;z < TotalNumberOfAtoms;z++) {
		
		fgets(buffer, 256, DOSCAR_fp);
		for (y = 0;y < IterationsPerSection;y++) {
			
			fscanf(DOSCAR_fp, "%s%s%s%s%s%s%s",
					TempStrings[0],
					TempStrings[1],
					TempStrings[2],
					TempStrings[3],
					TempStrings[4],
					TempStrings[5],
					TempStrings[6]);
		
			for (x = 0;x < 7;x++) {
				
				PartialDosArray[z][y][x] = GetCorrectString(TempStrings[x]);
				PartialDosArray[z][y][x] -= FermiEnergy;
				
			}
			
		}
		
	}
	
}

void WritePartialDos() {
	
	int TotalNumberOfAtoms   = GetTotalNumberOfAtoms();
	int IterationsPerSection = GetIterationsPerSection();
	
	char StringInt[10];
	char buffer[512];
	char *PartialDosFile = GetPartialDosFile();
	
	int z;
	for (z = 0;z < TotalNumberOfAtoms;z++) {
		
		strncpy(buffer, PartialDosFile, 256);
		sprintf(StringInt, "%d", z);
		strncat(buffer, StringInt, 10);
		
		FILE *fp = fopen(buffer, "w");
		
		int y, x;
		for (y = 0;y < IterationsPerSection;y++) {
		
			for (x = 0;x < 7;x++) {
			
				if ((x+1) != 7)
					fprintf(fp, "%lf\t", PartialDosArray[z][y][x]);
				else
					fprintf(fp, "%lf", PartialDosArray[z][y][x]);
				
			}
			if ((y+1) != IterationsPerSection)
				fprintf(fp, "\n");
				
			
		}
		
		fclose(fp);
		
	}
	
}

void free_PartialDosArray() {
	
	free(PartialDosArray);
	
}
