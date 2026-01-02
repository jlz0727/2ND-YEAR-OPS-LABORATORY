import os
import subprocess
import sys

pid = 1

try:
    p = subprocess.Popen(["whoami"])
except:
    pid = -1

if pid < 0:
    print("fork failed")
    sys.exit(1)
elif pid == 0:
    sys.exit(0)
else:
    print("\n Process ID is: %d\n" % os.getpid())
    p.wait()
    sys.exit(0)
