#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^D:/Tools/cygwin/lib!/usr/lib!ig;s! D:/Tools/cygwin/lib! /usr/lib!ig;s!^D:/Tools/cygwin/bin!/usr/bin!ig;s! D:/Tools/cygwin/bin! /usr/bin!ig;s!^D:/Tools/cygwin/!/!ig;s! D:/Tools/cygwin/! /!ig;s!^G:!/cygdrive/g!ig;s! G:! /cygdrive/g!ig;s!^F:!/cygdrive/f!ig;s! F:! /cygdrive/f!ig;s!^D:!/cygdrive/d!ig;s! D:! /cygdrive/d!ig;s!^C:!/cygdrive/c!ig;s! C:! /cygdrive/c!ig;' $1.org > $1 && rm -f $1.org
fi
