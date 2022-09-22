#ifndef DL_COMMON_H
#define DL_COMMON_H
#include <stdio.h>
#include <unicorn/unicorn.h>

#define Elf32(TYPE) Elf32_##TYPE

#define unlikely(x)       __builtin_expect((!!(x)),0)
#define attribute_unused __attribute__((unused))
#define do_rem(result, n, base) ((result) = (n) % (base))

/* common align masks, if not specified by sysdep headers */
#define _dl_pagesize 4096
#ifndef ADDR_ALIGN
#define ADDR_ALIGN (_dl_pagesize - 1)
#endif

#ifndef PAGE_ALIGN
#define PAGE_ALIGN (~ADDR_ALIGN)
#endif

#ifndef OFFS_ALIGN
#define OFFS_ALIGN (PAGE_ALIGN & ~(1ul << (sizeof(_dl_pagesize) * 8 - 1)))
#endif

#ifdef UF_DEBUG
#define uf_debug(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define uf_debug(fmt, ...)
#endif

// https://stackoverflow.com/questions/12946958/what-is-the-interface-for-arm-system-calls-and-where-is-it-defined-in-the-linux
// arch/ABI    instruction           syscall No.  retval  error    Notes
// arm/EABI    swi 0x0               r7           r0      -
// i386        int $0x80             eax          eax     -
// mips        syscall               v0           v0      a3       [1]

// arch/ABI      arg1  arg2  arg3  arg4 
// arm/EABI      r0    r1    r2    r3   
// i386          ebx   ecx   edx   esi  
// mips          a0    a1    a2    a3   

FILE* patch_count;
FILE* unexpected_log;

FILE* traceLogger;
FILE* libLogger;
FILE* pcLogger;
uint32_t* pc_log_buffer;
uint32_t pc_log_index;
#define PCLOGMAX 0x10000

enum uf_arch {UF_ARCH_ARM=0, UF_ARCH_MIPS, UF_ARCH_X86};
enum uf_endian {UF_ENDIAN_LITTLE, UF_ENDIAN_BIG};
enum uf_regs {                       
   UF_REG_PC=0,     UF_REG_SP,      UF_REG_LR,      UF_REG_A0,      UF_REG_A1,      UF_REG_A2,      UF_REG_A3,      UF_REG_RV,      UF_REG_SYSCALL,  UF_REG_BP
}; //RV stands for reture value
static int uf_conv_regs[3][10] = {        \
   {UC_ARM_REG_PC,  UC_ARM_REG_SP,  UC_ARM_REG_LR,  UC_ARM_REG_R0,  UC_ARM_REG_R1,  UC_ARM_REG_R2,  UC_ARM_REG_R3,  UC_ARM_REG_R0,  UC_ARM_REG_R7  , UC_ARM_REG_R11},     \
   {UC_MIPS_REG_PC, UC_MIPS_REG_SP, UC_MIPS_REG_RA, UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_A2, UC_MIPS_REG_A3, UC_MIPS_REG_V0, UC_MIPS_REG_V0 , UC_MIPS_REG_FP},     \
   {UC_X86_REG_EIP, UC_X86_REG_ESP, 0,              UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EAX, UC_X86_REG_EAX , UC_X86_REG_EBP}      \
};

enum uf_arch arch;
enum uf_endian endian;

static uint32_t uf_syscall[3][434]= { {219, 60, 57, 0, 1, 2, 3, -1, 85, 86, 87, 59, 80, 201, 133, 90, 94, -1, -1, 8, 39, 165, -1, 105, 102, -1, 101, -1, -1, 34, -1, -1, -1, 21, -1, -1, 162, 62, 82, 83, 84, 32, 22, 100, -1, 12, 106, 104, -1, 107, 108, 163, 166, -1, 16, 72, -1, 109, -1, -1, 95, 161, 136, 33, 110, 111, 112, -1, -1, -1, 113, 114, -1, -1, 170, 160, -1, 98, 96, 164, 115, 116, -1, 88, -1, 89, 134, 167, 169, -1, -1, 11, 76, 77, 91, 93, 140, 141, -1, 137, 138, -1, -1, 103, 38, 36, 4, 6, 5, -1, -1, 153, -1, -1, 61, 168, 99, -1, 74, -1, 56, 171, 63, -1, 159, 10, -1, -1, 175, 176, -1, 179, 121, 81, -1, 139, 135, -1, 122, 123, -1, 78, -1, 73, 26, 19, 20, 124, 75, 156, 149, 150, 151, 152, 142, 143, 144, 145, 24, 146, 147, 148, 35, 25, 117, 118, -1, -1, 7, 180, 119, 120, 157, 15, 13, 14, 127, 128, 129, 130, 17, 18, 92, 79, 125, 126, 131, 40, -1, -1, 58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 217, 155, 27, 28, -1, -1, -1, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 202, 203, 204, 206, 207, 208, 209, 210, 231, 212, 213, 233, 232, 216, -1, -1, 218, 222, 223, 224, 225, 226, 227, 228, 229, 230, -1, -1, 234, 235, -1, -1, -1, -1, 240, 241, 242, 243, 244, 245, 247, 41, 49, 42, 50, 43, 51, 52, 53, -1, 44, -1, 45, 48, 54, 55, 46, 47, 65, 64, 66, 69, 70, 68, 71, 30, 67, 29, 31, 248, 249, 250, 220, 236, 251, 252, 253, 254, 255, 237, 239, 238, 257, 258, 259, 260, 261, -1, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, -1, 276, 278, 279, 309, 281, 246, 280, 282, 283, 284, 285, 286, 287, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 288, 300, 301, 302, 303, 304, 305, 306, 307, 308, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 334, 333, 256, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433}, \
{-1, 60, 57, 0, 1, 2, 3, -1, 85, 86, 87, 59, 80, 201, 133, 90, 94, -1, -1, 8, 39, 165, -1, 105, 102, -1, 101, 37, -1, 34, 132, -1, -1, 21, -1, -1, 162, 62, 82, 83, 84, 32, 22, 100, -1, 12, 106, 104, -1, 107, 108, 163, 166, -1, 16, 72, -1, 109, -1, -1, 95, 161, 136, 33, 110, 111, 112, -1, -1, -1, 113, 114, -1, -1, 170, 160, 97, 98, 96, 164, 115, 116, -1, 88, -1, 89, 134, 167, 169, -1, 9, 11, 76, 77, 91, 93, 140, 141, -1, 137, 138, 173, -1, 103, 38, 36, 4, 6, 5, -1, 172, 153, -1, -1, 61, 168, 99, -1, 74, -1, 56, 171, 63, 154, 159, 10, -1, 174, 175, 176, 177, 179, 121, 81, -1, 139, 135, 183, 122, 123, -1, 78, -1, 73, 26, 19, 20, -1, -1, -1, -1, 124, 75, 156, 149, 150, 151, 152, 142, 143, 144, 145, 24, 146, 147, 148, 35, 25, 43, 49, 42, 52, 51, 55, 50, -1, 45, 47, -1, 46, 44, 54, 48, 41, 53, 117, 118, 178, 7, 180, 119, 120, 157, 15, 13, 14, 127, 128, 129, 130, 17, 18, 92, 79, 125, 126, 131, 40, 181, 182, -1, -1, -1, -1, -1, -1, 155, 27, 28, 217, -1, -1, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 202, 203, 204, 206, 207, 208, 209, 210, 231, 212, 213, 233, 232, 216, 218, 219, 221, -1, -1, 222, 223, 224, 225, 226, 227, 228, 229, 230, 234, 235, 237, 239, 238, 240, 241, 242, 243, 244, 245, 236, 247, -1, 248, 249, 250, 205, 253, 254, 255, 256, 257, 258, 259, 260, 261, -1, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 275, 277, 276, 278, 279, 273, 274, 246, 309, 281, 251, 252, 280, 282, -1, 284, 285, 283, 287, 286, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 288, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 334, 333, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 64, 66, 29, 31, 30, 67, 68, 69, 70, 71, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433},\
{219, 60, 57, 0, 1, 2, 3, -1, 85, 86, 87, 59, 80, 201, 133, 90, 94, -1, -1, 8, 39, 165, -1, 105, 102, -1, 101, 37, -1, 34, 132, -1, -1, 21, -1, -1, 162, 62, 82, 83, 84, 32, 22, 100, -1, 12, 106, 104, -1, 107, 108, 163, 166, -1, 16, 72, -1, 109, -1, -1, 95, 161, 136, 33, 110, 111, 112, -1, -1, -1, 113, 114, -1, -1, 170, 160, 97, 98, 96, 164, 115, 116, 23, 88, -1, 89, 134, 167, 169, -1, 9, 11, 76, 77, 91, 93, 140, 141, -1, 137, 138, 173, -1, 103, 38, 36, 4, 6, 5, -1, 172, 153, -1, -1, 61, 168, 99, -1, 74, -1, 56, 171, 63, 154, 159, 10, -1, 174, 175, 176, 177, 179, 121, 81, -1, 139, 135, 183, 122, 123, -1, 78, -1, 73, 26, 19, 20, 124, 75, 156, 149, 150, 151, 152, 142, 143, 144, 145, 24, 146, 147, 148, 35, 25, 117, 118, -1, 178, 7, 180, 119, 120, 157, 15, 13, 14, 127, 128, 129, 130, 17, 18, 92, 79, 125, 126, 131, 40, 181, 182, 58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 155, 27, 28, 217, -1, -1, -1, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, -1, 202, 203, 204, 205, 211, 206, 207, 208, 209, 210, 221, -1, 231, 212, 213, 233, 232, 216, 218, 222, 223, 224, 225, 226, 227, 228, 229, 230, -1, -1, 234, 235, -1, 236,237, 239, 238, 240, 241, 242, 243, 244, 245, 246, 247, -1, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, -1, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 277, 276, 278, 279, 309, 281, 280, 282, 283, 284, 285, 286, 287, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 321, 322, 41, 53, 49, 42, 50, 288, 55, 54, 51, 52, 44, 46, 45, 47, 48, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 158, 333, 334, -1, -1, -1, -1, -1, -1, 64, 66, 29, 31, 30, 67, 68, 69, 70, 71, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433}};


// NVRAM related to libnvram.so
extern char* nvram_get(const char * key);
extern int nvram_set(const char * key,const char * value);
extern int nvram_init();


#endif

