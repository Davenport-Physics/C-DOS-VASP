/*
 * arg.c
 * 
 * Copyright 2016 Emma Davenport <Davenport.Physics@gmail.com>
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

#include "arg.h"
#include "share.h"

#define ARGLISTLENGTH 1

static void arg_add(int argc, char *argv[], int index);
static void SetAddedPartialDosString(char *string);

static int *PartialList       = NULL;
static int  PartialListLength = 0;

static char *AddedPartialDosString = NULL;

static const StringFunction ArgList[ARGLISTLENGTH] = 
{{"-add", &arg_add}};


void ParseArgs(int argc, char *argv[]) {
	
	int y,x;
	for (y = 0;y < argc;y++) {
	
		for (x = 0;x < ARGLISTLENGTH;x++) {
		
			if (strcasecmp(ArgList[x].string, argv[y]) == 0) {
			
				ArgList[x].Function(argc, argv, y);
				
			}
			
		}
		
	}
	
}

static void arg_add(int argc, char *argv[], int index) {
	
	if ((index+1) >= argc) {
	
		printf("Error: Did you pass appropriate arugments?\n");
		printf("Halting Execution\n");
		exit(1);
		
	}
	
	/*
	 * The following three lines of code were initially at the end of
	 * the function, however strtok alters the value of argv[index+1]
	 * apparently, leading to some bug issues down the line.
	 * 
	 * Placing at the beginning doesn't fundamentally change how the
	 * program is ran, since if the user didn't pass a good argument
	 * the program halts regardless.
	 * 
	 * */
	AddedPartialDosString = malloc(512 * sizeof(char));
	memset(AddedPartialDosString,0, 512);
	SetAddedPartialDosString(argv[index+1]);
	
	char *DelimeterPointer = (char *)memchr(argv[index+1], '-', strlen(argv[index]));
	
	CheckForNullPointer(DelimeterPointer, 
						"Error: Didn't pass appropriate argument\n",
						TRUE);
						
	int start = 0, end = 0;
	char *TempPointer = strtok(argv[index+1], "-");
	CheckForNullPointer(TempPointer, 
						"Something went wrong with strtok 1",
						TRUE);

	start = atoi(TempPointer);
	TempPointer = strtok(NULL, "-");
	CheckForNullPointer(TempPointer, 
						"Something went wrong with strtok 2",
						TRUE);
	
	end = atoi(TempPointer);
	
	PartialListLength = end - start;
	PartialList       = malloc(PartialListLength * sizeof(int));
	
	int i;
	for (i = 0;i < PartialListLength;i++)
		PartialList[i] = (start+i);
		
}
static void SetAddedPartialDosString(char *string) {
	
	printf("AddedPartialDosString = %s\n", string);
	strncpy(AddedPartialDosString, string, strlen(string));
	
}

int *GetPartialList() {
	
	return PartialList;
	
}

int GetPartialListLength() {
	
	return PartialListLength;
	
}
char *GetAddedPartialDosString() {
	
	return AddedPartialDosString;
	
}

void free_ArgResources() {
	
	free(PartialList);
	free(AddedPartialDosString);
}
