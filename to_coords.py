import sys

alive = sys.argv[1]

grid = []
for line in sys.stdin:
    grid += [line]

y = 0
for l in grid:
    x = 0
    for cell in l:
        if cell == alive:
            print(f"{x}, {y}")
        x += 1
    y += 1
