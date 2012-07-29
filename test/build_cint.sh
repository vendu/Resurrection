XLIBPATH="/usr/X11R6/lib"
makecint -mk Makefile.cint -I../include/ -m -o wmtest -C wmtest.c -l /usr/X11R6/lib/libX11.so -l /usr/local/lib/libImlib2.so -l ../lib/Resurrection/libR_cint.dl -l ../lib/Rwm/libRwm_cint.dl
make -f Makefile.cint
