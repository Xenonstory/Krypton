/* DES.h - Written by Xenonstory on June 24, 2015    *
 * An implementation of the Data Encryption Standard */      

#ifndef DES_H
#define DES_H

#define DES_ROUNDS 16

#include "../Structures/BinStr.h"
#include "../Wrappers/BlockCipher.h"

// DESinitialize(key, mode) returns an instance of a block cipher ready for
//   decryption and encryption using the given key and mode
// requires: key != NULL, mode is a valid mode (ECB, etc.)
// effects: allocates memory to a new DES cipher
BlockCipher DES_initialize(BinStr key, char* mode);

// DESdestroy(cipher) destroys the given instance of the DES block cipher
// requires: cipher != NULL and cipher is a DES block cipher
// effects: frees memory used by the cipher
void DES_destroy(BlockCipher DES);

#endif
