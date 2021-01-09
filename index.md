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
  We've made this Engine for the UPC assignment Game Engines.</p>
    
   <h2>Gameplay video...enjoy!</h2>
<iframe width="420" height="315" src="https://www.youtube.com/embed/bbJLTymIAWw" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe></br>
   
   <h2>
     Who are we?
    </h2>
    <p>We are two students from CITM Terrassa who had created the Nidhogg Engine. Here you can click on our names to go to our github accounts.</p>
    
   <h5><a href="https://github.com/polf780">Pol de la Torre Solé</a>: </h5>
 
 - Game Controls (Play/Pause/Stop...), Editor Camera, ViewPort, Module Editor, Vehicle controller.
 
    <img src="https://raw.githubusercontent.com/marcpt98/Fight-club/gh-pages/Web%20images/foto_pol_dela_torre.png" width="400">
    
   <h5><a href="https://github.com/Ap011y0n">Albert Garcia Belerda</a>: </h5>
  
 - 3D models import, Materials and texture systems, Resources management systemm, Joints, GameObjects structre and components, Scene Scerialitzation.
 
   <img src="https://cdn.discordapp.com/attachments/555497331848380447/797496930098741308/albert_garcia.png" width="200">

    
   <h2>
     Main core subsystems
   </h2>
This Engine has 5 main core subsystems: 
- GameObjects & Components core: in charge of the management of all the objects in the engine. This core controls the addition, elimination of all the gameojbects on the world. All the management can be controlled throught the GameObjects module. Every GameObjects has his owns components, there are component Mesh,Transforms,Collider and Material.
- UI core: in charge of the management of all HUD of the Engine. This core, like the GameObjects core, can control the addition, elimination of his elements.
- Serialitzation: to save the scane(save laod)
- Resource Manager:
- Importer:

   <h2>
     Physics core:
   </h2>
   For the physics we have integrated the Bullet library into the Editor, now all GameObjects can create a collider component which will follow the physics from Bullet, there
   are 3 diferent types of colliders to choose (capsuel,sphere,box), you can also delete any collider component from a GameObject and create another, when creating a collider      you can decide it's mass or make it a collider sensor.
   
   <img src="https://cdn.discordapp.com/attachments/796830125256736808/797521691932229662/ezgif.com-gif-maker_2.gif" width="800" height="450" />
   
   The engine also let you modify the size, position and scale of the collider, and to choose betwen a static collider or a dynaimc one.
   There are diferent types of constraints, a total of 4 diferents constraints (p2p , hinge, slider and Cone) all of the are adjustable from the editor, and each GameObject just    can have one collider. The camera now has a collider so it can collide with all colliders and interact with them.
   
    <img src="https://cdn.discordapp.com/attachments/796830125256736808/797532436862074910/ezgif.com-gif-maker_3.gif" width="800" height="450" />
   
   <h2>
     How to play (controls):
    </h2>
       <h3>
    Character controls:
    </h3>
    - A: Move left </br>
    - D: Move right </br>
    - F: Shoot </br>
    - SPACE BAR: Jump </br>
    <h3>
    Special controls:
    </h3>
    - F1: Charge level 1 from the begining </br>
    - F2: Charge level 2 from the begining </br>
    - F3: Charge current level from the begining </br>
    - F5: Save your current position </br>
    - F6: Load your past position </br>
    - F8: Show all UI debug </br>
    - F9: Show all colliders and pathfinding </br>
    - F10: God mode (player without collisions and can fly) </br>
    - F11: Cap frames (siwtch fps cap between 30-60) </br>
    - º: Open console </br>

<h2>
     Innovation:
</h2>
- Enemy states and patrol </br>
- Game pause </br>
- Jump particles</br>
- Coldown on projectiles </br>
- Particle effects </br>
- Checkpoints </br>
- Every 1000 points you gain an extra life (if you get 1000 points (an extra life) and die, </br>
you have to get to 2000 points for another extra life)

   <h2>So…
      </h2>

 <h4>Do you want to try out the game? Download it using this <a href="https://github.com/Ap011y0n/Nidhogg-Engine/releases/tag/v1.0">link</a>!</h4>
 <h4>For more information, check our <a href="https://github.com/Ap011y0n/Nidhogg-Engine">repository</a> in Github: </h4> </br>
     
<h2>
    Licence
</h2>

//MIT License

Copyright (c) [2019] [Pol de la Torre Solé and Albert Garcia Belerda]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.//

  </body>

