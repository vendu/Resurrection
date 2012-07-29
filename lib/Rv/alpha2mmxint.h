#include <mmintrin.h>	/* MMX compiler intrinsics */

#include "pix.h"
#include "blend.h"

/* NOTE: leaves destination ALPHA undefined */
#define alphablendloq_mmx(src, dest, aval)                              \
    do {                                                                \
        __m64 _mzero;                                                   \
        __m64 _msrc;                                                    \
        __m64 _mdest;                                                   \
        __m64 _malpha;                                                  \
        __m64 _mtmp;                                                    \
                                                                        \
        _mzero = _mm_cvtsi32_si64(0);              /* 0000000000000000 */ \
        _malpha = _mm_cvtsi32_si64(aval);          /* 00000000000000AA */ \
        _mtmp = _mm_slli_si64(_malpha, 16);        /* 0000000000AA0000 */ \
        _malpha = _mm_or_si64(_mtmp, _malpha);     /* 0000000000AA00AA */ \
        _mtmp = _mm_slli_si64(_malpha, 32);        /* 00AA00AA00000000 */ \
        _malpha = _mm_or_si64(_malpha, _mtmp);     /* 00AA00AA00AA00AA */ \
        _msrc = _mm_cvtsi32_si64(src);             /* S:00000000AARRGGBB */ \
        _mdest = _mm_cvtsi32_si64(dest);           /* D:00000000AARRGGBB */ \
        _msrc = _mm_unpacklo_pi8(_msrc, _mzero);   /* S:00AA00RR00GG00BB */ \
        _mdest = _mm_unpacklo_pi8(_mdest, _mzero); /* D:00AA00RR00GG00BB */ \
        _msrc = _mm_sub_pi16(_msrc, _mdest);       /* S - D */          \
        _msrc = _mm_mullo_pi16(_msrc, _malpha);    /* T = (S - D) * A */ \
        _msrc = _mm_srli_pi16(_msrc, 8);           /* T >> 8 */         \
        _mdest = _mm_add_pi8(_msrc, _mdest);       /* D = D + T */       \
        _mdest = _mm_packs_pu16(_mdest, _mzero);   /* D:00000000??RRGGBB */ \
        (dest) = _mm_cvtsi64_si32(_mdest);         /* DEST = D */       \
    } while (FALSE)

