/*
 * share.c
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

#include <stdlib.h>
#include <string.h>

#include "share.h"

static FILE *DOSCAR_fp = NULL;

static int IterationsPerSection = 0;
static int TotalNumberOfAtoms   = 0;

#if __unix
static char *TotalDosFile   = "doscar-files/TotalDos";
static char *PartialDosFile = "doscar-files/PartialDosFile";
#elif __WIN32
static char *TotalDosFile   = "doscar-files\\TotalDos";
static char *PartialDosFile = "doscar-files\\PartialDosFile";
#endif

static double FermiEnergy = 0.0;

void SetFermiEnergy(double FE) {
	
	if (FermiEnergy != 0.0)
		printf("Was SetFermiEnergy called more than once?\n");
	
	FermiEnergy = FE;
	
}

double GetFermiEnergy() {
	
	return FermiEnergy;
		
}

void InitializeDoscarFilePointer(char *string, char *options) {
	
	if (DOSCAR_fp != NULL)
		printf("Was InitializeDoscarFilePointer called more than once?\n");
	
	DOSCAR_fp = fopen(string, options);
	
}

void CloseDoscarFilePointer() {
	
	fclose(DOSCAR_fp);
	
}

FILE *GetDoscarFilePointer() {
	
	return DOSCAR_fp;
	
}

void SetStandardValues(int IPerSection,int TotNumberOfAtoms) {
	
	if (IterationsPerSection != 0)
		printf("Was SetStandardValues called more than once?\n");
	
	IterationsPerSection = IPerSection;
	TotalNumberOfAtoms   = TotNumberOfAtoms;
	
}

int GetIterationsPerSection() {
	
	return IterationsPerSection;
	
}

int GetTotalNumberOfAtoms() {
	
	return TotalNumberOfAtoms;
	
}

char *GetTotalDosFile() {
	
	return TotalDosFile;
	
}

char *GetPartialDosFile() {
	
	return PartialDosFile;
	
}

double GetCorrectString(char string[256]) {

	char *position = (char *)memchr(string, 'E', strlen(string));
	
	if (position == NULL) {
		
		char buffer[512];
		char *pointer = strtok(string, "+-");
		
		strncpy(buffer, pointer, strlen(pointer));
		strncat(buffer, "E", 1);
		pointer = strtok(NULL, "+-");
		strncat(buffer, pointer, strlen(pointer));
		
		return atof(buffer);
	}
	return atof(string);
	
}
