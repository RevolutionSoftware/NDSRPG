/* File: main.c
 * Authors: Chickendude, add
 * Description: VOID */

#define DEBUG

// Includes NDS
#include <nds.h>	// Main NDS equates
#include <stdio.h>	// For console stuff
#include "tilemap.h"
#include "font.h"
#include "tiles.h"
#include "aux_macros.h"


// Equates
#define M_BASE0 0
#define M_BASE1 1
#define M_BASE2 2
#define M_BASE3 3
#define T_BASE0 0
#define T_BASE1 1

/* Each map entry is apparently 2 bytes, since a tileset can hold up
 * to 1024 tiles. Map is currently exactly same size as screen,
 * 32 8x8 tiles wide, 24 tiles tall. */
global_variable u16 map[] = {
// 0
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    8,9,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	10,11,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
// 1
    9,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,0,0,0,2,0,2,0,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,8,0,8,0,8,0,0,0,9,0,0,9,9,0,0,9,9,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,8,0,8,0,2,0,0,9,2,9,2,9,0,9,2,9,2,9,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,8,8,8,0,8,0,0,9,0,9,0,9,0,9,0,9,0,9,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,8,0,8,0,8,0,0,9,9,9,2,9,0,9,2,9,2,9,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,8,0,8,0,8,0,0,9,0,9,0,9,9,0,0,9,9,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,2,3,2,0,2,0,2,3,2,3,2,0,0,0,2,0,0,0,2,3,2,3,2,3,2,3,
    9,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	9,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
// 2
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
// 3
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,
    0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,5,0,1,0,1,0,1,0,1,0,1,0,1,
	2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,2,3,6,7,2,3,2,3,2,3,2,3,2,3,2,3,

};


int main(void) {
	int i;
	consoleDemoInit();
	iprintf("NDSRPG:\n");
	iprintf("4-bit color tilemap");

    /* NDS has nine memory banks, banks 0-4
     *  Use mode 0. Mode 0 is for tilebased sprites, called "text" mode
     * Other modes have options for rotation, scaling,
     * and bitmap display. You have access to 4 backgrounds in mode 0 */
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);   // Set mode 0 in 2D mode (not 3D)
    vramSetBankA(VRAM_A_MAIN_BG);					// There are nine memory banks, use memory bank A
    /* Initializes the background:
     * 	"Text" essentially means the same thing as "Tile"
     * Text4bpp means our tiles are 4bpp
     * static int bgInit (int layer (0-3), BgType type, BgSize size,
     *                    int mapBase (2kb offset in VRAM), int tileBase (16kb offset in VRAM))
     * There are other sizes available other than 256x256, eg. 512x256 and 512x512
     * I think that's what we'll use for scrolling our maps
     *		 layer, bg type,		 bg size,		   mapbase, tilebase */
	REG_BG0CNT = BG_PRIORITY_0 | BG_64x64 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
//	bg0 = bgInit(0, BgType_Text4bpp, BgSize_T_512x512, M_BASE0, T_BASE1);

    /* use DMA to copy data over
     * bgGetGfxPtr gets the uses the id from bgInit
     * (or bgInitSub, which works with the bottom screen) */
    // tile_ram is divided into blocks of 16kb, tileram(1) = tile_ram + 16kb
	dmaCopy(tilesTiles, BG_TILE_RAM(1), tilesTilesLen);
//	dmaCopy(map, bgGetMapPtr(bg0), sizeof(map));
	dmaCopy(tilesPal, BG_PALETTE, 256*2);

	int x = 0;
	int y = 0;

	tilemap(map, sizeof(map), x, y);
	
	while(1) {
        swiWaitForVBlank();
		// Check for keys now
		scanKeys();
		// Exit if Start was pressed
		if (keysDown()&KEY_START)
            break;
		if (keysDown()&KEY_A) iprintf("\nYou pressed A");
		if (keysUp()&KEY_A) iprintf("\nYou released A");
		i = keysHeld();
		if (i) {
			if (i&KEY_RIGHT) x++;
			if (i&KEY_LEFT) x--;
			if (i&KEY_DOWN) y++;
			if (i&KEY_UP) y--;
			REG_BG0HOFS = x;
			REG_BG0VOFS = y;
		}

	}

	return 0;
}
