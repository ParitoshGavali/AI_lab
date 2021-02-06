import random
import copy
from itertools import combinations
from queue import PriorityQueue

def generate_clause(n):
    clause=[ (random.randrange(-1,2,2)*x) for x in random.sample(range(1,n+1),3)]
    clause.sort()
    return clause

def generate_formula(n,k):
    formula=[]
    for i in range(k):
        clause=generate_clause(n)
        while clause in formula:
            clause=generate_clause(n)
        formula.append(clause)
    return formula

def print_formula(formula):
    str=""
    for clause in formula:
        for literal in clause:
            if(literal<0):
                str=str+'~'
                literal= - literal
            str= str + chr(ord('`')+literal) + ' v '
        str= str[:-2]
        str=str+'\n'
    print(str)

def heuristic(formula,state):
    val=0
    for clause in formula:
        flag = False
        for literal in clause:
            if((literal > 0) and state[literal-1]==1):
                flag=True
                break
            if((literal < 0) and state[-literal-1]==0):
                flag=True
                break
        if(flag):
            val=val+1
    return val

def goalTest(formula,state):
    if(heuristic(formula,state)==len(formula)):
        return True
    return False

def moveGen(state, k):
    nextStates=[]
    allCombinations=list(combinations(range(len(state)),k))
    for combination in allCombinations:
        nextState=copy.deepcopy(state)
        for i in combination:
            nextState[i]=1-nextState[i]
        nextStates.append(nextState)
    return nextStates

def generateRandomState(n):
    state=[]
    for i in range(n):
        state.append(random.randint(0,1))
    return state

def VND(formula, n):
    state=generateRandomState(n)
    print("Starting VND.....\n")
    print("Initial State:",state)
    heuristic_value=heuristic(formula,state)
    print(heuristic_value)
    count=1
    kmax=n
    k=1
    while(not goalTest(formula,state) and k <= kmax):        
        moveSet=moveGen(state,k)
        count=count+1
        best_heuristic_value=heuristic_value
        for move in moveSet:
            move_heuristic_value=heuristic(formula,move)
            if(move_heuristic_value > best_heuristic_value):
                best_heuristic_value=move_heuristic_value
                nextMove=move
        if(best_heuristic_value!=heuristic_value):
            print("Better state found in neighborhood",k)
            k=1
            heuristic_value=best_heuristic_value
            state=nextMove
            print(heuristic_value)
            print(state)
        else:
            print("No better state in neighborhood",k)
            k=k+1
        print("Total states explored:",count)

def beamSearch(formula,n,beamLength,initState):
    state=initState
    print("Starting beam search.....")
    print("Initial State:",state)
    heuristic_value=heuristic(formula,state)
    print(heuristic_value)
    visited=[]
    pq=PriorityQueue()
    pq.put((heuristic(formula,state),state))
    count=0
    while not pq.empty():
        x=pq.get()
        state=x[1]
        count+=1
        # print("Element popped from queue:",state)
        # print("Heuristic value:",x[0])
        if(goalTest(formula,state)):
            print("Goal state found")
            break
        else:
            nextStates=moveGen(state,1)
            nextStates.sort(key=lambda x:heuristic(formula,x))
            for nextState in nextStates[:beamLength]:
                if nextState not in visited:
                    pq.put((heuristic(formula,nextState),nextState))
                    visited.append(nextState)
    print("Total states explored:",count)

def tabuMoveGen(state):
    neighbours = []
    for i in range(len(state)):
        neighbour = copy.deepcopy(state)
        neighbour[i] = 1 - neighbour[i]
        neighbours.append(neighbour)
    return neighbours

def tabuSearch(formula,n,tt,initState):
    print("\nStarting Tabu Search ...")
    bestState=initState
    print("Initial State:",bestState)
    bestCandidate = bestState
    stoppingCriterion = 2**n
    itr = 0
    tabuList = [0]*n
    if(tt>=n):
        print("please use smaller tt")
        return
    while(not goalTest(formula,bestState) and (itr < stoppingCriterion)):
        itr += 1
        print("Current tabu list : ",tabuList)
        print("Prev bestcandidate : ",bestCandidate)
        neighbours = tabuMoveGen(bestCandidate)
        bestCandidate = []
        tabuElement = -1
        print("Neighbours : ",neighbours)
        for i in range(len(tabuList)):
            if tabuList[i]==0 :
                if len(bestCandidate) == 0:
                    bestCandidate = neighbours[i]
                    tabuElement = i
                else:
                    if(heuristic(formula,neighbours[i]) > heuristic(formula,bestCandidate)) :
                        bestCandidate = neighbours[i]
                        tabuElement = i
            else : 
                tabuList[i] = tabuList[i]-1
        tabuList[tabuElement] = tt
        print("New bestcandidate : ",bestCandidate)
        print("New Tabu list : ",tabuList)
        if(heuristic(formula,bestCandidate) > heuristic(formula,bestState)):
            bestState = bestCandidate
            print("Best State updated, heuristic value = ",heuristic(formula,bestState))
    if( itr >= stoppingCriterion ):
        print("Number of iterations exceeded! \n The best State so far is ",bestState)
    else :
        print("Best State is : ",bestState)
    print("number of states explored : ",itr+1)

def main():
    n=5
    k=44
    f=generate_formula(n,k)
    #f=[[1,2,3],[-1,2,3],[-2,1,3],[-3,1,2],[-2,-1,3],[-3,-1,2],[-3,-2,1],[-3,-2,-1]]
    #print(f)
    print_formula(f)
    #print(heuristic(f,[0,0,0,0]))
    #print(moveGen([0,0,0,0]))
    #print(list(combinations(range(4),3)))
    #print(moveGen([1,1,0,0],1))
    #print(generateRandomState(4))
    # VND(f,n)
    # for i in range(n):
    #     print("\nBeam search for beam length : ",i)
    initState=generateRandomState(n)
    
    # beamSearch(f,n,2,initState)

    for i in range(1,n+1,1):
        print("i",i)
        tabuSearch(f,n,i,initState)
        # beamSearch(f,n,i,initState)
    # tabuSearch(f,n,5,initState)
main()