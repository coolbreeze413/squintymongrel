#ifndef SQUINTYMONGREL_CONFIG_H
#define SQUINTYMONGREL_CONFIG_H

// always wrap the defines within #ifndef to allow the build system to pass in the
// flags using compiler definitions

#ifndef SQUINTYMONGREL_USE_SPLASH_SCREEN
    #define SQUINTYMONGREL_USE_SPLASH_SCREEN 1
#endif // #ifndef SQUINTYMONGREL_USE_SPLASH_SCREEN

#ifndef QT_NO_SYSTEMTRAYICON
    #define QT_NO_SYSTEMTRAYICON 0
#endif // #ifndef QT_NO_SYSTEMTRAYICON

#endif // #ifndef SQUINTYMONGREL_CONFIG_H