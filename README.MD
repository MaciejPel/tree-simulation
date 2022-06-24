# Simulation of a growing tree with OpenGL
Project made for computer graphics and visualisation (4th semester). The tree is generated recursively according to principles similar to fractals. The base is a cone

## Usage
Scene related changes should be made in `draw_scene` function, initial scene changes in `init_opengl_program` and changes after execution of a main loop in `free_opengl_program`. Additional variables should be initilized globally or in `main` function.

## Performance and overall results
Certainly, instead of generating the cone recursive number of times you could make one and then pass the arguments. Same applies for the calculations. I don't excatly know how it would work with the `tree_growth` but  it would improve drawing of the scene for sure.

## Results
![alt text](https://raw.githubusercontent.com/MaciejPel/tree-simulation/main/tree.gif?raw=true)

## Contributing
I won't be updating this project, feel free to fork and play around.
