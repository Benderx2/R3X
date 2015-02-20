/** NOTE: This checksum is *not* crypto-sec since it uses CRC32. 
 * It's only designed to protect against accidental tampering with an executable **/
#ifndef R3X_CHECKSUM_GEN_H
#define R3X_CHECKSUM_GEN_H
#include <system.h>

static inline uint32_t GenerateChecksumCRC32(uint8_t* Buffer, unsigned int size);

static inline uint32_t GenerateChecksumCRC32(uint8_t* Buffer, unsigned int size) {
  unsigned int i;
  signed int j;
  unsigned int byte, mask;
  uint32_t crc;
  i = 0;
  crc = 0xFFFFFFFF;
  while (i <= size) {
      byte = Buffer[i];            
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}
#endif