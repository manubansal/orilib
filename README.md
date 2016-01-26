OpenRadio block library provides blocks that can be used to tie together WiFi 802.11g
PHY layer and some variants like UEP, Softrate etc.

# Build instructions #

Before you can build orilib, you need to clone tiextlibs and do a make install in tiextlibs so it copies over some source files into this folder that are needed to build orilib. (Those files are TI copyrighted and released under TI source code license, which permits their use, modification, and re-distribution. All code in the orilib repository is Stanford copyrighted and released under Apache 2.0.)

setup environment variables:

Put this in your .bashrc (with appropriate modifications to path values):

```
export ORILIB_INC_DIR=/home/manub/workspace/orilib
export ORILIB_INSTALL_DIR=/home/manub/workspace/orilib/build
export DSPLIB_ROOT="$TIEXTLIBS_ROOT/dsplib_c66x_3_2_0_1"
export IQMATH_ROOT="$TIEXTLIBS_ROOT/iqmath"                                                                                                                                               
#export IQMATH_ROOT="$TIEXTLIBS_ROOT/c64xplus-iqmath_2_01_04_00"
export UNITY_ROOT="$TIEXTLIBS_ROOT/unity"
```

do some one-time bootstrapping:

$ cd <path_to>/tiextlibs; make preporilib


In the orilib folder, do the following steps:

Initialize configuration scripts only once:

$ cd orilib; autoreconf --install


Create the directory in which the project will be built:

$ cd orilib; mkdir build


Set up the build configuration:

$ cd build; ../_configure tsconf=none debug=2 memconf=memconf1 inptracemode=lc snrber=yes  #representative settings -- pick your own values for flags


Build the library:

$ make

You should have lib.orilib.a in your build folder.


