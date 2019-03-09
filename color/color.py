import ctypes
import sys
import _color

FOREGROUND_BLUE       = 0x0001
FOREGROUND_GREEN      = 0x0002
FOREGROUND_RED        = 0x0004
FOREGROUND_INTENSITY  = 0x0008
BACKGROUND_BLUE       = 0x0010
BACKGROUND_GREEN      = 0x0020
BACKGROUND_RED        = 0x0040
BACKGROUND_INTENSITY  = 0x0080

_default_color = _color.get_current_color()

def set_console_color(color):
    '''
    set_console_color(color)
    parameter: int or str
        int: windows api code
        str: windows command or normal word
    return: bool
        set successful(True) or not(False)
    '''
    if color is None: return False
    elif isinstance(color, int):
        return _color.setcolor(color)
    elif isinstance(color, str):
        return _color.set_color_with_words(color)
    else:
        return False

def reset_color():
    '''
    reset color as default color
    '''
    _color.setcolor(_default_color)

def printc(*value, color=None, sep=' ', end='\n', file=sys.stdout, flush=False):
    '''
    print something with colorful output,
    with the same args as `print` function,
    and add args `color`
    
    example:
    >>>color.printc("Hello world!", color='red')
    '''
    if(file != sys.stdout): color = None
    now_color = _color.get_current_color()
    set_console_color(color)
    print(*value, sep = sep, end = end, file = file, flush = flush)
    _color.setcolor(now_color)

def color(color):
    '''
    decorator for function witch has output to sys.stdout
    
    example:
    >>>@color.color('red')
    ...def myprint(*message):
    ...    print(*message)
    then every time using myprint, you'll get red output
    '''
    def decorator(func):
        def anyfunc(*args, **kw):
            now_color = _color.get_current_color()
            set_console_color(color)
            r = func(*args, **kw)
            _color.setcolor(now_color)
            return r
        return anyfunc
    return decorator

if __name__ == '__main__':
    pass