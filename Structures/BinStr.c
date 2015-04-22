/* BinStr.c - Written by Xenonstory on April 21, 2015 *
 * A structure used to store binary strings           */

#include "BinStr.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// see BinStr.h for details
BinStr create_BinStr(char *bits, unsigned int length) {
	assert(bits != NULL);
	BinStr new = malloc(sizeof(struct binstr));
	new->bits = malloc(sizeof(bool) * length);
	for(int i = 0; i < length; i++) {
		new->bits[i] = bits[i] - '0';
	}
	new->length = length;
	return new;
}

// see BinStr.h for details
BinStr empty_BinStr(unsigned int length) {
	BinStr new = malloc(sizeof(struct binstr));
	new->bits = malloc(sizeof(bool) * length);
	for(int i = 0; i < length; i++) {
		new->bits[i] = 0;
	}
	new->length = length;
	return new;
}

// see BinStr.h for details
void destroy_BinStr(BinStr str) {
	assert(str != NULL);
	free(str->bits);
	free(str);
	return;
}

// see BinStr.h for details
BinStr XOR(BinStr str1, BinStr str2) {
	assert(str1 != NULL && str2 != NULL && str1->length == str2->length);
	char bits[str1->length];
	for(int i = 0; i < str1->length; i++) {
		if(str1->bits[i] ^ str2->bits[i] == 0) {
			bits[i] = '0';
		} else {
			bits[i] = '1';
		}
	}
	BinStr new = create_BinStr(bits, str1->length);
	return new;
}

// see BinStr.h for details
char *toString(BinStr str) {
	char *new = malloc(sizeof(char) * (str->length + 1));
	for(int i = 0; i < str->length; i++) {
		new[i] = str->bits[i];
	}
	new[str->length] = '\0';
	return new;
}

// see BinStr.h for details
void print(BinStr str) {
	for(int i = 0; i < str->length; i++) {
		printf("%d", str->bits[i]);
	}
}
