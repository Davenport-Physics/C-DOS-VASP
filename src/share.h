/*
 * share.h
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


#ifndef SHARE_H
#define SHARE_H

#include <stdio.h>

typedef enum {
	
	FALSE = 0,
	TRUE
	
} bool;

typedef struct {
	
	char *string;
	void (*Function)(int argc, char *argv[], int index);
	
} StringFunction;

void CheckForNullPointer(void *p, char *message, bool HaltExecution);

void SetFermiEnergy(double FE);
void SetStandardValues(int IPerSection,int TotNumberOfAtoms);

void  InitializeDoscarFilePointer(char *string, char *options);
FILE *GetDoscarFilePointer();
void  CloseDoscarFilePointer();

int    GetIterationsPerSection();
int    GetTotalNumberOfAtoms();
char  *GetTotalDosFile();
char  *GetPartialDosFile();
double GetFermiEnergy();
double GetCorrectString(char string[256]);


#endif

