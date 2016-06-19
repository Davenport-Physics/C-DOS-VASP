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
#include <string.h>

typedef enum {
	
	FALSE = 0,
	TRUE
	
} bool;

static char TotalDosFile[256]   = "doscar-files/TotalDos";
static char PartialDosFile[256] = "doscar-files/PartialDosFile";

static int IterationsPerSection = 0;
static int TotalNumberOfAtoms   = 0;

static double FermiEnergy        = 0.0;
static double **TotalDosArray    = NULL;
static double ***PartialDosArray = NULL;

static FILE *DOSCAR_fp;

void initialize();
void initializeTotalDosArray();
void initializePartialDosArray();
void initializeNumberOfAtoms();
void MakeDosDirectories();

void GetTotalDos();
void GetPartialDos();
void GetTotalNumberOfAtoms(char buffer[256]);

void WriteTotalDos();
void WritePartialDos();

bool CheckIfStringContainsNumber(char buffer[256]);

int main(int argc, char **argv)
{
	
	initialize();
	
	GetTotalDos();
	GetPartialDos();
	
	WriteTotalDos();
	WritePartialDos();
	
	free(TotalDosArray);
	free(PartialDosArray);
	
	fclose(DOSCAR_fp);
	
	return 0;
}

void initialize() {
	
	DOSCAR_fp = fopen("DOSCAR", "r");
	
	if (DOSCAR_fp == NULL) {
	
		printf("DOSCAR file not found. Halting execution.\n");
		exit(1);
		
	}
	
	MakeDosDirectories();
	
	int i;
	char buffer[256];
	for (i = 0;i < 5;i++)
		fgets(buffer, 256, DOSCAR_fp);
		
	fscanf(DOSCAR_fp, "%*f %*f %d %lf %*f\n", &IterationsPerSection, &FermiEnergy);
	
	initializeTotalDosArray();
	initializeNumberOfAtoms();
	initializePartialDosArray();
	
}

void initializeTotalDosArray() {
	
	TotalDosArray = malloc(IterationsPerSection * sizeof(double *));
	int i;
	for (i = 0; i < IterationsPerSection;i++) {
	
		TotalDosArray[i] = malloc(3 * sizeof(double));
		
	}
	
	
}

void initializePartialDosArray() {
	
	int y, x;
	
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
void initializeNumberOfAtoms() {
	
	FILE *fp = fopen("POSCAR", "r");
	
	char buffer[256];
	
	int i;
	for (i = 0;i < 5;i++)
		fgets(buffer, 256, fp);
		
	fgets(buffer, 256, fp);
	if (CheckIfStringContainsNumber(buffer) == TRUE) {
		
		GetTotalNumberOfAtoms(buffer);
		
	} else {
	
		fgets(buffer, 256, fp);
		GetTotalNumberOfAtoms(buffer);
		
	}	
	fclose(fp);

}

void MakeDosDirectories() {

	FILE *fp = popen("mkdir doscar-files", "r");
	
	pclose(fp);
	
}

void GetTotalDos() {
	
	int i;
	for (i = 0;i < IterationsPerSection;i++) {
	
		fscanf(DOSCAR_fp, "%lf %lf %lf %*f %*f\n", &TotalDosArray[i][0], &TotalDosArray[i][1], &TotalDosArray[i][2]);
		TotalDosArray[i][0] -= FermiEnergy;
		TotalDosArray[i][2]  = -TotalDosArray[i][2]; //Spin-down is considered negative
	}
	
}

void GetPartialDos() {

	int z,y;
	
	char buffer[256];
	for (z = 0;z < TotalNumberOfAtoms;z++) {
		
		fgets(buffer, 256, DOSCAR_fp);
		for (y = 0;y < IterationsPerSection;y++) {
		
			fscanf(DOSCAR_fp, "%lf%lf%lf%lf%lf%lf%lf", 
					&PartialDosArray[z][y][0],
					&PartialDosArray[z][y][1],
					&PartialDosArray[z][y][2],
					&PartialDosArray[z][y][3],
					&PartialDosArray[z][y][4],
					&PartialDosArray[z][y][5],
					&PartialDosArray[z][y][6]);
			
		}
		
	}
	
	
}

void GetTotalNumberOfAtoms(char buffer[256]) {

	char *pointer = strtok(buffer, " ");
	
	while (pointer != NULL) {
	
		TotalNumberOfAtoms += atoi(pointer);
		pointer = strtok(NULL, " ");
		
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
void WritePartialDos() {
	
	char StringInt[10];
	char buffer[512];
	
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

bool CheckIfStringContainsNumber(char buffer[256]) {
	
	char *pointer = strtok(buffer, " ");
	
	if (atoi(pointer) != 0)
		return TRUE;
	else
		return FALSE;
	
}
