#include <inttypes.h>
#include <iostream>

int main()
{
    unsigned char bitmap[] = {100, 110, 120, 130};
    uint16_t val1 = *reinterpret_cast<uint16_t *>(bitmap + 1);
    std::cout << "val10: " << (val1 & 0x00FF) << std::endl;
    std::cout << "val11: " << ((val1 & 0xFF00) >> 8) << std::endl;

    return 0;
}
