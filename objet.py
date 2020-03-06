from OBJET import OBJET

import numpy as np

class Objet(object):
    """OBJET"""

    def __init__(self, path_to_meta_json, width=500, height=500):
        self._OBJET = OBJET(path_to_meta_json, width, height)

        self.width = width
        self.height = height

    def draw(self, ):
        self._OBJET.Draw()

    def get_image(self, ):
        img = np.array(self._OBJET.GetImage())
        img = img.reshape([self.height, self.width, -1])
        return np.flip(img, axis=0)

    def to_image(self, path_to_image):
        self._OBJET.ToImage(path_to_image)

    def set_objetct_position(self, object_name, position):
        self._OBJET.SetObjectPosition(object_name, position)

    def set_objetct_y_rotation(self, object_name, y_rotation):
        self._OBJET.SetObjectYRotation(object_name, y_rotation)

    def set_objetct_scale(self, object_name, scale):
        self._OBJET.SetObjectScale(object_name, scale)
