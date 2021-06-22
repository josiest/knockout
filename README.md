# knockout

knockout is a simple program to record time punches

## Requirements
- C++17
- CMake 3.10

## Installation

```console
$ cd <installation-root-dir>
$ git clone https://github.com/josiest/knockout.git && cd knockout
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

In order to run `punch` anywhere on the console, you'll need to add
`<installation-root-dir>/knockout/bin` to your path environment variable.

## Usage

The following example punches in, punches out, then prints the timecard

```console
$ punch in
$ punch out
$ punch log
```
