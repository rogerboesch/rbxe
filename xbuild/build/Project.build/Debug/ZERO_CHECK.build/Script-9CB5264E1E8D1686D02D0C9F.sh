#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/roger/Data/Projects-Retro/PixelEngine/xbuild
  make -f /Users/roger/Data/Projects-Retro/PixelEngine/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/roger/Data/Projects-Retro/PixelEngine/xbuild
  make -f /Users/roger/Data/Projects-Retro/PixelEngine/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/roger/Data/Projects-Retro/PixelEngine/xbuild
  make -f /Users/roger/Data/Projects-Retro/PixelEngine/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/roger/Data/Projects-Retro/PixelEngine/xbuild
  make -f /Users/roger/Data/Projects-Retro/PixelEngine/xbuild/CMakeScripts/ReRunCMake.make
fi

