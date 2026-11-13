# 3D CPU based renderer
A project recreating the 3d rendering pipeline on the CPU and showing performance differences when using SIMD (to be implemented) and without.\
\
The project uses [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)\
\
Code documentation is at: `docs\documentation.md`.


## Controls
Use WSAD to move the camera horizontaly, shift and space for vertical movement. Q and E for turning left and right.\
Press H to switch between scenes.\
Press X to switch between rendering modes. (to be implemented)

## Viewing your models
The renderer requires `.obj` and `.mtl` files. To view your models modify the `scene.txt` or `scene2.txt` file following this convencion:

```txt
model_name_without_extension x_coordinate y_coordinate z_coordinate
next_model_name x_coordinate y_coordinate z_coordinate
```

Providing additional sets of `x_coordinate y_coordinate z_coordinate` in the same line will create instances of that model. `scene2.txt` has an example of instancing.\
`scene2.txt` is much more complex and will cause a spike in process memory usage once switched to. Once loaded the scene data isn't freed until the end of the program.\
Models can also be scaled and rotated but that is not supported by the scene format and if the user wishes to do so they have to use functions given in the commented example in `main.cpp`.\
\
Default models present in the scene are: an untextured cube, Suzzane, Stanford bunny.\
The Stanford bunny model has visible holes at the bottom of it that are not a result of erronous culling.


## Compilation
Compile with Visual Studio 2022 or newer.

## Sources
Sources i used to learn about 3D graphics and to make this project.

* Real-Time Rendering, Fourth Edition by Tomas Akenine-Möller, Eric Haines, Naty Hoffman
* [songho.ca](https://www.songho.ca/index.html)
* [learnopengl](https://learnopengl.com/)
* Code for creating a Windows API window is from [this tutorial](https://www.youtube.com/watch?v=Kx5CN-V6FvQ) by Aerideus.

I used GenAI for small code fragments and explanations.
