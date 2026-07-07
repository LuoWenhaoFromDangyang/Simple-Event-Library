import os

with open(os.path.join(os.path.dirname(os.path.dirname(__file__)), 'common', 'ver_pure.txt')) as f:
    __version__ = f.read().strip()
__author__ = "只是一个普通的人偶"
__email__ = "3666569527@qq.com"
__description__ = "Simple Event Library"