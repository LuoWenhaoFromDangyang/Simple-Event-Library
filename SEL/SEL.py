dynamic_import = (lambda: [
    (lambda: [
        path := __import__('os').path.join(__import__('os').path.abspath(__import__('os').path.dirname(__file__)), 'res', 'py', 'main.py'),
        code := compile(open(path).read(), path, "exec"),
        exec(code, (lambda: [
            globals().update({'__file__':path,'__name__':'__main__'}),
            globals().pop('path',0),
            globals().pop('code',0),
            globals()
        ])()[-1])
    ]),
    lambda: None
])()[-1]
from res.py.main import *
