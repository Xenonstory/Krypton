/* DES.c - Written by Xenonstory on June 24, 2015    *
 * An implementation of the Data Encryption Standard */

#include "DES.h"
#include <assert.h>
#include <stdlib.h>

const int DES_KEY_SIZE = 64;

const int DES_BLOCK_SIZE = 64;
const int DES_ROUND_KEY_SIZE = 48;
const int DES_ROUNDS = 16;

const int PC1_PERMUTATION_SIZE = 28;
int PC1_C_PERMUTATION[] = {57, 49, 41, 33, 25, 17,  9,
                            1, 58, 50, 42, 34, 26, 18,
                           10,  2, 59, 51, 43, 35, 27,
    	                   19, 11,  3, 60, 52, 44, 36};
int PC1_D_PERMUTATION[] = {63, 55, 47, 39, 31, 23, 15,
                            7, 62, 54, 46, 38, 30, 22,
                           14,  6, 61, 53, 45, 37, 29,
                           21, 13,  5, 28, 20, 12,  4};
int KEY_SHIFTS[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
int ready_round_key = 0;
BinStr key_block_C, key_block_D;

const int PC2_PERMUTATION_SIZE = 48;
int PC2_PERMUTATION[] = {14, 17, 11, 24,  1,  5,
                          3, 28, 15,  6, 21, 10,
                         23, 19, 12,  4, 26,  8,
                         16,  7, 27, 20, 13,  2,
                         41, 52, 31, 37, 47, 55,
                         30, 40, 51, 45, 33, 48,
                         44, 49, 39, 56, 34, 53,
                         46, 42, 50, 36, 29, 32}; 

const int I_PERMUTATION_SIZE = 64;
int I_PERMUTATION[] = {58, 50, 42, 34, 26, 18, 10, 2,
                       60, 52, 44, 36, 28, 20, 12, 4,
                       62, 54, 46, 38, 30, 22, 14, 6,
                       64, 56, 48, 40, 32, 24, 16, 8,
                       57, 49, 41, 33, 25, 17,  9, 1,
                       59, 51, 43, 35, 27, 19, 11, 3,
                       61, 53, 45, 37, 29, 21, 13, 5,
                       63, 55, 47, 39, 31, 23, 15, 7};

const int E_PERMUTATION_SIZE = 48;
int E_PERMUTATION[] = {32,  1,  2,  3,  4,  5,
                        4,  5,  6,  7,  8,  9,
       	                8,  9, 10, 11, 12, 13,
    	               12, 13, 14, 15, 16, 17,
                       16, 17, 18, 19, 20, 21,
                       20, 21, 22, 23, 24, 25,
                       24, 25, 26, 27, 28, 29,
                       28, 29, 30, 31, 32,  1};

const int P_PERMUTATION_SIZE = 32;
int P_PERMUTATION[] = {16,  7, 20, 21, 29, 12, 28, 17,
                        1, 15, 23, 26,  5, 18, 31, 10,
                        2,  8, 24, 14, 32, 27,  3,  9,
                       19, 13, 30,  6, 22, 11,  4, 25};

const int S_BOX_INPUT_SIZE = 48;
const int S_BOX_COLS = 16;
const int S_BOX_ROWS = 4;
int S_BOX[][64] = {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
                     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
                     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
                   {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
                     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
                    10, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
                    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
                   {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
                    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
                    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
                     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
                   { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
                    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    	       	    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
                     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
                   { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
                    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
                     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
                    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
                   {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
                    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
                     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
                     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
                   { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
                    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
                     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
                     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
                   {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
                     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
                     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

// cPermutation(key) returns the C permutation used in the key-expansion
// effects: allocates memory to a new BinStr
// requires: key is a valid BinStr and key->length == DES_KEY_SIZE
BinStr cPermutation(BinStr key) {
        assert(key != NULL && key->length == DES_KEY_SIZE);
        return permutate(key, PC1_C_PERMUTATION, PC1_PERMUTATION_SIZE, 1);
}

// dPermutation(key) returns the D permutation used in the key-expansion
// effects: allocates memory to a new BinStr
// requires: key is a valid BinStr and key->length == DES_KEY_SIZE
BinStr dPermutation(BinStr key) {
        assert(key != NULL && key->length == DES_KEY_SIZE);
        return permutate(key, PC1_D_PERMUTATION, PC1_PERMUTATION_SIZE, 1);
}

// rPermutation(key) returns the second permutation choice for round keys
// effects: allocates memory to a new BinStr
// requires: key is a valid BinStr and key->length == DES_ROUND_KEY_SIZE
BinStr rPermutation(BinStr key) {
	assert(key != NULL && key->length == PC1_PERMUTATION_SIZE * 2);
	return permutate(key, PC2_PERMUTATION, PC2_PERMUTATION_SIZE, 1);
}

// initializeKeyGenerator(key) initializes the "generator" for the various
//   round keys to be used by DES
// requires: key is a valid BinStr and key->length == DES_KEY_SIZE
// effects: sets the state of this program to be ready for generating
void initializeKeyGenerator(BinStr key) {
	assert(key != NULL && key->length == DES_KEY_SIZE);
	key_block_C = cPermutation(key);
	key_block_D = dPermutation(key);
	ready_round_key = 1;
}

// generateRoundKey() returns the latest round key to be used. User must free
//   the returned round key
// requires: initializeKeyGenerator() was called, and generateRoundKey()
//           was not called more than the number of DES rounds
// effects: increments the state of the key generator, allocates memory
//          to a new round key
BinStr generateRoundKey() {
	assert(ready_round_key > 0 && ready_round_key <= DES_ROUNDS);
	key_block_C = replace(key_block_C, rotateL(key_block_C,
					   KEY_SHIFTS[ready_round_key - 1]));
	key_block_D = replace(key_block_D, rotateL(key_block_D,
					   KEY_SHIFTS[ready_round_key - 1]));
	ready_round_key++;
	BinStr round_key = append(key_block_C, key_block_D);
	round_key = replace(round_key, rPermutation(round_key));
	return round_key;
}

// destroyKeyGenerator() destroys the "generator" for the various
//   round keys used by DES
// requires: initializeKeyGenerator() was called at some point
// effects: restarts the state of this generator to null
void destroyKeyGenerator() {
	destroy_BinStr(key_block_C);
	destroy_BinStr(key_block_D);
	ready_round_key = 0;
}

// verifyKey(key) returns false if the key given does not pass DES verification
//   else it returns true
// requires: key is a valid BinStr and key->length == DES_KEY_SIZE
bool verifyKey(BinStr key) {
	assert(key != NULL && key->length == DES_KEY_SIZE);
	for(int i = 0; i < DES_KEY_SIZE; i += BITS_PER_BYTE) {
		BinStr section = snip(key, i, i + BITS_PER_BYTE - 1);
		if(parity(key) == 0) {
			return 0;
		}
	}
	return 1;
}

// iPermutation(block) returns a new BinStr in the form of the initial
//   permutation for the DES cipher
// effects: allocates memory to a new BinStr
// requires: block is a valid BinStr and block->length == DES_BLOCK_SIZE
BinStr iPermutation(BinStr block) {
	assert(block != NULL && block->length == DES_BLOCK_SIZE);
	return permutate(block, I_PERMUTATION, I_PERMUTATION_SIZE, 1);	
}

// fPermutation(block) returns a new BinStr in the form of the final
//   permutation for the DES cipher
// effects: allocates memory to a new BinStr
// requires: block is a valid BinStr and block->length == DES_BLOCK_SIZE
BinStr fPermutation(BinStr block) {
	assert(block != NULL && block->length == DES_BLOCK_SIZE);
	return reversePermutate(block, I_PERMUTATION, I_PERMUTATION_SIZE, 1);
}

// ePermutation(R) returns a new BinStr that expands the given right block
//   using the DES E-bit selection.
// effects: allocates memory to a new BinStr
// requires: R is a valid BinStr and R->length == DES_BLOCK_SIZE / 2
BinStr ePermutation(BinStr R) {
	assert(R != NULL && R->length == DES_BLOCK_SIZE / 2);
	return permutate(R, E_PERMUTATION, E_PERMUTATION_SIZE, 1);
}

// pPermutation(R) returns a new BinStr that permutates the given right
//   block after being processed through the S boxes.
// effects: allocates memory to a new BinStr
// requires: R is a valid BinStr and R->length == DES_BLOCK_SIZE / 2
BinStr pPermutation(BinStr R) {
	assert(R != NULL && R->length == DES_BLOCK_SIZE / 2);
	return permutate(R, P_PERMUTATION, P_PERMUTATION_SIZE, 1);
}

// sBox(block) returns a new BinStr which is the output of putting block through
//   the DES S boxes.
// effects: allocates memory to a new BinStr
// requires: block is a valid BinStr and block->length == S_BOX_INPUT_SIZE
BinStr sBox(BinStr block) {
	assert(block != NULL && block->length == S_BOX_INPUT_SIZE);
	BinStr new = empty_BinStr(0);	
	for(int box = 0; box < 8; box++) {

		// Find the relevant row
		int start = box * 6;
		BinStr rowStr = snip(block, start, start);
		BinStr rowEnd = snip(block, start + 5, start + 5);
		rowStr = replace(rowStr, append(rowStr, rowEnd));
		int row = toDecimal(rowStr);
        destroy_BinStr(rowStr);
        destroy_BinStr(rowEnd);
	
		// Find the relevent column
		BinStr colStr = snip(block, start + 1, start + 4);
		int col = toDecimal(colStr);
        destroy_BinStr(colStr);

		// Find the relevant element and convert it
        int element = S_BOX[box][(S_BOX_COLS * row) + col];
		BinStr bin_element = int_to_BinStr(element);
		bin_element = replace(bin_element, cut(bin_element, 4)); 
		new = replace(new, append(new, bin_element));
        destroy_BinStr(bin_element);
	}
	return new;
}

// blockCipher(block, key) returns a new BinStr that is the evaluation of
//   the DES block cipher applied to R using the given key
// effects: allocates memory to a new BinStr
// requires: block is a valid BinStr and block->length == DES_BLOCK_SIZE / 2
//           and key is a valid BinStr and key->length = DES_ROUND_KEY_SIZE
BinStr blockCipher(BinStr block, BinStr key) {
	assert(block != NULL && block->length == DES_BLOCK_SIZE / 2
	       && key != NULL && key->length == DES_ROUND_KEY_SIZE);	
	BinStr new = ePermutation(block);
	new = replace(new, XOR(new, key));
	new = replace(new, sBox(new));
	new = replace(new, pPermutation(new));
	return new;
}

// encryptBlock(block, key) returns a new BinStr that is the evaluation of the
//   DES fesitel network on block using the given key.
// effects: allocates memory to a new BinStr
// requires: block is a valid BinStr and block->length == DES_BLOCK_SIZE
//           and key is a valid BinStr and key->length == DES_KEY_SIZE
BinStr encryptBlock(BinStr block, BinStr key) {
    assert(block != NULL && block->length == DES_BLOCK_SIZE &&
           key != NULL && key->length == DES_KEY_SIZE);
    
    // Prepare the key generator and start the initial L and R blocks
    initializeKeyGenerator(key);
    BinStr new = iPermutation(block);
    BinStr L = snip(new, 0, (DES_BLOCK_SIZE / 2) - 1);
    BinStr R = snip(new, DES_BLOCK_SIZE / 2, DES_BLOCK_SIZE - 1);    

    // Go through the feistel network
    for(int i = 0; i < DES_ROUNDS; i++) {
        BinStr round_key = generateRoundKey();
        BinStr new_R = blockCipher(R, round_key);
        new_R = replace(new_R, XOR(new_R, L));
        destroy_BinStr(L);
        L = R;
        R = new_R;
        destroy_BinStr(round_key);
    }

    // Destroy the key generator and return the result
    destroyKeyGenerator();
    new = replace(new, append(R, L));
    new = replace(new, fPermutation(new));
    destroy_BinStr(L);
    destroy_BinStr(R);
    return new;
}

// See DES.h for details
BinStr DESencrypt(BinStr msg, BinStr key) {
	assert(msg != NULL && key != NULL && key->length == DES_KEY_SIZE);
	// TODO: Encrypt the message using DES
	return msg;
}

// See DES.h for details
BinStr DESdecrypt(BinStr cip, BinStr key) {
	assert(cip != NULL && key != NULL && key->length == DES_KEY_SIZE);
	// TODO: Decrypt the message using DES
	return cip;
}

