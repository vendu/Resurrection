root=`pwd`
libs="lib/Resurrection lib/Rwm"
for lib in $libs
do
  cd $lib
  if test -f Makefile.cint ; then
      make -f Makefile.cint clean
  fi
  ./build_cint.sh
  cd $root
done

