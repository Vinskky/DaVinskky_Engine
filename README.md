# DaVinskky_Engine
C++ 3D Game engine created with learning purposes.

![ScreenShot](https://github.com/Vinskky/DaVinskky_Engine/blob/main/Docs/Screenshot_V1.PNG?raw=true)

It has been coded from scratch in C and C++ along with the following libraries:

- [Standard Template Library](<https://en.wikipedia.org/wiki/Standard_Template_Library>)
- [OpenGL v4.6](<https://www.opengl.org/>)
- [Glew v2.1.0](<http://glew.sourceforge.net/>)
- [ImGui v1.83 (Docking Branch)](<https://github.com/ocornut/imgui/tree/docking>)
- [MatGeoLib v1.5](<https://github.com/juj/MathGeoLib>)
- [PhysFS v3.0.2](<https://icculus.org/physfs/>)
- [Parson v1.1.0](<https://github.com/kgabis/parson>)
- [Assimp v3.1.1](<http://www.assimp.org/index.php/downloads>)
- [DevIL v1.8.0](<http://openil.sourceforge.net/download.php>)


****

## Developed by: Sebastián Delgado Vargas

* Github: Vinskky (https://github.com/Vinskky)
* Repository: https://github.com/Vinskky/DaVinskky_Engine

**Implemented features**

* Library Integrations.
* ImGui UI Display & Structure
* ImGui panels: Configuration, Console, Hierarchy & Inspector
* 3D Model Import
* Image Import
* Drag & Drop Models and textures from a directory (Specifically those inside the Assets folder).
* GameObject and Components structure

****

### Engine Features

* **Main Menu Toolbar:** Allows to show/hide some editor panels, go to URL links, and close the application.
* **Configuration Panel:** Allow us to modify some parameters from existing modules.
* **Hierarchy Panel:** Shows all GameObjects on the scene with the parenting working as an ImGui TreeNode.
* **Console Panel:** Help us to identify what is going on in the engine through messages.
* **Inspector Panel:** Once selected a GameObject on the Hierarchy panel, allow us to see if the GameObject selected has certain components. (For the moment is only visual)


****

## About Use

### Installation

No previous installation is required. Download the latest [RELEASE](<https://github.com/Vinskky/DaVinskky_Engine/releases>) from the repository and execute the 
"DaVinskky Engine.exe" executable, which you will find in the "DaVinskky Engine" folder.



### Camera Controls

#### **MOUSE**

* **Mouse Wheel:** Moves the camera forwards or backwards in its X axis.
* **Left Alt + Left Click**: Rotate around a reference point.

**KEYBOARD**

* **W Key:** Moves the camera forwards.
* **A Key:** Strafes the camera to the left.
* **S Key:** Moves the camera backwards.
* **D Key:** Strafes the camera to the right.
* **Q Key:** Moves the camera vertically.
* **E Key:** Moves the camera vertically.
* **SHIFT Key:** Increase the camera's movement speed.


