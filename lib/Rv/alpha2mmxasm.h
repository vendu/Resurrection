#include "pix.h"
#include "blend.h"

#define alphablendloq_mmx_asm(src, dest, aval)                          \
    do {                                                                \
        __asm__ ("emms\n");                                             \
        __asm__ ("pxor %mm0, %mm0\n");                                  \
        __asm__ ("movd %0, %%mm1\n" : : "rm" (src));                    \
        __asm__ ("movd %0, %%mm2\n" : : "rm" (dest));                   \
        __asm__ ("movd %0, %%mm3\n" : : "rm" (aval));                   \
        __asm__ ("punpcklbw %mm0, %mm1\n");                             \
        __asm__ ("movq %mm3, %mm5\n");                                  \
        __asm__ ("punpcklbw %mm0, %mm2\n");                             \
        __asm__ ("psllq $16, %mm5\n");                                  \
        __asm__ ("pxor %mm5, %mm3\n");                                  \
        __asm__ ("movq %mm3, %mm5\n");                                  \
        __asm__ ("psllq $32, %mm5\n");                                  \
        __asm__ ("pxor %mm5, %mm3\n");                                  \
        __asm__ ("psubw %mm2, %mm1\n");                                 \
        __asm__ ("movq %mm1, %mm4\n");                                  \
        __asm__ ("pmullw %mm3, %mm4\n");                                \
        __asm__ ("psrlw $8, %mm4\n");                                   \
        __asm__ ("paddb %mm4, %mm2\n");                                 \
        __asm__ ("packuswb %mm0, %mm2\n");                              \
        __asm__ __volatile__ ("movd %%mm2, %0\n" : "=rm" (dest));       \
    } while (FALSE)

