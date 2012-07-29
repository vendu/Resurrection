#include <stdint.h>
#include <stdio.h>

uint8_t tab[65536][8];

#define utf8enc(u, p) \
    do {                                                                \
        if ((u) < 0x80) {                                               \
            *(p)++ = (uint8_t)(u);                                      \
        } else if ((u) < 0x0800) {                                      \
            *(p)++ = (uint8_t)(0xc0 | (((u) >> 6) & 0x1f));             \
            *(p)++ = (uint8_t)(0x80 | ((u) & 0x3f));                    \
        } else if ((u) < 0x10000) {                                     \
            *(p)++ = (uint8_t)(0xe0 | (((u) >> 12) & 0x0f));            \
            *(p)++ = (uint8_t)(0x80 | (((u) >> 6) & 0x3f));             \
            *(p)++ = (uint8_t)(0x80 | ((u) & 0x3f));                    \
        } else {                                                        \
            ; /* perhaps in the future. */                              \
        }                                                               \
    } while (0)

uint16_t
utf8dec(unsigned char *str, int *len)
{
    unsigned char *cp = str;
    int nb = 1;
    uint16_t wch = *cp;
    uint16_t tmp;
    
    if (wch > 0x7f) {
        nb = 1;
        while ((*++cp & 0xc0) == 0xc0) {
            nb++;
        }
        cp = str;
        wch = 0;
        if (nb == 2) {
            tmp = *cp++;
            wch = (tmp & 0x1f) << 6;
            tmp = *cp++;
            wch |= tmp & 0x3f;
        } else if (nb == 3) {
            /* nb == 3 */
            tmp = *cp++;
            wch = (tmp & 0x0f) << 12;
            tmp = *cp++;
            wch |= (tmp & 0x3f) << 6;
            tmp = *cp++;
            wch |= (tmp & 0x3f);
        }
    } else if (wch) {
        nb = 1;
    }
    *len = nb;

    return wch;
}

int
main(int argc, char *argv[])
{
    int i;
    uint16_t wc;
    int len;
    uint8_t *cp;

#if 0
    i = 0xa9;
    cp = &tab[i][0];
    utf8enc(i, cp);
    printf("\n");
    i = 0x2260;
    cp = &tab[i][0];
    utf8enc(i, cp);
    printf("\n");
#endif

    exit(0);

    for (i = 0 ; i < 65536 ; i++) {
        cp = &tab[i][0];
        utf8enc(i, cp);
        wc = utf8dec(&tab[i][0], &len);
        if (wc != i) {
//            printf("0x%x (0x%x): %x\n", i, wc, len);
            printf("ERROR: %x != %x (%d)\n", wc, i, len);
            exit(1);
        }
    }

    exit(0);
}
