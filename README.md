# gayclicker - a decently fast x11 auto clicker

an autoclicker for x11

assign a shortcut to run it and the first time you press the shortcut it will begin autoclicking, the next time you press it, the program realizes theres already a gayclicker instance running and stops it. like a toggle

optional argument to specify clicks per second, if omitted will attempt to click as fast as possible (i got upwards of 4700 cps)

## example

```bash
$ gayclicker # click as fast as possible
$ gayclicker 20 # click at 20 clicks per second
```

## build

just run `make build` in the root of the repo and the binary will be placed at build/gayclicker

## prequistiteisies

x11
