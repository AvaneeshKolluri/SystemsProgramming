/*******************************************************************************
 * Name        : pfind.h
 * Author      :  Avaneesh Kolluri
 * Date        :  2/24/2020
 * Description : pfind header.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
/**
 * TODO - put all non-static function prototypes from pfind.c inside
 * wrapper #ifndef.
 */
#ifndef HELP_H
#define HELP_H

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encFiles(char *path, unsigned int permCode, struct stat *statbuf, int dirLength, char *perm_str);

#endif

