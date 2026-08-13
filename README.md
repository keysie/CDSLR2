# CDSLR² (Clever DSLR Remote) by Keysie

## Overview
<img width="629" height="288" alt="general-usecase drawio" src="https://github.com/user-attachments/assets/2c78cc79-4702-4864-ad29-00d1de3d10fb" />
  
Connect Sony DLSR via proprietary remote cable and USB (optional).  
Helps remote controlling the camera from a computer and moving pictures
off the camera between shots over USB.  
See https://github.com/keysie/CDSLR2 for pinout etc.   
  
Available commands:  
U1 : Enable USB host connection  
U0 : Disable USB host connection  
S1 : Enable shutter pin - force shutter open or cycle shutter  
S0 : Disable shutter pin - release shutter  
F1 : Enable focus pin - force autofocus if enabled on camera  
F0 : Disable focus pin - stop focusingTimer: Entering Sleep mode  
  
## Basic Proof of Concept (Windows only)
Camera must be set to Mass-Storage mode and shoot raw images. The path where Windows mounts the camera must be hard-coded, as well as the path inside the camera's file system where new images can be found. The code then retrieves the newest image from the camera and displays its raw contents (most likely results in false color).

