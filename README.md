# 3D-version-Gaunlet
Gaunlet is a classic Arcade game. 
This project is to create a 3D version game of Gaunlet with OpenGL and C++.
[A short demonstration](https://www.youtube.com/watch?v=OMrUBLJSWFA&t=68s) has been uploaded to Youtube.
For more detailed information, you can refer to the description (PDF) in this repository.
All the code are in the CG_Project1 folder, while the 3D models are in the Release folder.

# Technology
Summarise some technologies used for this project.
All the objects in this game are rendered with Phong model.
Shadows are rendered using PCF (percentage-closer filtering).
Skeletal animation is also implemented with the help of Assimp (Assimp helped us parse FBX files).
Execpt the model of the character (from Mihoyo Genshin Impact) and the cabin (from a free 3D model site), all the models are made by me using Blender.
The skeletal animation is also made by me in Blender.

# Dependency
The project is build in Visual Studio 2017.
The library used (a part) are GLM-0.9.8.5, GLEW-2.1.0, GLFW-3.3.2, GLAD, Assimp5.0.0. 
The Assimp version is important (for load textures of FBX files) and should be build using Cmake to suit your system.
