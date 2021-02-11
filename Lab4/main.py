import copy
from enum import Enum

class Color(Enum):
    WHITE = 0   # unvisited
    GREY = 1    # inprocess
    BLACK = 2   # processing completed

class Node:
    x = None                # coordinates of the node
    y = None
    color = Color.WHITE     # intial color of the node
    def __init__(self,x,y):
        self.x = x
        self.y = y
    def getCoordinates(self):   
        return (self.x,self.y)
    def getColor(self):
        return self.color
    def setColor(self,color):
        self.color = color
    def isVisited(self):
        return self.color != Color.WHITE
    def isProcessed(self):
        return self.color == Color.BLACK

# abstract class for node
class graphNode(Node):
    index = -1          # unique index which identifies the node ranges from [0,total number of nodes in graph)
    def __init__(self,x,y,index):
        self.x = x
        self.y = y
        self.index = index
    def getIndex(self):
        return self.index
    
class Graph:
    nodes = None            # list of nodes stored in order of their indices
    numNodes = 0            # total number of nodes
    distanceMatrix = []     # square matrix of size numNodes x numNodes 
    nearestNeighbour = []   # a hash that stores the nearest neighbours of a node
    def __init__(self,nodes,distanceMatrix):
        self.nodes = nodes
        self.distanceMatrix = distanceMatrix
        self.numNodes = len(nodes)
        # make the nearest neighbour list
        for i in range(self.numNodes):
            neighbours = []   
            for j in range(self.numNodes):
                neighbours.append((distanceMatrix[i][j],j))
            neighbours.sort()
            print(neighbours.pop(0))
            self.nearestNeighbour.append(copy.deepcopy(neighbours))
        print("nearest neighbour : \n", self.nearestNeighbour)
    def getNode(self,index):
        # return node at given index
        return self.nodes[index]
    def changeNodeColor(self,index,color):
        # change color of node at given index
        self.nodes[index].setColor(color)
    def makeAllNodesUnvisited(self):
        # mark all nodes unvisited
        for i in range(self.numNodes):
            self.nodes[i].setColor(Color.WHITE)
    def isVisited(self,index):
        # return bool value true if node is visited
        return self.nodes[index].isVisited()
    def isProcessed(self,index):
        # returns if the node is processed
        return self.nodes[index].isProcessed()
    def getNeighbours(self,index):
        return self.nearestNeighbour[index]
    def getUnvisitedNeighbours(self,index):
        unvisited = []
        for dist,i in self.nearestNeighbour[index]:
            if not self.isVisited(i):
                unvisited.append((dist,i))
        return unvisited

def readData():
    distanceType = input()
    numNodes = int(input())
    nodes = []
    for i in range(numNodes):
        line = input()
        (x,y) = line.split()
        n = graphNode(float(x),float(y),i)
        nodes.append(n)
    distMatrix = []
    for i in range(numNodes):
        line = input()
        row = line.split()
        row = list(map(float, row))
        distMatrix.append(row)
    return Graph(nodes,distMatrix)

# test
# graph = readData()
# print(graph.getNode(0).getCoordinates())
# print(graph.getNode(0).getColor(),graph.isVisited(0))
# graph.changeNodeColor(0,Color.GREY)
# print(graph.getNode(0).getColor(),graph.isVisited(0))
# print(graph.getNeighbours(1))
# print('-')
# print(graph.getUnvisitedNeighbours(1))
# graph.makeAllNodesUnvisited()
# print(graph.getNode(0).getColor(),graph.isVisited(0))