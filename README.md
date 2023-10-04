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
- Qt Installer provided `Qt 5.15.2` ( base install path used: `/home/${USER}/qt` ==>> so, `5.15.2` path: `/home/${USER}/qt/5.15.2/gcc_64/` )

### Build

If the Qt Version is changed, using `CMAKE_PREFIX_PATH` we are mostly trying to use a separate non-distro-provided installation of Qt, using a Qt Installer.
Recommended to use a base path like: `/home/${USER}/qt` for installation.

In this case, there are 2 things to do:

1. Add the below line in `CMakeLists.txt`, and set path according to the installation on the system:

   ```cmake
   list(PREPEND CMAKE_PREFIX_PATH "<path/to/parent/of/Qt/bin/and/lib>")
   ```

2. Ensure that we remove the CMake `build` directory to clear the cache and any artifact first:
   
   ```bash
   make distclean
   ```

and then build as usual:

```bash
git clone https://github.com/coolbreeze413/squintymongrel.git
cd squintymongrel
make
```




### Run
```bash
./install/bin/squintymongrel
```

If the Qt Version is changed, using `CMAKE_PREFIX_PATH` we are mostly trying to use a separate non-distro-provided installation of Qt, using a Qt Installer.

In this case, to run the built application, set the `LD_LIBRARY_PATH` variable, for example:

```bash
LD_LIBRARY_PATH=/home/${USER}/qt/5.15.2/gcc_64/lib:$LD_LIBRARY_PATH ./install/bin/squintymongrel
```


## Attributions

The awesome `squintymongrel` image used in the application is:  
Dog by Cristiano Zoucas  
https://thenounproject.com/icon/dog-151542/  
CC BY 3.0  
