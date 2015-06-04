OpenRadio block library provides blocks that can be used to tie together WiFi 802.11g
PHY layer and some variants like UEP, Softrate etc.

# Build instructions #

do some one-time bootstrapping:

$ cd <path_to>/orextlibs; make install


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


# environment variables #

Put this in your .bashrc (with appropriate modifications to path values):

export ORILIB_INC_DIR=/home/manub/workspace/orilib
export ORILIB_INSTALL_DIR=/home/manub/workspace/orilib/build
