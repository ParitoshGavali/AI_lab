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
    for i in state:
        for state_index,j in enumerate(i):
            for k in goal:
                for l,goal_index in enumerate(k):
                    if(j==l):
                        cost=cost + len(i) - state_index - 1 + len(k) - goal_index - 1
    return -cost

def naiveHelper(list1,list2):
    n = min(len(list1,list2))
    score = 0
    for i in range(n):
        if list1[i]==list2[i]:
            score += 1
    return score    
    
def naiveMatching(goal,state):
    score = 0
    for i in range(len(state)):
        score += naiveHelper(state[i],goal[i])
    return score

            
            

# AI algorithms



# Main program

# Initialisation of the domain 
startState,goalState = initGame(3,6)
visitedStates = []
pQueue = []
pQueue.append(startState)

print('Initialising...')
print('\nStart State : ')
printState(startState)
print('\nGoal State : ')
printState(goalState)
