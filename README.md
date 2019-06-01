# set console color with python

## about this package

I want to have a very easy way to print colorful text in console. In this package, use can use string color name like `color="red"` to set the output text as red. However, as you know, parsing string is not an easy work. So, this package is not the fastest way to set console color.

## about color name

I want to make this package can be used with the same color name in both Windows and Linux. So, the color name of ANSI code follows the [wikipedia](https://en.wikipedia.org/wiki/ANSI_escape_code). However, it is different with Windows API, you can use command `color/?` to see Windows' color name. At last, both name are supported in this package.

For `bright red`, you should use `color="bred"`, and `light red` (Windows' name) is `"lred"`.

For background and foreground color, use one char of `";:,\. \t"` to split. (You can add split char in the source files before compile, if you like.) For example, `"blue;red"` means blue background and red foreground.

## install

```batch
git clone https://github.com/0382/color.git
cd color
python setup.py install --record installed.txt
```

## usgae

- example

```python
import color

@color.color('red')
def myprint(*args, **kw):
    print(*args)
    for k, v in kw.items():
        if(k == 'poor_man'):
            color.printc(f'{k}: {v}', color='bcyan;yellow')
        elif(k == 'python'):
            color.printc(f'{k}: {v}', color='23;160;250') # rgb can not work on windows
        else:
            color.printc(f'{k}: {v}')

if __name__ == '__main__':
    myprint('hello,',' world', poor_man = 'I like python', what = 'the fuck', python = 'I love c++')
```
