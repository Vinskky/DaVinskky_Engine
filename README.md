# DaVinskky_Engine
C++ 3D Game engine created with learning purposes.

![ScreenShot](https://github.com/Vinskky/DaVinskky_Engine/blob/main/Docs/Screenshot_V1.PNG?raw=true)

It has been coded from scratch in C and C++ along with the following libraries:

- [Standard Template Library](<https://en.wikipedia.org/wiki/Standard_Template_Library>)
- [SDL v2.0.12](<https://www.libsdl.org/index.php>)
- [OpenGL v3.1](<https://www.opengl.org/>)
- [Glew v2.1.0](<http://glew.sourceforge.net/>)
- [ImGui v1.85 (Docking Branch)](<https://github.com/ocornut/imgui/tree/docking>)
- [Assimp v3.3.0](<http://www.assimp.org/index.php/downloads>)
- [DevIL v1.7.8](<http://openil.sourceforge.net/download.php>)
- [MatGeoLib v2.0](<https://github.com/juj/MathGeoLib>)
- [PhysFS v3.0.2](<https://icculus.org/physfs/>)
- [JSON for Modern C++ version 3.10.4 (Single Include)](https://github.com/nlohmann/json/tree/develop/single_include/nlohmann)

****

## Developed by: 
* Sebastián Delgado Vargas: Vinskky (https://github.com/Vinskky)
* Abraham David Diaz Garcia: Theran1 (https://github.com/Theran1)
* Yeray Tarifa Mateo: yeraytm (https://github.com/yeraytm)
* Repository: https://github.com/Vinskky/DaVinskky_Engine

****

## Overview
**Implemented features**

* Library Integrations.
* ImGui UI Display & Structure
* ImGui panels: Configuration, Console, Hierarchy & Inspector
* 3D Model Import
* Image Import
* Drag & Drop Models and textures from a directory (Specifically those inside the Assets folder).
* GameObject and Components structure
* You can Edit Values of GameObjects in the Inspector
* Use of Hierarchy: You are able to delete, reparent, create empty and create children of the current Game Object
* Use of Transform: translate, rotate and scale Game Objects.
* Camera: A master camera is created by deafault, you are able to create a camera different from the Master Camera, edit its transform and position values and enable and disable "show frustum" and "frustum culling". 
* Added the option to see an objects Bounding Box (AABB as well as OBB)
* Meshes are saved as our own format (.DavMesh for meshes) under “Library/Meshes” folder
* Serialization: You can Save And Load the Scene, It is Saved as a serialized Json File

**New**
* Shaders: The engine uses a shader pipeline to draw geometry. 
* The whole Scene is drawn with shaders by default.
* New shaders can be applied to materials with the editor. 
* New Shaders can be created.
* Editing a shader in-engine automatically compiles and applies the changes.
* Uniform values can be edited from the inspector.
* Internal text editor that ca show Logs and compilation errors from the shader in the engine console

**Engine Features**
* **Main Menu Toolbar:** Allows to show/hide some editor panels, go to URL links, and close the application.
* **Configuration Panel:** Allow us to modify some parameters from existing modules.
* **Hierarchy Panel:** Shows all GameObjects on the scene with the parenting working as an ImGui TreeNode.
* **Console Panel:** Help us to identify what is going on in the engine through messages.
* **Inspector Panel:** Once selected a GameObject on the Hierarchy panel, allow us to see if the GameObject selected has certain components. (For the moment is only visual)
* **Time Manage Panel:** To control the internal game timer

****

## About Use

### Installation

No previous installation is required. Download the latest [RELEASE](<https://github.com/Vinskky/DaVinskky_Engine/releases>) from the repository and execute the 
"DaVinskky Engine.exe" executable, which you will find in the "DaVinskky Engine" folder.

### Input Text Box
When writting text into a Text Box, you must have selected the box itself and press ENTER whenever you want to set that text. Once ENTER has been pressed, the corresponding text will be saved successfully.

#### Loading & Saving Scenes
Whenever you want to load or save an scene, just write the name of the .json file you want to set. The extension will be used properly by the engine.

### Camera Controls

#### MOUSE

* **Mouse Wheel:** Moves the camera forwards or backwards in its X axis.
* **Left Alt + Left Click**: Rotate around a reference point.

#### KEYBOARD

* **W Key:** Moves the camera forwards.
* **A Key:** Strafes the camera to the left.
* **S Key:** Moves the camera backwards.
* **D Key:** Strafes the camera to the right.
* **Q Key:** Moves the camera vertically.
* **E Key:** Moves the camera vertically.
* **SHIFT Key:** Increase the camera's movement speed.
