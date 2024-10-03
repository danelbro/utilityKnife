# utilityKnife - a 2D game development framework (mainly for me) #

I’m planning to add to this as I make little games and thought I’d better give it a name. If anyone else finds some use for it that’s cool.

The framework is a wrapper for SDL and has some other useful classes that you might want in games.

## Building ##

### Linux ###

You need:

* git
* CMake (v3.27+)
* GCC (v10.1+)

```
git clone –-recursive https://github.com/danelbro/utilityKnife.git
cd utilityKnife
./config.sh
./build.sh
```

### Windows ###

```
git clone --recursive https://github.com/danelbro/utilityKnife.git
```

Run CMake with these options:

```
BUILD_SHARED_LIBS=OFF
SDLTTF_VENDORED=ON	// requires that the repository is cloned recursively - otherwise you'll need SDL_ttf installed on your system
SDL_TEST_LIBRARY=OFF
```
