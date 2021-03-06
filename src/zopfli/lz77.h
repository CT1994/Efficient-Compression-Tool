/*
Copyright 2011 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: lode.vandevenne@gmail.com (Lode Vandevenne)
Author: jyrki.alakuijala@gmail.com (Jyrki Alakuijala)
*/

/*
Functions for basic LZ77 compression and utilities for the "squeeze" LZ77
compression.
*/

/*Modified by Felix Hanau*/

#ifndef ZOPFLI_LZ77_H_
#define ZOPFLI_LZ77_H_

#include "zopfli.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
Stores lit/length and dist pairs for LZ77.
Parameter litlens: Contains the literal symbols or length values.
Parameter dists: Contains the distances. A value is 0 to indicate that there is
no dist and the corresponding litlens value is a literal instead of a length.
Parameter size: The size of both the litlens and dists arrays.
The memory can best be managed by using ZopfliInitLZ77Store to initialize it
and ZopfliCleanLZ77Store to destroy it.

*/
typedef struct ZopfliLZ77Store {
  unsigned short* litlens;  /* Lit or len. */
  unsigned short* dists;  /* If 0: indicates literal in corresponding litlens,
      if > 0: length in corresponding litlens, this is the distance. */
  size_t size;
  unsigned char symbols;
} ZopfliLZ77Store;

void ZopfliInitLZ77Store(ZopfliLZ77Store* store);
void ZopfliCleanLZ77Store(ZopfliLZ77Store* store);
void ZopfliCopyLZ77Store(const ZopfliLZ77Store* source, ZopfliLZ77Store* dest);

/*
Verifies if length and dist are indeed valid, only used for assertion.
*/
void ZopfliVerifyLenDist(const unsigned char* data, size_t datasize, size_t pos,
                         unsigned short dist, unsigned short length);

/*
Counts the number of literal, length and distance symbols in the given lz77
arrays.
litlens: lz77 lit/lengths
dists: ll77 distances
start: where to begin counting in litlens and dists
end: where to stop counting in litlens and dists (not inclusive)
ll_count: count of each lit/len symbol, must have size 288 (see deflate
    standard)
d_count: count of each dist symbol, must have size 32 (see deflate standard)
*/
void ZopfliLZ77Counts(const unsigned short* litlens,
                      const unsigned short* dists,
                      size_t start, size_t end,
                      size_t* ll_count, size_t* d_count, unsigned char symbols);

/*
Does LZ77 using an algorithm similar to gzip, with lazy matching, rather than
with the slow but better "squeeze" implementation.
The result is placed in the ZopfliLZ77Store.
If instart is larger than 0, it uses values before instart as starting
dictionary.
*/
void ZopfliLZ77Lazy(const ZopfliOptions* options, const unsigned char* in,
                      size_t instart, size_t inend,
                      ZopfliLZ77Store* store);

#ifdef __cplusplus
}
#endif

#endif  /* ZOPFLI_LZ77_H_ */
