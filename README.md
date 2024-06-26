![tval](icon.png)
<p align="center">
  <img src="https://img.shields.io/badge/License-Apache%202.0-blue.svg" alt="License">
</p>
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
sudo make install
```
It will install `tval` in your `/usr/local/bin` folder.

## How to use ?
To use `tval` it's easy !
```shell
tval [OPTIONS] "EXPRESSION"
```

### Options
There is several options you can have access :
- `-h`, `--help` : Print the help message
- `-p PRECISION` : Set the precision of the result (default : 12)
- `-s` : Print the result in the scientific format

## Example
```shell
tval "2 +5/2"
4.500000000000
```

```shell
tval -p 0 "sin(pi)"
-0
```

```shell
tval -s -p 2 "2*pi + 230"
2.36e+02
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

## Foot note
I love working on this project. If you have any suggestion about new fonctionnalities dont hesitate !
