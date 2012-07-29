AC_DEFUN([R_SET_TEMPLATES],
[
    AH_TEMPLATE([_FILE_OFFSET_BITS], [64 may make 64-bit off_t work])
    AH_TEMPLATE([R_SBRK_T], [define to sbrk() argument type])
])

