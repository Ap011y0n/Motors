#Nidhogg Engine 
This is a university project of the 3rd year of the career of GDD(video Game Design and Development)from the CITM (https://www.citm.upc.edu/), made by two students in Terrassa(Spain), were we made a 3D Game Engine in C++. This version of the engine works as a Geometry Viewer that suports drag and drop texture.

Link to the github page of the project: https://github.com/Ap011y0n/Nidhogg-Engine

##Team Members

* Albert Garcia Belerda
* Github: Ap011y0n (https://github.com/Ap011y0n)

* Pol de la Torre Solé
* Github: polf780(https://github.com/polf780)

##How to use the Engine
On the left side there's the Hierarchy window where you can see the Game Objects with their correspondent childs, when clicking one the Inspector on the right side will show you the selected Game Object components.
  
On the Inspector when a Game Object is selected you can delete it using the first button on top of the inspector, you can also change the name of the selected Game Object on the Inspector.
The console window on the bottom side outputs information about the imported files and textures.

There is a Main menu bar in the top side with multiple sub menus, which allows you to:
- Open the GameObjects menu to create Primitives shapes. 
- Edit submenu where you can change the theme of the Editor or open the Configuration menu.
- The window sub menu let you to turn on/off editor windows. 
- Help submenu that has multiple options and information about the engine and the students.

When a Game Object is selected in the Hierarchy you can see their multiple components and you can also modify them using the mouse or the keyboard. 
- In the transform component you can modify a Game Objects Scale, Position and Rotation.
- On the component mesh you can display the Normals faces from de object.
- The material component you can change the texture of the Game Object for the checkers texture and also see   the name the texture and it's width and height.

The engine supports drag and drop of files like fbx, png and dds textures. To change a texture simply select a gameobject with a texture component and drag a png or a dds.   

#Controls

##Camera Controls
- Mouse left click: Rotate camera.
- Mouse wheel: to move forward and backwards the camera.
- Shift + right click: Focus the camera on the selected Game Object and rotate on it's orbit.
- WASD buttons: move camera postion, forward, bacward and sideways.
- Shift + (WASD): move camera postion faster. 
- F: Focus the camera on the slected Game Object.

##Keyboard

## License & Copyright 

[MIT License]

Copyright (c) 2019 by Pol de la Torre & Albert Garcia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.