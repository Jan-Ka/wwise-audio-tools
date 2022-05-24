#ifndef WW2OGG_CODEBOOK_H
#define WW2OGG_CODEBOOK_H

#include "bitstream.h"
#include "errors.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>

using namespace std;

/* stuff from Tremor (lowmem) */
namespace {
int ilog(unsigned int v) {
  int ret = 0;
  while (v) {
    ret++;
    v >>= 1;
  }
  return (ret);
}

unsigned int _book_maptype1_quantvals(unsigned int entries,
                                      unsigned int dimensions) {
  /* get us a starting hint, we'll polish it below */
  int bits = ilog(entries);
  int vals = entries >> ((bits - 1) * (dimensions - 1) / dimensions);

  while (1) {
    unsigned long acc = 1;
    unsigned long acc1 = 1;
    unsigned int i;
    for (i = 0; i < dimensions; i++) {
      acc *= vals;
      acc1 *= vals + 1;
    }
    if (acc <= entries && acc1 > entries) {
      return (vals);
    } else {
      if (acc > entries) {
        vals--;
      } else {
        vals++;
      }
    }
  }
}

} // namespace

namespace ww2ogg {
class codebook_library {
  char *codebook_data;
  long *codebook_offsets;
  long codebook_count;

  // Intentionally undefined
  codebook_library &operator=(const codebook_library &rhs);
  codebook_library(const codebook_library &rhs);

public:
  codebook_library(std::string indata);
  codebook_library(void);

  ~codebook_library() {
    delete[] codebook_data;
    delete[] codebook_offsets;
  }

  const char *get_codebook(int i) const {
    if (!codebook_data || !codebook_offsets) {
      throw parse_error_str("codebook library not loaded");
    }
    if (i >= codebook_count - 1 || i < 0)
      return NULL;
    return &codebook_data[codebook_offsets[i]];
  }

  long get_codebook_size(int i) const {
    if (!codebook_data || !codebook_offsets) {
      throw parse_error_str("codebook library not loaded");
    }
    if (i >= codebook_count - 1 || i < 0)
      return -1;
    return codebook_offsets[i + 1] - codebook_offsets[i];
  }

  void rebuild(int i, bitoggstream &bos);

  void rebuild(bitstream &bis, unsigned long cb_size, bitoggstream &bos);

  void copy(bitstream &bis, bitoggstream &bos);
};
} // namespace ww2ogg

#endif // WW2OGG_CODEBOOH_H
