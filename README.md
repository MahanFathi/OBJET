
# OBJET: A Graphics Playground for Computer Vision
OBJET, is a graphical environment, developed in OpenGL and accessible in Python, to cater to the data needs of one willing to conduct research on computer vision in such domains. 

<img align="right" width="200" height="200" src="./resources/logo/OBJET.png">
Load your own objects in a room, take a snapshot of the viewport, from the view point of the customizable camera, and load it to python. 
This gives you the ability to creat data on-the-fly, in your python data loader. 
Check out the footnoted video by Ali Eslami et al. to see such environment in practice.

## Synopsis

### Dependencies
On Arch Linux:
```
sudo pacman -S rapidjson mesa glfw-x11 glew glm assimp swig freeimage
```

### Usage

#### Build
```bash 
make python
```

#### get_image
<img align="right" width="200" height="200" src="./resources/images/ps_meta.png" title="ps_meta room"> <img align="right" width="200" height="200" src="./resources/images/meta.png" title="meta room">
```python
# for now
from objet import Objet
objet = Objet("./configs/ps_meta.json")
objet.set_object_position("cross", [3., 0., 3.])
objet.draw()
image = objet.get_image()
objet.to_image("./output.png")
```

#### get_depth_map
<img align="right" width="200" height="200" src="./resources/images/depth_ps_meta.png" title="ps_meta room"> <img align="right" width="200" height="200" src="./resources/images/depth_meta.png" title="meta room">
```python
# for now
depth_map = objet.get_depth_map()
# for demonstration
from PIL import Image
img = Image.fromarray(depth_map * 10)
img.show()
```



##### What does "OBJET" mean?
Objet is French for "object."

##### Are you French?
No, I used Google Translate.

##### Why C++/SWIG and not PyOpenGL?
* Data Scientists should learn to install dependencies on their own — life ain't always `pip install package`.
* PyOpenGL is an overkill by nature. 
* Might be interested in linking the data directly to CUDA later on.
* This work has been done over a COVID-19 quarantine, I had to kill some time.

###### Ali Eslami et al.: www.youtube.com/watch?v=G-kWNQJ4idw
