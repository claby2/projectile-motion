## Projectile Motion
Projectile motion simulation with parabolic trajectory based on mouse position.
`Left-click` to throw the projectile. When the projectile is thrown, either `left-click` again or press `space bar` to reset projectile to the default position.
Press `m` to toggle between constant and variable initial velocity (default is constant initial velocity). With a variable initial velocity, the mouse Y position will represent the maximum height of the trajectory. When the mouse position y value is below the starting position in this mode, the default initial velocity value will be taken instead.

## Dependencies
```
SDL2
```

## Build and run
```
$ g++ source.cpp -w -lSDL2main -lSDL2 -o source
$ ./source
```