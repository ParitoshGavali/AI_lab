import copy
import math
import random
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
            # print(neighbours.pop(0))
            self.nearestNeighbour.append(copy.deepcopy(neighbours))
        # print("nearest neighbour : \n", self.nearestNeighbour)
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
    def getDistance(self,index1,index2):
        return self.distanceMatrix[index1][index2]
    def getHeuristicCost(self,path):
        cost = 0
        for i in range(len(path)-1):
            cost = cost + self.getDistance(path[i],path[i+1])
        cost = cost + self.getDistance(path[0],path[len(path)-1])
        return cost

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

def TSPfitness(gene,graph):
    return graph.getHeuristicCost(gene)

def rouletteWheel(population,fitness):
    fitness = [fitness(gene) for gene in population]
    n = len(population)
    totalFitness = sum(fitness)
    rouletteSelection = [math.floor((f/totalFitness)*n) for f in fitness]
    survivingPopulation = []
    for i in range(n):
        for _ in range(rouletteSelection[i]):
            survivingPopulation.append(population[i])
    print('Fitness : ',fitness)
    print('roulette wheel : ',rouletteSelection)
    if(len(survivingPopulation) is not n):
        print('ERROR : new population size is not expected! required ',n,' recieved ',len(survivingPopulation))
        # print('Population : ',population)
        
        # print('next Population : ',survivingPopulation)
        exit()
    return survivingPopulation

class GeneticAlgorithm:
    populationSize = -1
    population = []
    graph = None
    selectionFunction = None
    fitnessFunction = None
    def __init__(self,graph,populationSize=6,selectionFunction=rouletteWheel,fitnessFunction=TSPfitness):
        self.populationSize = populationSize
        self.graph = graph
        self.selectionFunction = selectionFunction
        self.fitnessFunction = fitnessFunction
        tour = [i for i in range(0,self.graph.numNodes)]
        print("test 1")
        for _ in range(self.populationSize):
            random.shuffle(tour)
            self.population.append(copy.deepcopy(tour))
    def fitness(self,gene):
        return (self.fitnessFunction(gene,self.graph)/10000)**4
    def selection(self):
        return self.selectionFunction(self.population,self.fitness)
    # def crossover(self):
        #write code for crossover to get new generation
    # def mutation(self):
        #write code for mutation
    def evolve(self):
        newPopulation = self.selection()
        # print("Old population : ",self.population)
        oldMadara = self.getBestGene()
        print("\nBest of old population : \n",oldMadara,"\nfitness of : ",self.fitness(oldMadara))
        self.population = newPopulation
        newMadara = self.getBestGene()
        print("\nNew population : \n",newMadara,"\nfitness of : ",self.fitness(newMadara))
        # self.crossover()
        # self.mutation()
    def isBetterGene(self,g1,g2):
        return self.fitness(g1)<self.fitness(g2)
    def getBestGene(self):
        bestGene = None
        for gene in self.population:
            if bestGene == None:
                bestGene = gene
            elif self.isBetterGene(gene,bestGene):
                bestGene = gene
        return bestGene


print("test 0")
graph = readData()
print("test after data 0")
AI = GeneticAlgorithm(graph)
AI.evolve()

# graph = readData()
# tour = [i for i in range(0,graph.numNodes)]
# bestTourCost = graph.getHeuristicCost(tour)
# print("best tour : ",bestTourCost)
# buffer = ""
# for city in tour:
#     buffer = buffer + str(city) + " "
# print(buffer)
# for i in range(200):
#     random.shuffle(tour)
#     tourCost = graph.getHeuristicCost(tour)
#     if(bestTourCost > tourCost):
#         bestTourCost = tourCost
#         print("best tour cost:",bestTourCost)
#         buffer = ""
#         for city in tour:
#             buffer = buffer + str(city) + " "
#         print(buffer)
