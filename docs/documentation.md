# Project documentation
:3

## Data processing pipeline
Overview of what happens when starting the renderer.

1. [Loading the models from file](#reading-model-data)
1. [Initialising scene components](#scene)
1. [Processing the polygons](#math-pipeline)
1. [Rasterization and z-buffer checking](#rasterization)
1. [Pseudo fragment shader](#pseudo-fragment-shader)
1. [Printing the result by copying the buffer to device context](#window)

## Reading model data
As stated previously in `README.md`the `scene.txt` file that links models with their `.obj` and `.mtl` files, follows this convencion:
```txt
model_name_without_extension x_coordinate y_coordinate z_coordinate
next_model_name x_coordinate y_coordinate z_coordinate
```
As mentioned in `README.md` providing additional coordinates in the same line will create instances of the model.\
The `scene.txt` and `scene2.txt` files are processed line by line. Each time first a new [Entity](#entity) is created and assigned a new [material](#material) with the texture described in the `.mtl` file. If the `.mtl` file does not contain a `map_Kd` line, the [material](#material) creation returns `nullptr` for which default blue and green primitives will be rendered. 
Transforms of instances are created from the sets of coordinates and added to the new [Entity](#entity), then the `.obj` file with the model's name is opened.\
From `.obj` file the vertices, texture vts, normals and faces are read and pushed to temporary vectors and bigger polygons are processed via fan triangulation method.
Then the temporary vertices vector is normalized for model coordinates and after that is done the temporary vectors are passed by reference to the new  [Entity](#entity) that was passed by reference to this loading function.\

## Internal organisation
Data ownership and properties of the components

### Renderer
The `Renderer` instance stores the `float* depthbuffer` and clears it by assigning `300.f`to all cells before calling the [rasterization function](#rasterization). It also stores the `uint32_t* buffer` that is the frame buffer shared with the `Window` class as well
as the predefined projection matrix and `std::vector<std::array<Vertex, 3>> triangles` which stores projected triangle polygons of all the models with the coordinates of vertices and uv for texturing.
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
is the one that clears the frame buffer and prints the data put there by the `Renderer` to the screen. The window is also responsible of printing the on screen information such as number of primitives rendered.

### Scene
The `Scene` instance owns a `Camera` and `std::vector <Entity*> objects`. It is responsible for starting the process described in [Reading model data](#reading-model-data). Since the lifetimes of a `Camera` instance and all objects (`Entity` class) are owned by the
`Scene` it provides functions that return by refernce the `Camera` instance and `Entity` instances given their index (which is being used by the `Renderer` instance to iterate over all objects to render them).\
The scene also owns the normalized directional light vector and has a function for rotating it.\
The project is set up to handle 2 scenes that can be switched between by pressing the "h" key. Both scenes are loaded before the main loop but because of the OS's lazy loading, switching to the second scene for the first time will cause a process memory usage spike.

### Entity
An `Entity` instance stores its mesh, pointer to a `Material` and a `std::vector<Transform>` of its instances.
```cpp
struct Mesh
{
	std::vector<Vec<float, 3>> v;
	std::vector<Vec<float, 2>> vt;
	std::vector<Vec<float, 3>> vn;

	std::vector<Vec<int, 3>> f;
	std::vector<Vec<int, 3>> fvt;
	std::vector<Vec<int, 3>> fvn;
};

struct Transform
{
	Vec<float, 3> position;
	Quaternion rotation;
	Vec<float, 3> scale;
};
```
An `Entity` instance can call a method to return the model matrix of a choosen instance of the model by index.

### Material
A `Material` instance stores its width, height and `uint32_t* texture` that stores the texture data in the `0xAARRGGBB` format. It returns the texture colour at uv coordinates (the uv coordinates are normalized inside the function not before).\
If the given uv coordinates point to a pixel out of range a predefined red colour will be returned.

## Math pipeline
How the 3D models are processed every frame.\
\
The `Vertex` struct used in it:
```cpp
struct Vertex {
	Vec<float, 4> v;
	Vec<float, 2> vt;
	Vec<float, 3> vn;
	Vertex(Vec<float, 4> iv, Vec<float, 2> ivt, Vec<float, 3> ivn) : v(iv), vt(ivt), vn(ivn) {}
};
```

1. ### MVP matrix creation
	[Entity](#entity) (along with the instance index), [Camera](#camera) and the projection matrix from [Renderer](#renderer) are passed by reference to the function. From [Entity](#entity) instance and [Camera](#camera) we get the model and view matrix which multiplied together with the projection matrix give us the MVP matrix.
1. ### projecting the vertices
	Iterating through the vertices of the passed model we create a 4-component vector called `Vec<float, 4> vertex` that represents the current local vertex with fourth component being `1` to create homogenous coordinates. That vertex is multiplied by the MVP matrix which gives us
	clip coordinates that are then pushed to the `std::vector <Vec<float, 4>> projected` vector.
1. ### clipping
	In the clipping stage we iterate through the faces of the model. A `std::vector<Vertex> polygon` is made using indexes from the current face that poit to vertices from `projected` vector from the last step and pair them with vt and vn values. The `polygon` is passed by reference for near plane clipping where
	we check if the current vertex and the next one is inside (`w` of that vertex `>= 0.1f`). If both are inside we push back the second one to the outputs, if the second one is inside we push back the intersection and the second one, if the first one is inside we push back the intersection.
	This proces repeats for all vertices and for the last one the second vertex is the first one of the polygon.\
	If the `polygon` is empty after near clipping we skip that face.\
	Then for each vertex of the `polygon` we process homogenous clip space to normalized device coordinates (`NDC`) and if a vertex is outside the screen after normalization we set a clip flag to true.\
	If the clip flag is true the `polygon` is clipped using the Sutherland-Hodgman algorithm.
1. ### converting NDC to pixel coordinates
	Every point in the `polygon` is converted to pixel coordinates and if there are more than 3 vertices we use fan triangulation to divide the `polygon` into triangles that are pushed to the `std::vector<std::array<Vertex, 3>> triangles` for rasterization.
1. ### Rasterization
	A [Renderer](#renderer) method iterates through all of the projected triangles inside `std::vector<std::array<Vertex, 3>> triangles`. Every triangle is checked if it is clockwise using the cross product method. Counter-clockwise triangles are not printed (backface culling).
	For every triangle that is being printed we get its bounding box known in code as `coordinateBlock` and check (using barycentric coordinates) for each pixel if it is inside the triangle (checking the middle of the pixel by adding `0.5f`).  If the point lies within the triangle
	we get the depth from dot product of the vector with the triangle vertices z values and the barycentric weights vector. If that depth is lower than the value present at that coordinate in the z buffer it replaces it.
1. ### Pseudo fragment shader
	We pass the vt coordinates that were processed along the vertex coordinates to a function with the current triangle being printed and its weights and return the uv coordinates. The uv coordinates are passed by reference to the model's [material](#material) that normalizes them and returns
	the texture colour they point to. If the pointer to the [material](#material) is `nullptr` the colour returned is a either green or blue depending on the primitives order in the model. Then the vn values are also interpolated using the weights and normalized.
	The diffuse value (Lambertian Reflectance) is calculated as a dot product of the calculated normals and directional light and clamped to 0 if lower.\
	Intensity is calculated like this:
	```cpp
	float intensity = AMBIENT_LIGHT + (1.0f - AMBIENT_LIGHT) * diffuse;
	```
	and it is applied to the RGB channels of the obtained colour (either from the material or the default one). The AMBIENT_LIGHT is defined as `0.25f`. 

### quaternions
Quaternions are managed using these functions:
* `normalizeQuaternion` scales the magnitude to `1` by dividing by lenght. if the lenght is `0` it sets it to a unit quaternion.
* `axisAngle` normalizes the length of input vector, multiplies the imaginary part by `1 / normalized_lenght`, divides given angle by 2, sets the scalar to cos of the angle and multiplies the imaginary part by the sin of the angle.
* `rotateVector` Rotates the vector using the `v' = qvq*` formula.
\
\
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