#!/usr/bin/python3
import subprocess
import sys

def gasket(p, q, r):
    subprocess.run(['bazel-bin/gasket', str(p.real), str(p.imag), str(q.real), str(q.imag), str(r.real), str(r.imag)])

if __name__ == "__main__":
    gasket(complex(float(sys.argv[1]), float(sys.argv[2])), 
            complex(float(sys.argv[3]), float(sys.argv[4])), 
            complex(float(sys.argv[5]), float(sys.argv[6])))
