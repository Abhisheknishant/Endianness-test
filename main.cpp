#include <inttypes.h>
#include <iostream>

#if defined(__GLIBC__) || (defined(__GNUC__) && !defined(__llvm__) && !defined(__MINGW32__) && !defined(__FreeBSD__) && defined(__BYTE_ORDER__))
#include <endian.h>
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define VISP_LITTLE_ENDIAN
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define VISP_BIG_ENDIAN
#elif (__BYTE_ORDER == __PDP_ENDIAN)
// Currently not supported when reading / writing binary file
#define VISP_PDP_ENDIAN
//#error PDP endian is not supported. //Uncomment if needed/happens
#else
#error Unknown machine endianness detected.
#endif
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN) || defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)
#define VISP_BIG_ENDIAN
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN) || defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#define VISP_LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) ||         \
    defined(__hpux) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)

#define VISP_BIG_ENDIAN
#elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) || defined(_M_IX86) ||           \
    defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) ||            \
    defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__ANDROID__)
    // It appears that all Android systems are little endian.
    // Refer https://stackoverflow.com/questions/6212951/endianness-of-android-ndk
#define VISP_LITTLE_ENDIAN
#elif defined(WINRT) // For UWP
// Refer https://social.msdn.microsoft.com/Forums/en-US/04c92ef9-e38e-415f-8958-ec9f7c196fd3/arm-endianess-under-windows-mobile?forum=windowsmobiledev
#define VISP_LITTLE_ENDIAN
#else
#error Cannot detect host machine endianness.
#endif

namespace
{
class vpRGBa
{
public:
    enum AlphaDefault
    {
        alpha_default = 255
    };

    vpRGBa() : R(0), G(0), B(0), A(vpRGBa::alpha_default) {}

    vpRGBa(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0)
        : R(r), G(g), B(b), A(a) {}

public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

#ifdef VISP_BIG_ENDIAN
// Swap 16 bits by shifting to the right the first byte and by shifting to the
// left the second byte
uint16_t swap16bits(const uint16_t val)
{
  return (((val >> 8) & 0x00FF) | ((val << 8) & 0xFF00));
}
#endif

uint16_t safe_reinterpret_cast_uchar_to_uint16(unsigned char * const ptr)
{
#ifdef VISP_LITTLE_ENDIAN
    return *reinterpret_cast<uint16_t *>(ptr);
#elif defined(VISP_BIG_ENDIAN)
    return swap16bits(*reinterpret_cast<uint16_t *>(ptr));
#else
    throw std::runtime_error("Not supported endianness for correct reinterpret_cast().");
#endif
}
}

int main()
{
    unsigned char bitmap[] = {100, 110, 120, 130};
    uint16_t val1 = *reinterpret_cast<uint16_t *>(bitmap + 1);
    uint16_t val2 = safe_reinterpret_cast_uchar_to_uint16(bitmap + 1);
    std::cout << "val10: " << (val1 & 0x00FF) << std::endl;
    std::cout << "val11: " << ((val1 & 0xFF00) >> 8) << std::endl;
    std::cout << "val10 (safe): " << (val2 & 0x00FF) << std::endl;
    std::cout << "val11 (safe): " << ((val2 & 0xFF00) >> 8) << std::endl;

    vpRGBa rgba(100, 110, 120);
    float r = static_cast<float>(reinterpret_cast<unsigned char *>(&rgba)[0]);
    float g = static_cast<float>(reinterpret_cast<unsigned char *>(&rgba)[1]);
    float b = static_cast<float>(reinterpret_cast<unsigned char *>(&rgba)[2]);
    std::cout << "r: " << r << std::endl;
    std::cout << "g: " << g << std::endl;
    std::cout << "b: " << b << std::endl;

    return 0;
}
