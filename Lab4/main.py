import copy
import math
import time
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
    def getMaxPossibleTourLength(self):
        maximum = max([max(row) for row in self.distanceMatrix])
        return maximum*self.numNodes
    def getHeuristicCost(self,path):
        cost = 0
        for i in range(len(path)-1):
            cost = cost + self.getDistance(path[i],path[i+1])
        cost = cost + self.getDistance(path[len(path)-1],path[0])
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
    percentage = [(f/totalFitness) for f in fitness]
    survivingPopulation = []
    print('Fitness : ',fitness)
    print('percentage : ',percentage)
    # take 1 : convert percentage to integers directly
    # rouletteSelection = [math.floor(p*n) for p in percentage]
    # for i in range(n):
    #     for _ in range(rouletteSelection[i]):
    #         survivingPopulation.append(population[i])
    # 
    # take 2 : find max, append, reduce the percentage
    # for i in range(n):
    #     maxProbGene = max(percentage)
    #     idx = percentage.index(maxProbGene)
    #     while(percentage[idx]>=0.0):
    #         percentage[idx] -= (1/n)
    #         survivingPopulation.append(population[idx])
    #         if(len(survivingPopulation)>=n):
    #             return survivingPopulation
    #     print('Gene number : ',idx,' added')
    #
    # take 3 : actual roulette wheel
    for i in range(n):
        ball = random.random()
        value = 0
        for j in range(n):
            value += percentage[j]
            if(ball<value):
                survivingPopulation.append(population[j])
                # print('added gene ',j)
                break
    if(len(survivingPopulation) is not n):
        # print('ERROR : new population size is not expected! required ',n,' recieved ',len(survivingPopulation))
        exit()
    return survivingPopulation


def orderedCrossover(population):
    n = len(population[0])
    getRange = random.sample(range(1,n),2)
    getRange.sort()
    start = getRange[0]
    stop = getRange[1]
    # print("slice index : ",start,stop)
    newPopulation = []
    for gene in population:
        getRange = random.sample(range(1,n),2)
        getRange.sort()
        start = getRange[0]
        stop = getRange[1]
        newGene = gene[start:stop+1] + gene[:start] + gene[stop+1:]
        newPopulation.append(copy.copy(newGene))
    return newPopulation

def testCrossover(population):
    n = len(population[0])
    pairsList = list(range(0,len(population)))
    random.shuffle(pairsList)
    for i in range(0,len(pairsList),2):
        start = random.randint(0,int(n-(n/10)))
        stop = start + int(n/20)
        transplant1 = population[i][start:stop+1]
        transplant2 = population[i+1][start:stop+1]
        for t in transplant1:
            population[i+1].remove(t)
        population[i+1] += transplant1
        for t in transplant2:
            population[i].remove(t)
        population[i] += transplant2
    return population

def rePopulate(population, reqSize):
    n = len(population[0])
    currPopulation = copy.copy(population)
    while(len(currPopulation)!=reqSize):
        parents = random.sample(copy.deepcopy(population),2)
        start = random.randint(0,int(n/4))
        stop = start + int(n/2) + random.randint(0,int(n/4))-1
        transplant1 = parents[0][start:stop+1]
        for t in transplant1:
            parents[1].remove(t)
        offspring = parents[1] + transplant1
        currPopulation.append(offspring)
        # print('offspring : ',len(offspring),' currPoplation size : ',len(currPopulation))
    print('repopulation compplete!')
    return currPopulation

class GeneticAlgorithm:
    populationSize = -1
    population = []
    graph = None
    selectionFunction = None
    fitnessFunction = None
    crossoverFunction = None
    def __init__(self,graph,populationSize=6,selectionFunction=rouletteWheel,fitnessFunction=TSPfitness,crossoverFunction=testCrossover):
        self.populationSize = populationSize
        self.graph = graph
        self.selectionFunction = selectionFunction
        self.fitnessFunction = fitnessFunction
        self.crossoverFunction = crossoverFunction 
        tour = [i for i in range(0,self.graph.numNodes)]
        print("test 1")
        for _ in range(self.populationSize):
            random.shuffle(tour)
            self.population.append(copy.deepcopy(tour))
    def fitness(self,gene):
        return self.graph.getHeuristicCost(gene)
    def tourLength(self,gene):
        return self.graph.getHeuristicCost(gene)
    def selection(self):
        n = self.populationSize
        temp = []
        for gene in self.population:
            temp.append((self.fitness(gene),gene))
        temp.sort()
        newPopulation = []
        for i in range(int(n/5)):
            # print(temp[i][0])
            newPopulation.append(temp[i][1])
        self.population = newPopulation
        # newPopulation = []
        # for i in range(int(n/2)):
        #     gene = self.getBestGene()
        #     self.population.remove(gene)
        #     newPopulation.append(gene)
        # self.population = newPopulation
        print("new population size after selection : ",len(self.population))
        # for gene in self.population:
        #     print(gene)
    def crossover(self):
        # print("population before corssover : ",self.population)
        self.population = rePopulate(self.population,self.populationSize)
        # print("population after crossover : ",self.population)
    def mutation(self):
        for i in range(len(self.population)):
            prob = random.randint(0,9)
            if(prob>8):
                for _ in range(random.randint(1,5)):
                    positions = random.sample(range(0,len(self.population[i])),2)
                    p1 = positions[0]
                    p2 = positions[1]
                    temp = self.population[i][p1]
                    self.population[i][p1] = self.population[i][p2]
                    self.population[i][p2] = temp
    def evolve(self):
        # hashirama = self.getBestGene()
        # print("\n Best population before selection : \n",hashirama,"\nfitness of : ",self.fitness(hashirama))
        
        # selection process
        self.selection()
        # hashirama = self.getBestGene()
        # print("\n Best population after selection : \n",hashirama,"\nfitness of : ",self.fitness(hashirama))

        # crossover 
        self.crossover()
        # hashirama = self.getBestGene()
        # print("\n Best population after crossover : \n",hashirama,"\nfitness of : ",self.fitness(hashirama))

        # mutation 
        self.mutation()
        hashirama = self.getBestGene()

        return hashirama
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
AI = GeneticAlgorithm(graph,50)
timeout = time.time() + 250
bestTour = None
genNumber = 0
stopingCriteria = 1000
while ((time.time() < timeout) and (stopingCriteria > 0)):
    genNumber += 1
    # stopingCriteria -= 1
    currGenBestTour = AI.evolve()
    if(bestTour == None):
        bestTour=currGenBestTour
    if(AI.tourLength(bestTour)>AI.tourLength(currGenBestTour)):
        stopingCriteria = 1000
        bestTour = currGenBestTour
        print("New best tour : ",AI.tourLength(bestTour)," (generation ",genNumber,")")
        buffer = ""
        for city in bestTour:
            buffer = buffer + str(city) + " "
        print(buffer)

if(stopingCriteria < 0):
    print("Evolution stopped!")
print("New best tour : ",AI.tourLength(bestTour)," (generation ",genNumber,")")
buffer = ""
for city in bestTour:
    buffer = buffer + str(city) + " "
print(buffer)
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
