dnl -*- Autoconf -*-

AC_DEFUN([SST_palacios_CONFIG], [

  AC_ARG_WITH([palacios],
    [AS_HELP_STRING([--with-palacios@<:@=DIR@:>@],
      [Use Palacios package installed in optionally specified DIR])])

  happy = "yes"

  AS_IF([test "$with_palacios" = "no"], [happy="no"])

  CPPFLAGS_saved="$CPPFLAGS"
  LDFLAGS_saved="$LDFLAGS"
  LIBS_saved="$LIBS"

  AS_IF([test ! -z "$with_palacios" -a "$with_palacios" != "yes"],
    [PALACIOS_CPPFLAGS="-I$with_palacios"
     CPPFLAGS="$PALACIOS_CPPFLAGS $CPPFLAGS"
     PALACIOS_LDFLAGS="-L$with_palacios"
     LDFLAGS="$PALACIOS_LDFLAGS $LDFLAGS"],
    [PALACIOS_CPPFLAGS=
     PALACIOS_LDFLAGS=])

  AC_LANG_PUSH(C++)
  AC_CHECK_HEADERS([MemorySystem.h], [], [happy="no"])
  AC_CHECK_LIB([palacios], [libpalacios_is_present], 
    [PALACIOS_LIB="-lpalacios"], [happy="no"])
  AC_LANG_POP(C++)

  CPPFLAGS="$CPPFLAGS_saved"
  LDFLAGS="$LDFLAGS_saved"
  LIBS="$LIBS_saved"

  AC_SUBST([PALACIOS_CPPFLAGS])
  AC_SUBST([PALACIOS_LDFLAGS])
  AC_SUBST([PALACIOS_LIB])

  AS_IF([test "$happy" = "yes"], [$1], [$2])
])
