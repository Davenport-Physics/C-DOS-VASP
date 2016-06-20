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

static int *PartialList       = NULL;
static int  PartialListLength = 0;

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
	
	
	
}

int *GetPartialList() {
	
	return PartialList;
	
}

int GetPartialListLength() {
	
	return PartialListLength;
	
}

void free_ArgResources() {
	
	
	
}
