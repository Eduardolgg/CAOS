# CAOS
CAOS init suite

Actually CAOS is only testet on devuan and only have a replacement for rc (/etc/init.d/rc) init script

Build requirements
-------------
To build CAOS you need installed:
  * meson version 0.39.1 or above (To install meson see: http://mesonbuild.com/Getting-meson.html)
  * ninja version 1.7.2 or above
  * gcc version 4.9.2 or above
  * gcovr (OPTIONAL for tests)
  * valgind (OPTIONAL for tests)

Screenshots
-------------
You can see some Screenshots at:
 * https://drive.google.com/drive/folders/1NuhqPsTQoIqtLTdFoD41WpOgNo2UrScT

How to build and install
--------------------------

Yo can run the script buildAndInstall.sh or follow this steps:

    meson build
    ./setReleaseMode.sh
    cd ./build
    ninja clean
    ninja
    sudo ninja install

Configuring your inittab
--------------------------
replace your rc init lines by zeus, for example:

    si::sysinit:/sbin/zeus serial
    l0:0:wait:/sbin/zeus serial
    ...

The tag serial is the start algorithm you can use serial or low_parallel, but low_parallel is still under development

How to run tests
------------------
Yo can run the script buildAndTestAll.sh or follow this steps:

    ./setDebugMode.sh
    rm -rf ./tests/fake_init.d/log/*

    cd ./build
    ninja clean
    ninja
    ninja test

How to build and run examples
-------------------------------
To build the examples go to examples directory and run:

    meson build
    cd ./build
    ninja clean
    ninja
    Now go to src subdir in build directory -> cd src
    Run the examples do you want.
