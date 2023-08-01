from PIL import Image
import numpy as numpy

class ImageLoader:
    def __init__(self, id, file_name):
        self.id = id
        self.file_name = file_name
        self.image = Image.open(self.file_name)
        self.width, self.height = self.image.size
        self.rgba_array = self.get_rgba()

    # Get an rgba array representation of the image
    def get_rgba(self):
        return np.asarray(self.image)

    # Get a string representation to store in the file
    def to_string(self):
        contents = str(self.id) + " "

        for pixel in self.rgba_array:
            contents += "(" + str(pixel[0]) + "," + str(pixel[1]) + "," + str(pixel[2]) + "," + str(pixel[3]) + "),"

        return contents
