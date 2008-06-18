import os

print " Test: small complete graphs ".center(80, '=')
print

test_params = ["0 1 4 80", "0 2 4 80", "-1 4 4 80", "-1 4 6 80", "-10 30 6 80", "-30 10 6 80"]

for params in test_params:
    if os.system("./test " + params) != 0:
        print
        print " FAILED ".center(80, "=")
        raise SystemExit, 1

print
print " PASSED ".center(80, "=")
