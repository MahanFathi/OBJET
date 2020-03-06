
# (WIP) OBJET: A Graphics Playground for Computer Vision
<img align="right" width="200" height="200" src="./resources/logo/OBJET.png">
Load your own objects in a room, take a snapshot of the viewport, from the view point of the customizable camera, and load it to python. This gives you the ability to creat data on-the-fly, in your python data loader. Check out the footnoted video by Ali Eslami et al. to see such environment in practice.

## Dependencies
On Arch Linux:
```
sudo pacman -S mesa glfw-x11 glew assimp swig freeimage
```

## Usage
```bash 
make python
```
```python
# for now
from objet import Objet
objet = Objet("./configs/meta.json")
objet.set_object_position("cross", [3., 0., 3.])
objet.draw()
image = objet.get_image()
objet.to_image("./output.png")
```

###### Ali Eslami et al.: www.youtube.com/watch?v=G-kWNQJ4idw
