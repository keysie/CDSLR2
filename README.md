# CDSLR2

## Basic Proof of Concept (Windows only)
Camera must be set to Mass-Storage mode and shoot raw images. The path where Windows mounts the camera must be hard-coded, as well as the path inside the camera's file system where new images can be found. The code then retrieves the newest image from the camera and displays its raw contents (most likely results in false color).