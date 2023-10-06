# squintymongrel

![SquintyMongrel](resources/images/squintymongrel.svg "SquintyMongrel")

A Qt-based application for experimentally using the Monaco Editor as a general editor application.

```
ADD MORE!
```

## Usage

### Platforms

Tested on Ubuntu 20.04 with:
- default `apt` provided `Qt 5.12.8`
- Qt Installer provided `Qt 5.15.2` ( base install path used: `/home/${USER}/qt` ==>> so, `5.15.2` path: `/home/${USER}/qt/5.15.2` )

In general, it is recommended to use a base path like: `/home/${USER}/qt` or `/opt/qt` for installation, so it becomes easy to use the same build commands across different machines.


### Get Sources

Clone the repo as usual:
```bash
git clone https://github.com/coolbreeze413/squintymongrel.git
cd squintymongrel
```


### Build Local Install

For distro provided installation of Qt, we can use:

```bash
make
```


If we want to use a separate non-distro-provided installation of Qt, using a Qt Installer, then:

1. Ensure that we remove the CMake `build` directory once (if built earlier) to clear the cache and any artifacts first:
   
   ```bash
   make distclean
   ```


2. Call `make` with the `qmake` path of the desired Qt Installation to use:

   ```bash
   make QMAKE_PATH=/path/to/bin/qmake
   ```

   For example, if we have `Qt 5.15.2` installation at: `/home/${USER}/qt/5.15.2`  
   Then, qmake path for gcc, x86_64 will be at: `/home/${USER}/qt/5.15.2/gcc_64/bin/qmake`  
   and then we use:  
   ```bash
   make QMAKE_PATH=/home/${USER}/qt/5.15.2/gcc_64/bin/qmake
   ```


### Run Local Install

For distro provided installation of Qt, we can use:

```bash
./install/bin/squintymongrel
```


If we want to use a separate non-distro-provided installation of Qt, using a Qt Installer, then to run the built application, set the `LD_LIBRARY_PATH` variable to the corresponding Qt install `lib/` path:

```bash
LD_LIBRARY_PATH=/path/to/qt/install/gcc_64/lib:$LD_LIBRARY_PATH ./install/bin/squintymongrel
```

For example, if we have `Qt 5.15.2` installation at: `/home/${USER}/qt/5.15.2`  
Then, lib path for gcc, x86_64 will be at: `/home/${USER}/qt/5.15.2/gcc_64/lib`  
and then we use:  
```bash
LD_LIBRARY_PATH=/home/${USER}/qt/5.15.2/gcc_64/lib:$LD_LIBRARY_PATH ./install/bin/squintymongrel
```


## Attributions

The awesome `squintymongrel` image used in the application is:  
Dog by Cristiano Zoucas  
https://thenounproject.com/icon/dog-151542/  
CC BY 3.0  
