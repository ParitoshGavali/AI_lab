import random
import copy
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
            print('completed!')
            return True
        nextStates = generateNewStates(v)
        for u in nextStates:
            if u not in visitedStates:
                pq.put((func(u,goal),u))
                visitedStates.append(u)
    return False

# Hill Climb Racing
def HC(start,goal,func):
    visitedStates = []
    visitedStates.append(start)
    currState = start
    value = func(start,goal)
    printState(currState)
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
        printState(currState)
        if not flag:
            print('local maxima')
            return False
    return True


# Main program

# Initialisation of the domain 
startState,goalState = initGame(3,6)
print('Initialising...')
print('\nStart State : ')
printState(startState)
print('\nGoal State : ')
printState(goalState)
# print('h : ',heuristic_manhattan(startState,goalState))
# print('n : ',naiveMatching(startState,goalState))
# print('n : ',weightedMatching(startState,goalState))


print('BFS Manhattan')
BFS(startState,goalState,heuristic_manhattan)
print('\n\n BFS Naive')
BFS(startState,goalState,naiveMatching)
print('\n\n BFS Weighted Naive')
BFS(startState,goalState,weightedMatching)
print('HC Manhattan')
HC(startState,goalState,heuristic_manhattan)
print('\n\n HC Naive')
HC(startState,goalState,naiveMatching)
print('\n\n HC Weighted Naive')
HC(startState,goalState,weightedMatching)
print('\nStart State : ')
printState(startState)
print('\nGoal State : ')
printState(goalState)
