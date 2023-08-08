# This is a python wrapper for my bmp library

import ctypes as ctp

# logging
DEBUG = 1
if DEBUG:
    log = print
else:
    def log(*args, **kwargs) -> None:
        pass


# the color type
class Color:
    def __init__(self, red: int, green: int, blue: int) -> None:
        self.red = red
        self.green = green
        self.blue = blue


class C_color_t(ctp.Structure):
    _fields_ = [
        ("blue", ctp.c_uint8),
        ("green", ctp.c_uint8),
        ("red", ctp.c_uint8)
    ]


class Point:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y


class C_point_t(ctp.Structure):
    _fields_ = [
        ("x", ctp.c_uint32),
        ("y", ctp.c_uint32)
    ]


# the C library API
SO_FILE = "./libbmap.so"
libbmap_c = ctp.CDLL(SO_FILE)
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

# the color black
black = Color(0, 0, 0)

class Image:
    def __init__(self, x: int, y: int, color: Color = black) -> None:
        """ constructor """
        # TODO: color initialization
        self.image_p = libbmap_c.bmp_image_ctor(
            ctp.c_uint32(x), 
            ctp.c_uint32(y)
        )
        log("Constructed image at", hex(self.image_p))

    def draw_rectangle(self, a: Point, b: Point, col: Color):
        """ Draws a rectangle from `a` to `b` of color `col` """
        log(f"drawing rectangle from ({a.x}, {a.y}) to ({b.x}, {b.y}) "
             "to image at {self.image_p}")
        libbmap_c.bmp_draw_rect(
            C_point_t(a.x, a.y),
            C_point_t(b.x, b.y),
            C_color_t(col.blue, col.green, col.red),
            self.image_p
        )

    def save_bmp(self, filename: str):
        """ Saves the image as a .bmp file """
        log(f"saving \'{filename}\'")
        libbmap_c.bmp_save(filename.encode("utf-8"), self.image_p)\

    def __del__(self) -> None:
        """ destructor """
        log("Destructing image at", hex(self.image_p))
        libbmap_c.bmp_image_dtor(self.image_p)


def main() -> None:
    """ This is a demo usage of the Image, Point and Color classes """
    a = Image(100, 200)
    a.draw_rectangle(Point(50, 50), Point(70, 80), Color(255, 0, 0))
    a.save_bmp("obrazecek_druhy.bmp")


if __name__ == "__main__":
    main()