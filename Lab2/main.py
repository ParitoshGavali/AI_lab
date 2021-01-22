import random
import copy
import sys
import csv
from queue import PriorityQueue

# Basic codes

def printState(state):
    for s in state:
        print(s)

def initGame(stacks,blocks):
    
    sampleSpace = list(range(1,blocks+1))
    # print('Sample space : ',sampleSpace)
    
    initState = [[] for _ in range(stacks)]
    random.shuffle(sampleSpace)
    for s in sampleSpace:
        initState[random.randint(0,stacks-1)].append(s)
    
    goalState = [[] for _ in range(stacks)]
    random.shuffle(sampleSpace)
    for s in sampleSpace:
        goalState[random.randint(0,stacks-1)].append(s)
    
    # print('\nInitial State : ')
    # printState(initState)
    # print('\nGoal State : ')
    # printState(goalState)
    return initState,goalState

# Find new states
def generateNewStates(state):
    numStack = len(state)
    newStates = []
    tempState = []
    for i in range(numStack):
        if (len(state[i])>=1):
            for j in range(numStack):
                if i!=j :
                    tempState = copy.deepcopy(state)
                    n = tempState[i].pop()
                    tempState[j].append(n)
                    newStates.append(tempState)
    return newStates

# check for goal
def goalTest(curr,goal):
    return curr==goal


# Heuristics
def heuristic_manhattan(state, goal):
    cost=0
    for state_row_index,i in enumerate(state):
        for state_index,j in enumerate(i):
            for goal_row_index,k in enumerate(goal):
                for goal_index,l in enumerate(k):
                    if(j==l):
                        if(state_index==goal_index and state_row_index==goal_row_index):
                            cost = cost - (len(i) - state_index - 1)                            
                        elif(state_row_index != goal_row_index):
                            tempcost = len(i) - state_index + len(k) - goal_index - 1
                            cost=cost + tempcost
                        else:
                            cost = cost + abs(state_index-goal_index)
    return cost

def naiveHelper(list1,list2):
    n = min(len(list1),len(list2))
    score = 0
    for i in range(n):
        if list1[i]==list2[i]:
            score = score + 1
    return score    
    
def naiveMatching(state,goal):
    score = 0
    for i in range(len(state)):
        score += naiveHelper(state[i],goal[i])
    return -score

def weightedHelper(list1,list2):
    n = min(len(list1),len(list2))
    score = 0
    for i in range(n):
        if list1[i]==list2[i]:
            score = score + len(list1) - i
    return score

def weightedMatching(state,goal):
    score = 0
    for i in range(len(state)):
        score += weightedHelper(state[i],goal[i])
    return -score
            
            

# AI algorithms

# BFS
def BFS(start,goal,func):
    visitedStates = []
    pq = PriorityQueue()
    count = 1
    pq.put((func(start,goal),start))
    visitedStates.append(start)
    while(pq):
        v = pq.get()
        v = v[1]
        count += 1
        print('\n',count,func(v,goal))
        printState(v)
        if goalTest(v,goal):
            print('\nCompleted!!')
            print('Number of iterations : ',count)
            print('Heuristic value : ',func(v,goal))
            return True,count
        nextStates = generateNewStates(v)
        for u in nextStates:
            if u not in visitedStates:
                pq.put((func(u,goal),u))
                visitedStates.append(u)
    return False,count

# Hill Climb Racing
def HC(start,goal,func):
    visitedStates = []
    visitedStates.append(start)
    currState = start
    value = func(start,goal)
    printState(currState)
    count = 1
    while(not goalTest(currState,goal)):
        nextStates = generateNewStates(currState)
        flag = False
        for u in nextStates:
            if u not in visitedStates:
                if func(u,goal) <= value:
                    flag = True
                    value = func(u,goal)
                    currState = u
        visitedStates.append(currState)
        print('\n',count,func(currState,goal))
        printState(currState)
        count += 1
        if not flag:
            print('\nStuck in local maxima')
            print('Number of iterations : ',count)
            print('Heuristic value : ',func(currState,goal))
            return False,count
    print('\nCompleted!!')
    print('Number of iterations : ',count)
    print('Heuristic value : ',func(currState,goal))
    return True,count


# Main program

# Initialisation of the domain 
rows = int(sys.argv[1])
numberOfBlocks = int(sys.argv[2])
startState,goalState = initGame(rows,numberOfBlocks)
print('Initialising...')
print('\nStart State : ')
printState(startState)
print('\nGoal State : ')
printState(goalState)
# print('h : ',heuristic_manhattan(startState,goalState))
# print('n : ',naiveMatching(startState,goalState))
# print('n : ',weightedMatching(startState,goalState))

line = str(rows) + ',' + str(numberOfBlocks)

print('\n######################\nBFS Manhattan')
status,itr = BFS(startState,goalState,heuristic_manhattan)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\n BFS Naive')
status,itr = BFS(startState,goalState,naiveMatching)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\n BFS Weighted Naive')
status,itr = BFS(startState,goalState,weightedMatching)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\nHC Manhattan')
status,itr = HC(startState,goalState,heuristic_manhattan)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\n HC Naive')
status,itr = HC(startState,goalState,naiveMatching)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\n HC Weighted Naive')
status,itr = HC(startState,goalState,weightedMatching)
line = line + ',' + str(status) + ',' + str(itr) 

print('\n######################\nStart State : ')
printState(startState)
print('\nGoal State : ')
printState(goalState)
print(line)

## write in csv
f = open('data.csv', 'a')
f.write('\n' + line)
f.close()