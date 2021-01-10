---
layout: default
---

  
  <body>
    <img src="https://cdn.discordapp.com/attachments/555497331848380447/797195454222696448/generatedtextewe.png">
   
   <h2>
      What is Nidhogg Engine?
   </h2>
    <img src="https://cdn.discordapp.com/attachments/555497331848380447/797502304374226954/phosot.png">
   
   <p>Nidhogg Engine is a university project of the 3rd year of the career of GDD(video Game Design and Development) from the <a href="https://www.citm.upc.edu/ing/">CITM</a> made by two students in Terrassa(Spain), were we made a 3D Game Engine in C++.
  We've made this Engine for the UPC assignment Game Engines. Our main goal was to create a basic but functional tool to create 3d games with openGL and SDL. The engine consists in an editor were you can add and eddit gameobjects with meshes and textures, a resource manager which is really useful to manage all the assets used in the project and a system to serialize and save all the information stored in a scene and load it afterwards. We also added the first of many subsystems an engine can have, which is a physics engine, more information about it is detailed below.</p>
    
   <h2>Showcase</h2>
<iframe width="560" height="315" src="https://www.youtube.com/embed/XHlPIRvOjkY" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
   
   <h2>
     Who are we?
    </h2>
    <p>We are two students from CITM Terrassa who had created the Nidhogg Engine. Here you can click on our names to go to our github accounts.</p>
    
   <h5><a href="https://github.com/polf780">Pol de la Torre Solé</a>: </h5>
 
<p> - Game Controls and time management (Play/Pause/Stop...), Editor Camera, ViewPort, Module Editor, Vehicle controller, collider types, graphic designer, user experience.</p>
 
    <img src="https://raw.githubusercontent.com/marcpt98/Fight-club/gh-pages/Web%20images/foto_pol_dela_torre.png" width="400">
    
   <h5><a href="https://github.com/Ap011y0n">Albert Garcia Belerda</a>: </h5>
  
<p> - 3D models import, Materials and texture systems, Resource management system, Constraints and basic collision system, GameObjects structure and components, Scene Scerialitzation,  hot reload, culling and mouse picking.</p>
 
   <img src="https://cdn.discordapp.com/attachments/498515828610957332/797540430774009936/yo.png" width="200">

    
   <h2>
     Main core subsystems
   </h2>
This Engine has 5 main core subsystems: 
- GameObjects & Components core: in charge of the management of all the objects in the engine. This core controls the addition and deletion of all the gameOjbects on the world. Every GameObject parameter can be controlled through the GameObjects inspector and hierarchy. Every GameObjects has his owns components, there are three types: Mesh, Transforms, Collider and Material.
- UI core: in charge of the management of all HUD of the Engine. This core, like the GameObjects core, can control the addition, deletion and edition of it's elements. You can monitorize the performance of the engine, edit the theme, edit the window mode, etc.
- Serialitzation core: With the serialitzation module we can save our scene information, so this way we can save our progress so later we can load it again on the engine. Same goes for the play and stop system, which uses this module to save the scene when pressign play so it can go back to it's previous state when pressing stop.
- Resource Manager core: We have a main module to deal with the resources were we check the assets from the assets folder, create custom files in the library folder and avoid loading the same asset two times because of its reference counting system. 
- Importer core: With the imoprter module we can import and change some options of the images and models we import into the engine.

   <h2>
     Physics core:
   </h2>
   For the physics we have integrated the Bullet library into the Editor, now all GameObjects can create a collider component which will use the physics from Bullet, there
   are 3 different types of colliders to choose (capsuel, sphere, box), you can also delete any collider component from a GameObject and create another. When creating a collider you can decide it's mass or make it a collider sensor, which detects collisions but is unaffected by any force. There's also the option to create it as a static body, which affects the other bodys but is unaffected by external forces.
   
   <img src="https://cdn.discordapp.com/attachments/796830125256736808/797521691932229662/ezgif.com-gif-maker_2.gif" width="800" height="450" />
   
   The engine also let you modify the size, position and scale of the collider.
   There are diferent types of constraints, a total of 4 diferents constraints (p2p , hinge, slider and Cone) all of the are adjustable from the editor, and some of them have more optioons once created, such as the angle limits of the slider cosntraint and the motor of the hinge constraint. As the other copmponents, colliders are too serialized into the save files, and even all this customitzation is saved so they can return to their original positions and sizes.
   
   The camera now has a collider so it can collide with all colliders and interact with them, there also a car which you can easily control to test collisions with the objects distributed throguh the scene.
   
    
    <img src="https://cdn.discordapp.com/attachments/796830125256736808/797532436862074910/ezgif.com-gif-maker_3.gif" width="800" height="450" />

   <h2>So…
      </h2>

 <h4>Do you want to try out our project?? Download it using this <a href="https://github.com/Ap011y0n/Nidhogg-Engine/releases/tag/v1.0">link</a>!</h4>
 <h4>For more information, check our <a href="https://github.com/Ap011y0n/Nidhogg-Engine">repository</a> in Github: </h4> 
     
<h2>
    Licence
</h2>

//MIT License

Copyright (c) [2019] [Pol de la Torre Solé and Albert Garcia Belerda]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.//

  </body>

