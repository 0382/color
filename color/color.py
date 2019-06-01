import sys
import functools
from . import _color


_default_color = _color.get_current_color()

def set_console_color(color):
    if color is None: return False
    if isinstance(color, str) or isinstance(color, int):
        _color.setcolor(color)
    else:
        return False

def reset_color():
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
        @functools.wraps(func)
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