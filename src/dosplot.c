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

#include <sys/stat.h>

#include "share.h"
#include "totaldos.h"
#include "partialdos.h"

static char *ProgramVersion = ".1";

void InitializeProgramState();
void MakeDosDirectories();

int DetermineTotalNumberOfAtoms(char buffer[256]);
int initializeNumberOfAtoms();
bool CheckIfStringContainsNumber(char buffer[256]);

int main(int argc, char **argv)
{
	
	printf("Version %s", ProgramVersion);
	
	InitializeProgramState();
	
	ReadTotalDos();
	WriteTotalDos();
	
	ReadPartialDos();
	WritePartialDos();
	
	CloseDoscarFilePointer();
	free_TotalDosArray();
	free_PartialDosArray();
	
	return 0;
}

void InitializeProgramState() {
	
	InitializeDoscarFilePointer("DOSCAR", "r");
	FILE *DOSCAR_fp = GetDoscarFilePointer();
	
	if (DOSCAR_fp == NULL) {
	
		printf("DOSCAR file not found. Halting execution.\n");
		exit(1);
		
	}
	
	MakeDosDirectories();
	
	int i;
	char buffer[256];
	for (i = 0;i < 5;i++)
		fgets(buffer, 256, DOSCAR_fp);
		
	int IterationsPerSection;
	double FermiEnergy;
	fscanf(DOSCAR_fp, "%*f %*f %d %lf %*f\n", &IterationsPerSection, &FermiEnergy);
	
	SetFermiEnergy(FermiEnergy);
	SetStandardValues(IterationsPerSection, initializeNumberOfAtoms());
	
	initialize_TotalDosArray();
	initialize_PartialDosArray();
	
}

int initializeNumberOfAtoms() {
	
	FILE *fp = fopen("POSCAR", "r");
	
	char buffer[256];
	
	int i;
	for (i = 0;i < 5;i++)
		fgets(buffer, 256, fp);
		
	fgets(buffer, 256, fp);
	if (CheckIfStringContainsNumber(buffer) == TRUE) {
		
		fclose(fp);
		return DetermineTotalNumberOfAtoms(buffer);
		
	} else {
	
		fgets(buffer, 256, fp);
		fclose(fp);
		return DetermineTotalNumberOfAtoms(buffer);
		
	}	

}

void MakeDosDirectories() {

#if __unix
	mkdir("doscar-files", S_IRWXU | S_IRWXG | S_IRWXO);
#elif __WIN32
	mkdir("doscar-files");
#endif
	
}

int DetermineTotalNumberOfAtoms(char buffer[256]) {

	int TotalNumberOfAtoms = 0;

	char *pointer = strtok(buffer, " ");
	while (pointer != NULL) {
	
		TotalNumberOfAtoms += atoi(pointer);
		pointer = strtok(NULL, " ");
		
	}
	
	return TotalNumberOfAtoms;
	
}

bool CheckIfStringContainsNumber(char buffer[256]) {
	
	char *pointer = strtok(buffer, " ");
	
	if (atoi(pointer) != 0)
		return TRUE;
	else
		return FALSE;
	
}
