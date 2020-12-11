#Nidhogg Engine 
This is a university project of the 3rd year of the career of GDD(video Game Design and Development)from the CITM (https://www.citm.upc.edu/), made by two students in Terrassa(Spain), were we made a 3D Game Engine in C++. This version of the engine works as a Geometry Viewer that suports drag and drop texture.

Link to the github page of the project: https://github.com/Ap011y0n/Nidhogg-Engine

##Team Members

* Albert Garcia Belerda
* Github: Ap011y0n (https://github.com/Ap011y0n)

* Pol de la Torre Solé
* Github: polf780(https://github.com/polf780)


### Assigment 1

##How to use the Engine
On the left side there's the Hierarchy window where you can see the Game Objects with their correspondent childs, when clicking one the Inspector on the right side will show you the selected Game Object components.
  
On the Inspector when a Game Object is selected you can delete it using the first button on top of the inspector, you can also change the name of the selected Game Object on the Inspector.
The console window on the bottom side outputs information about the imported files and textures.

There is a Main menu bar in the top side with multiple sub menus, which allows you to:
- Open the GameObjects menu to create Primitives shapes. 
- Edit submenu where you can change the theme of the Editor or open the Configuration menu.
- The window sub menu let you to turn on/off editor windows. 
- Help submenu that has multiple options and information about the engine and the students.

When we open the Configuration menu it appears below the Inspector, with multiple sub menus to change some aspects from the Engine:
- Application: change name of the app, max of fps and has a graph of the current fps.
- Window: change the width, height, Brightness and let enable or disable multiple options from the window.
- Input: Shows all the input from the keyboard or the mouse.
- Hardwere: Shows info like the system Ram, the graphic card name, the number of cores, all info related to    hardware. 

When a Game Object is selected in the Hierarchy you can see their multiple components and you can also modify them using the mouse or the keyboard. 
- In the transform component you can modify a Game Objects Scale, Position and Rotation.
- On the component mesh you can display the Normals faces from de object.
- The material component you can change the texture of the Game Object for the checkers texture and also see   the name the texture and it's width and height.

The engine supports drag and drop of files like fbx, png and dds textures.   

#Controls

##Camera Controls
- Mouse right click: Rotate camera.
- Mouse wheel: to move forward and backwards the camera.
- Shift + left click: Focus the camera on the selected Game Object and rotate on it's orbit.
- WASD buttons: move camera postion, forward, bacward and sideways.
- Shift + (WASD): move camera postion faster. 
- F: Focus the camera on the slected Game Object.

##Additional funcionality
- If you move a Game Object the father will move all the children, but when selecting a children and moving    him it will move/rotate/Scale independently of the father.
- On the inspector we can change the name of any Game Object independently. 
- There is an option in the Menu Bar inside Edit to change the Engine Theme.
- We have added on top of the inspector a Delete button that will delte the Game Object, when deleting a     Game Object it will also delete all his children. 

##Comments
- We have added the png of Lenna for testing purposes.
- We have added an dds for testing purposes.
- The changing name function loses the Focus so it's this function is still a work in progress.



### Assigment 2
Now on the Hierarchy window you can reparent all gameobjects by selecting them and dragging them into another GameObject, if you want to select one of the GameObjects from the Hierarchy you just need to left click the name of the GameObject. After selecting an object in the hierarchy the right click will open a popup where you can choose between several options, the first one is deleting the selected GameObject, creating an empty child for the selected GameObject and creating a new empty GameObject in the scene.

There is a new window below the hierarchy where we can see the different resources than can be used by the user.
After selecting some in the right part below there is a window where we can see the process where the resources use reference counting , and we can see how many there are in the scene, also there is a button to import them into the scene.

Now we can move the Objects in the scene directly by selecting them with the mouse in the Game window, with the left click, after selecting one through the game window we are able to see it selected in the game Hierarchy as well, after selecting a GameObject it will appear the guizmo which can move rotate and scale the gameobject itself.
We added on the main menu bar inside File two new options, one to save the scene and another one to Load the previous scene saved.

There is now another option inside the option GameObject in the menu bar, where you can create Cameras inside the scene, when selecting a camera in  the inspector we can change the position of the Far plane, near plane, and the value Fov of the camera.

On the top side under the main menú bar there is a new window with general options, where we can change the Guizmo from world to local, also we can put visible all the bounding boxes, and we can put on Play mode the engine, meanwhile is os play mode we can Pause or Stop, when he use the stop button the game will turn to the state before pushing the play button.

##New controls and Shortcuts
F1: Load Bakerhouse.fbx
F3: Save scene.
F4: Load scene.
E: Guizmo translate mode.
R: Guizmo rotate mode.
T: Guizmo scale mode.

##In progress
Locals in gizmo
The outliner
Delete resources

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
