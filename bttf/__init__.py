import sys as _sys
if _sys.version_info >= (3,):
    from ._bttf import *
else:
    def install(arg):
        pass


