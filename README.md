# 3D CPU based renderer
A project recreating the 3d rendering pipeline on the CPU and showing performance differences when using SIMD (to be implemented) and without.\
\
Code documentation is at: `docs\documentation.md`.


## Controls
Use WSAD to move the camera horizontaly, shift and space for vertical movement. Q and E for turning left and right.

## Viewing your models
The renderer requires `.obj` and `.mtl` files. To view your models modify the `scene.txt` file following this convencion:

```txt
model_name_without_extension x_coordinate y_coordinate z_coordinate
next_model_name x_coordinate y_coordinate z_coordinate
```

## Compilation
Compile with Visual Studio 2022 or newer.

## Sources
Sources i used to learn about 3D graphics and to make this project.

* Real-Time Rendering, Fourth Edition by Tomas Akenine-Möller, Eric Haines, Naty Hoffman
* [songho.ca](https://www.songho.ca/index.html)
* [learnopengl](https://learnopengl.com/)
* Code for creating a Windows API window is from [this tutorial](https://www.youtube.com/watch?v=Kx5CN-V6FvQ) by Aerideus.

I used GenAI for small code fragments and explanations.
