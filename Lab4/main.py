class Node:
    x = None
    y = None
    def __init__(self,x,y):
        self.x = x
        self.y = y
    def getNode(self):
        return (self.x,self.y)

class Graph:
    nodes = None
    distanceMatrix = None
    def __init__(self,nodes,distanceMatrix):
        self.nodes = nodes
        self.distanceMatrix = distanceMatrix
    def getNeighbours(self,node):
        # return neighbours in descending order of distance
        neighbours = None
        return neighbours

def readData():
    distanceType = input()
    numNodes = int(input())
    nodes = []
    for i in range(numNodes):
        line = input()
        (x,y) = line.split()
        n = Node(float(x),float(y))
        nodes.append(n)
    distMatrix = []
    for i in range(numNodes):
        line = input()
        row = line.split()
        row = list(map(float, row))
        distMatrix.append(row)
    return Graph(nodes,distMatrix)

graph = readData()
print(graph.distanceMatrix)