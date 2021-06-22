# knockout

knockout is a simple program to record time punches

## Example Usage

The following example punches in, punches out, then prints the timecard

```console
$ punch in
Punched in at 10:22 AM
$ punch out
Punched out at 10:22 AM
$ punch cards
Tue 06/22
Time in:        10:22 AM
Time out:       10:22 AM
Total hours: 0.008
```

You can also `punch cards archive` to move the current timecards to an archived
folder and start the time card list fresh. This is useful to restart the
timecards after a pay-period ends.

## Requirements
- C++17
- CMake >= 3.10
- Boost

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

## Future Development

Some things I may or may not add in the future

- a graphical user interface
- an optional message to associate with a timecard
- a more robust archive system
