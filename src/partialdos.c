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
#include "arg.h"
#include "share.h"

static double ***PartialDosArray = NULL;

static void WriteAddedPartialDos(double **AddPartialDosArray, int IterationsPerSection);

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
			
			fscanf(DOSCAR_fp, "%lf%s%s%s%s%s%s\n",
					&PartialDosArray[z][y][0],
					TempStrings[1],
					TempStrings[2],
					TempStrings[3],
					TempStrings[4],
					TempStrings[5],
					TempStrings[6]);
				
			PartialDosArray[z][y][0] -= FermiEnergy;
			for (x = 1;x < 7;x++) {
				
				PartialDosArray[z][y][x] = GetCorrectString(TempStrings[x]);
				
			}
			for (x = 2;x < 7;x+=2) {
			
				PartialDosArray[z][y][x] = -PartialDosArray[z][y][x];
				
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

void AddPartialDos(int *list, int list_length) {
	
	double **AddedPartialDosArray = NULL;
	
	int IterationsPerSection = GetIterationsPerSection();
	
	AddedPartialDosArray = malloc(IterationsPerSection * sizeof(double *));
	
	int y,x;
	for (y = 0;y < IterationsPerSection;y++) {
	
		AddedPartialDosArray[y]    = malloc(7 * sizeof(double));
		AddedPartialDosArray[y][0] = PartialDosArray[0][y][0];
		for (x = 1;x < 7;x++) {
			
			AddedPartialDosArray[y][x] = 0.0;
			
		}
	}
	
	int i;
	for (i = 0;i < list_length;i++) {
		
		int z = list[i];
		for (y = 0;y < IterationsPerSection;y++) {
		
			for (x = 1;x < 7;x++) {
				
				AddedPartialDosArray[y][x] += PartialDosArray[z][y][x];
				
			}
			
		}
		
	}
	
	WriteAddedPartialDos(AddedPartialDosArray, IterationsPerSection);
	
	free(AddedPartialDosArray);
	
}

static void WriteAddedPartialDos(double **AddedPartialDosArray, int IterationsPerSection) {

	char filename[512] = {'\0'};
	printf("AddedPartialDosString = %s\n", AddedPartialDosString);
#if __unix
	strncpy(filename, "doscar-files/AddedPartialDos (", 30);
#elif __WIN32
	strncpy(filename, "doscar-files\\AddedPartialDos (", 30);
#endif
	strncat(filename, AddedPartialDosString, strlen(AddedPartialDosString));
	strncat(filename, ")", 1);
	
	FILE *TempFilePointer = fopen(filename, "w");
	
	CheckForNullPointer(TempFilePointer, 
						"Could not write to AddedPartialDosFile", 
						TRUE);
						
	int y,x;
	for (y = 0;y < IterationsPerSection;y++) {
	
		for (x = 0;x < 7;x++) {
		
			if ((x + 1) != 7)
				fprintf(TempFilePointer, "%lf\t", AddedPartialDosArray[y][x]);
			else
				fprintf(TempFilePointer, "%lf", AddedPartialDosArray[y][x]);
			
		}
		if ((y + 1) != IterationsPerSection)
			fprintf(TempFilePointer, "\n");
		
	}
	
	fclose(TempFilePointer);
	
}

void free_PartialDosArray() {
	
	free(PartialDosArray);
	
}
