#!/bin/sh
TMPBASEDIR=$PWD;
sudo apt-get -y install qt5-default graphviz-dev doxygen libeigen3-dev;
cd $TMPBASEDIR/Src/Doc;
doxygen RobotSDK_Linux.doc;
ln -s $HOME/SDK/RobotSDK_4.0/Doc/html/index.html $HOME/SDK/RobotSDK_4.0/Doc/RobotSDK.html
cd $TMPBASEDIR;
qmake -makefile $TMPBASEDIR/Src/Kernel/Kernel.pro -r -o "Makefile.Release" "CONFIG+=release";
make -f Makefile.Release;
make -f Makefile.Release install;
rm Makefile.Release;
qmake -makefile $TMPBASEDIR/Src/Kernel/Kernel.pro -r -o "Makefile.Debug" "CONFIG+=debug";
make -f Makefile.Debug;
make -f Makefile.Debug install;
rm Makefile.Debug;
qmake -makefile $TMPBASEDIR/Src/Robot-X/Robot-X.pro -r -o "Makefile.Release" "CONFIG+=release";
make -f Makefile.Release -B;
make -f Makefile.Release install;
rm Makefile.Release;
qmake -makefile $TMPBASEDIR/Src/Robot-X/Robot-X.pro -r -o "Makefile.Debug" "CONFIG+=debug";
make -f Makefile.Debug -B;
make -f Makefile.Debug install;
rm Makefile.Debug;
rm install;

