#include "mcc/uint256.h"

#include <bitset>
#include <sstream>

namespace mcc {
  std::string uint256::ToHexString() const{
   static const char *hexDigits = "0123456789ABCDEF";
   std::string hexString;
   hexString.reserve(size() * 2);
   std::for_each(
       const_begin(),
       const_end(),
       [&hexString](uint8_t inputByte) {
         hexString.push_back(hexDigits[inputByte >> 4]);
         hexString.push_back(hexDigits[inputByte & 0x0F]);
       });
   return hexString;
 }

 std::string uint256::ToBinaryString() const{
   std::stringstream ss;
   std::bitset<256> bits;
   for(int idx = 0; idx < size(); idx++){
     auto curr = data()[idx];
     int offset = idx * CHAR_BIT;
     for(int bit = 0; bit < CHAR_BIT; bit++){
       bits[offset] = curr & 1;
       offset++;
       curr >>= 1;
     }
   }
   ss << bits;
   return ss.str();
 }
}