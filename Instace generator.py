import random

file = open('source.txt', 'w')

print("Provide number of points to make:")
N = int(input())
points = []

file.writelines([str(N), '\n'])

for i in range (0, N):
    x = random.randint(0, 500)
    y = random.randint(0, 500)
    
    while (x,y) in points:
        x = random.randint(0, 500)
        y = random.randint(0, 500)
        
    file.writelines([str(i+1), '\t', str(x), '\t', str(y), '\n'])
    points.append((x,y))


file.close()

print("Success")
