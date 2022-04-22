#include <sstream>
#include <string>

#include "packed_codebooks.h"
#include "wwriff.h"

extern "C" bool
ww2ogg(const std::string &indata, std::ostream &outdata,
       unsigned char codebooks_data[] = packed_codebooks_bin,
       bool inline_codebooks = false, bool full_setup = false,
       ForcePacketFormat force_packet_format = kNoForcePacketFormat);

extern "C" std::string
wem_info(const std::string &indata,
         unsigned char codebooks_data[] = packed_codebooks_bin,
         bool inline_codebooks = false, bool full_setup = false,
         ForcePacketFormat force_packet_format = kNoForcePacketFormat);
