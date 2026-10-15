# Project documentation
:3

## Data processing pipeline
Overview of what happens when starting the renderer.

1. [Loading the models from file](#reading-model-data)
1. [Initialising scene components](#scene)
1. [Processing the polygons](#math-pipeline)
1. [Rasterization and z-buffer checking](#rasterization)
1.  more steps to be implemented
1. [Printing the result by copying the buffer to device context](#window)

## Reading model data
As stated previously in `README.md`the `scene.txt` file that links models with their `.obj` and `.mtl` files, follows this convencion:
```txt
model_name_without_extension x_coordinate y_coordinate z_coordinate
next_model_name x_coordinate y_coordinate z_coordinate
```
The `scene.txt` file is processed line by line. Each time first a new [Entity](#entity) is created with given xyz coordinates passed to its constructor, then the `.obj` file with the model's name is opened.\
From `.obj` file the vertices and faces are read and pushed to temporary vectors and bigger polygons are processed via fan triangulation method.
Then the temporary vertices vector is normalized for model coordinates and after that is done the temporary vectors are passed by reference to the new object ([Entity](#entity) instance) that was passed by reference to this loading function.\
\
texture loading is yet to be implemented and currently the polygons use a predefined set of flat colours.


## Internal organisation
Data ownership and properties of the components

### Renderer
The `Renderer` instance stores the `float* depthbuffer` and clears it by assigning `300.f`to all cells before calling the [rasterization function](#rasterization). It also stores the `uint32_t* buffer` that is the frame buffer shared with the `Window` class as well
as the predefined projection matrix and `std::vector<std::array<Vec<float, 3>, 3>> triangles` which stores projected triangle polygons of all the models.
The width and height of the `Window` is passed to the `Renderer` by value so it can create a z buffer of appropriate size and pass those values for model projection.\
\
The projection matrix:
```cpp
#define FOV 45.f * (M_PI / 180.0)
#define FRONT 1.f
#define BACK 100.f

//the part below is executed in the constructor

	float tangent = std::tan(FOV / 2);
	float right = FRONT * tangent;
	float aspect = (float)width / (float)height;
	float top = right / aspect;

	projection = {
	FRONT / right, 0, 0, 0,
	0, FRONT / top, 0, 0,
	0, 0, -(BACK + FRONT) / (BACK - FRONT), -1,
	0, 0, -(2 * BACK * FRONT) / (BACK - FRONT), 0
	};
```
### Camera
The camera stores its struct transform which contains a `Vec<float, 3> position` and a `Quaternion rotation`. It is set to be looking at negative Z and it's starting position is `0, 0, -4.f`. A `Camera` instance can call a method to return the view matrix.

### Window
`<Windows.h>` required.\
The `Window` width and height are set to be `1366` and `768`. It stores `uint32_t* buffer` and `BITMAPINFO bmi` and manages key inputs in the main loop. The `uint32_t* buffer` (the frame buffer) is shared with the `Renderer` instance but the `Window` instance
is the one that clears the frame buffer and prints the data put there by the `Renderer` to the screen.

### Scene
The `Scene` instance owns a `Camera camera` and `std::vector <Entity*> objects`. It is responsible for starting the process described in [Reading model data](#reading-model-data). Since the lifetimes of a `Camera` instance and all objects (`Entity` class) are owned by the
`Scene` it provides functions that return by refernce the `Camera` instance and `Entity` instances given their index (which is being used by the `Renderer` instance to iterate over all objects to render them).\
The project is coded to support only one scene loaded from the `scene.txt` file but `Renderer` renders a `Scene` passed to it by reference so the project can be easily modified to support more scenes.

### Entity
An `Entity` instance stores its mesh and transform:
```cpp
struct Mesh
{
	std::vector<Vec<float, 3>> vertices;
	std::vector<Vec<int, 3>> faces;
};

struct Transform
{
	Vec<float, 3> position;
	Quaternion rotation;
	Vec<float, 3> scale;
};
```
An `Entity` instance can call a method to return the model matrix.

## Math pipeline
How the 3D models are processed every frame.

1. ### MVP matrix creation
	[Entity](#entity), [Camera](#camera) and the projection matrix from [Renderer](#renderer) are passed by reference to the function. From [Entity](#entity) and [Camera](#camera) we get the model and view matrix which multiplied together with the projection matrix give us the MVP matrix.
1. ### projecting the vertices
	Iterating through the vertices of the passed model we create a 4-component vector called `Vec<float, 4> vertex` that represents the current local vertex with fourth component being `1` to create homogenous coordinates. That vertex is multiplied by the MVP matrix which gives us
	clip coordinates that are then pushed to the `std::vector <Vec<float, 4>> projected` vector.
1. ### clipping
	In the clipping stage we iterate through the faces of the model. A `std::vector<Vec<float, 4>> polygon` is made using indexes from the current face that poit to vertices from `projected` vector from the last step. The `polygon` is passed by reference for near plane clipping where
	we check if the current vertex and the next one is inside (`w` of that vertex `>= 0.1f`). If both are inside we push back the second one to the outputs, if the second one is inside we push back the intersection and the second one, if the first one is inside we push back the intersection.
	This proces repeats for all vertices and for the last one the second vertex is the first one of the polygon.\
	If the `polygon` is empty after near clipping we skip that face.\
	Then for each vertex of the `polygon` we process homogenous clip space to normalized device coordinates (`NDC`) and if a vertex is outside the screen after normalization we set a clip flag to true.\
	If the clip flag is true the `polygon` is clipped using the Sutherland-Hodgman algorithm.
1. ### converting NDC to pixel coordinates
	Every point in the `polygon` is converted to pixel coordinates and if there are more than 3 vertices we use fan triangulation to divide the `polygon` into triangles that are pushed to the `std::vector<std::array<Vec<float, 3>, 3>> triangles` for rasterization.
1. ### Rasterization
	A [Renderer](#renderer) method iterates through all of the projected triangles inside `std::vector<std::array<Vec<float, 3>, 3>> triangles`. Every triangle is checked if it is clockwise using the cross product method. Counter-clockwise triangles are not printed (backface culling).
	For every triangle that is being printed we get its bounding box known in code as `coordinateBlock` and check (using barycentric coordinates) for each pixel if it is inside the triangle (checking the middle of the pixel by adding 0.5f).  If the point lies within the triangle
	we get the depth from dot product of the vector with the triangle vertices z values and the barycentric weights vector. If that depth is lower than the value present at that coordinate in the z buffer it replaces it and also changes the colour at that position in the frame buffer.

### quaternions
Quaternions are managed using these functions:
* `normalizeQuaternion` scales the magnitude to `1` by dividing by lenght. if the lenght is `0` it sets it to a unit quaternion.
* `axisAngle` normalizes the length of input vector, multiplies the imaginary part by `1 / normalized_lenght`, divides given angle by 2, sets the scalar to cos of the angle and multiplies the imaginary part by the sin of the angle.
* `rotateVector` Rotates the vector using the `v' = qvq*` formula.
When rotating an object the input is given like for euler angles `(angle_x, angle_y, angle_z)` and then `axisAngle` calculated for each one:
```cpp
	Quaternion qx = axisAngle({ 1, 0, 0 }, d.x);
	Quaternion qy = axisAngle({ 0, 1, 0 }, d.y);
	Quaternion qz = axisAngle({ 0, 0, 1 }, d.z);

	Quaternion delta = multiply(qz, multiply(qy, qx));
```
The objet's transform rotation quaternion is multiplied by `delta` and then passed by reference to `normalizeQuaternion`.
\
\
[go to the top](#project-documentation)