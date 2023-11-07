# tval

**tval** is an easy to use shell application for quick and easy calculation.

## Why ?
When I'm working on a project and I need a quick calcul I usualy use Python. The problem is that it require many step.
- I launch python
- If i need some math function like `log10` or `exp` I need to import the `math` module.
- And then I can type my calcul and get the result.

This is to many step for a quick calcul this is why I created `tval` for `Terminal (Math Expression) Evaluation`.

## How to build ?
To build `tval` it's easy. There is 2 way of doing it.
### Using the build script
```shell
./build.sh
```
It will juste generate the `tval` executable.
### Using Make
```shell
make install
```
It will install `tval` in your `/usr/local/bin` folder.

## How to use ?
To use `tval` it's easy !
```shell
tval "YOUR MATH EXPRESSION"
```

## Example
```shell
tval "2 +5/2"
4.500000000000
```

```shell
tval "sin(pi)"
-0.000000000000
```

```shell
tval "exp(2)"
7.389056098931
```

## Available Constants and Functions
### Constants
- `pi`/`pie` : 3.141592653589793238462643383279502884197
- `e` : 2.7182818284590452353602874713526624977572
- `gr` *(golden ratio)*: 1.61803398874989484820458683436563811
### Functions
- `log`, `log10`, `log2`
- `exp`
- `tanh`, `sqrt`
- `sin`, `cos`, `tan`, `atan`

## Todos
### Bugs
- Expression like `tan(pi/2)` should return `inf` and not the maximum value for a `long double`.

### What I want to work on ?
- Add `-p` parrameter to be able to set the precision of the result.
- Include more functions and constants.
- Add complex numbers