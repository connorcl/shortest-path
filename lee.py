
class queue:
    
    def __init__(self):
        self.q = []
    
    def enqueue(self, x):
        self.q.append(x)
        
    def pop(self):
        i = self.q[0]
        del(self.q[0])
        return i
    
    def get_q(self):
        return self.q

A = []

for i in range(20):
    A.append([])

for i in range(20):
    for j in range(20):
        A[i].append([i, j, 0])
        
A[12][2][2] = -1
A[3][4][2] = -2
        
def get_adj_coords(x, y):
    
    l = []
    
    if y+1 in range(20):
        l.append([x, y+1])
    if y-1 in range(20):
        l.append([x, y-1])
    if x+1 in range(20):
        l.append([x+1, y])
    if x-1 in range(20):
        l.append([x-1, y])
    
    return l

def print_matrix():
    for i in range(len(A)):
        for j in range(len(A[0])):
            el = A[i][j][2]
            if el in range(10):
                el = " " + str(el)
            else:
                el = str(el)
            print(el + " ", end='')
        print('\n')

start = [12, 2]
end = [3, 4]

q = queue()

for i in get_adj_coords(*start):
    x, y = i
    A[x][y][2] = 1
    q.enqueue(A[x][y])
    
done = False
while not done:
    next_cell = q.pop()
    for i in get_adj_coords(next_cell[0], next_cell[1]):
        x, y = i
        if A[x][y][2] == -2:
            done = True
        elif A[x][y][2] == 0:
            A[x][y][2] = next_cell[2] + 1
            q.enqueue(A[x][y])
            print(A[x][y])
            
next_coords = end
done = False
while not done:
    min_dist = 1000
    coords_min = []
    for i in get_adj_coords(next_coords[0], next_coords[1]):
        x, y = i
        if A[x][y][2] == -1:
            print("Done")
            done = True
        elif A[x][y][2] < min_dist and A[x][y][2] > 0:
            min_dist = A[x][y][2]
            coords_min = [x, y]
    if not done:
        next_coords = coords_min
        print(next_coords)
        A[next_coords[0]][next_coords[1]][2] = -3
    
        
    
        


