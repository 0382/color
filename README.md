# set console color with python

### install
```batch
git clone https://github.com/0382/color.git
cd color
python setup.py install --record installed.txt
```

### usgae
- example
```python
import color

color.printc("Hello world!", color='red')

@color.color('green')
def myprint(*args, **kw):
    print(*args)
    for k,v in kw.items():
        color.printc(f"{k}: {v}", color='gray;blue')

myprint('Hello', 'world!', name = 'Alice')
```