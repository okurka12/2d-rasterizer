# This is a python wrapper for my bmp library

import ctypes as ctp
import platform as plt


# path to the C dynamic library (leave empty string to select predefined path)
SO_FILE = ""

DEBUG = 1  # Do we want debug print statements?

# logging
if DEBUG:
    log = print
else:
    def log(*args, **kwargs) -> None:
        pass

if SO_FILE == "":
    if plt.system() == "Linux":
        so_file_path = "./libbmap.so"
    elif plt.system() == "Windows":
        so_file_path = ".\\libbmap.dll"
    else:
        raise RuntimeError(
            "Don't know where to look for the library on this system\n"
            f"Perhaps you can set the SO_FILE variable in {__file__}"
        )

else:
    so_file_path = SO_FILE


class Color:
    """ an RGB color """
    def __init__(self, red: int, green: int, blue: int) -> None:
        self.red = red
        self.green = green
        self.blue = blue

# common colors
COLORS = {
    "black": Color(0, 0, 0),
    "white": Color(255, 255, 255)
}


class C_color_t(ctp.Structure):
    """ C representation of class Color """
    _fields_ = [
        ("blue", ctp.c_uint8),
        ("green", ctp.c_uint8),
        ("red", ctp.c_uint8)
    ]


class Point:
    """ A 2D point """
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y


class C_point_t(ctp.Structure):
    """ C representation of class Point """
    _fields_ = [
        ("x", ctp.c_uint32),
        ("y", ctp.c_uint32)
    ]


# the C library API
libbmap_c = ctp.CDLL(so_file_path)
libbmap_c.bmp_image_ctor.argtypes = ctp.c_uint32, ctp.c_uint32
libbmap_c.bmp_image_ctor.restype  = ctp.c_void_p
libbmap_c.bmp_image_dtor.argtypes = ctp.c_void_p,
libbmap_c.bmp_draw_rect.argtypes = (
    C_point_t, 
    C_point_t, 
    C_color_t, 
    ctp.c_void_p
)
libbmap_c.bmp_save.argtypes = ctp.c_char_p, ctp.c_void_p
libbmap_c.bmp_save.restype = ctp.c_int

class Image:
    def __init__(self, x: int, y: int, color: Color = COLORS["black"]) -> None:
        """ constructor of image `x` by `y` 
            initialised to `color` (black by default) """
        
        # image construction
        self.image_p = libbmap_c.bmp_image_ctor(
            ctp.c_uint32(x), 
            ctp.c_uint32(y)
        )

        # null check
        if self.image_p == None:
            raise RuntimeError("Couldn't allocate memory")
        
        log("Constructed image at", hex(self.image_p))

        # color initialisation
        self.draw_rectangle(Point(0, 0), Point(x - 1, y - 1), color)

    def draw_rectangle(self, a: Point, b: Point, col: Color):
        """ Draws a rectangle from `a` to `b` of color `col` """
        log(f"drawing rectangle from ({a.x}, {a.y}) to ({b.x}, {b.y}) "
            f"to image at {hex(self.image_p)}")
        libbmap_c.bmp_draw_rect(
            C_point_t(a.x, a.y),
            C_point_t(b.x, b.y),
            C_color_t(col.blue, col.green, col.red),
            self.image_p
        )

    def save_bmp(self, filename: str) -> None:
        """ Saves the image as a .bmp file """
        log(f"saving \'{filename}\'")
        rcode = libbmap_c.bmp_save(filename.encode("utf-8"), self.image_p)
        if rcode != 0:
            raise RuntimeError(f"Couldnt save {filename}.")

    def __del__(self) -> None:
        """ destructor """
        log("Destructing image at", hex(self.image_p))
        libbmap_c.bmp_image_dtor(self.image_p)


def main() -> None:
    """ This is a demo usage of the Image, Point and Color classes """
    a = Image(100, 200)
    a.draw_rectangle(Point(50, 50), Point(70, 80), Color(255, 0, 0))
    a.save_bmp("python_example.bmp")


if __name__ == "__main__":
    main()