import random


class SkipListNode:
    def __init__(self, nLevels, key, nHop):
        self.m_NextNodes = [None for _ in range(nLevels)]
        self.m_Key = key        
        self.m_nHops = [nHop for _ in range(nLevels)]
        
    
class SkipList:
    def __init__(self, funcEqual=None, funcLessThan=None, nMaxLevels=12, nPromoteProb=0.5, nLength=None):
        self.m_Head = SkipListNode(1, None, 1)
        self.m_Tail = None
        self.m_nMaxLevels = nMaxLevels
        self.m_nPromoteProb = nPromoteProb
        self.m_funcLessThan = funcLessThan
        self.m_funcEqual = funcEqual
        self.m_nMaxLength = nLength
        self.m_nLength = 0
        self.m_listCache = []        
        self.m_bDirty = True
    
    def _LessThan(self, a, b):
        return a < b
    
    def _Equal(self, a, b):
        return a == b
    
    def ShouldPromote(self):
        return random.random() < self.m_nPromoteProb

    def CreateNewLevel(self):
        if self.GetCurrentLevels() == self.m_nMaxLevels:
            return        
        self.m_Head.m_NextNodes.append(None)        
        self.m_Head.m_nHops.append(self.m_nLength+1)

    def GetCurrentLevels(self):
        return len(self.m_Head.m_NextNodes)

    def __iter__(self):
        Node = self.m_Head.m_NextNodes[0]        
        while Node:
            yield Node.m_Key
            Node = Node.m_NextNodes[0]        

    def _GetRankNode(self, nIndex):
        if nIndex > self.m_nLength or nIndex <= 0:
            raise IndexError("index out of range")
        
        nSteps = 0                        
        Node = self.m_Head
        for nLevel in reversed(range(self.GetCurrentLevels())):            
            while Node and nSteps + Node.m_nHops[nLevel] <= nIndex:                
                nSteps += Node.m_nHops[nLevel]
                Node = Node.m_NextNodes[nLevel]

            if nSteps == nIndex:
                break

        return Node
    
    def GetRank(self, nIndex):
        Node = self._GetRankNode(nIndex)
        return Node.m_Key

    def GetRankRange(self, nStart, nEnd):
        if nStart > nEnd:
            raise ValueError("nStart should be smaller than nEnd")

        if nStart > self.m_nLength or nStart <= 0 or nEnd > self.m_nLength or nEnd <= 0:            
            raise IndexError("index out of range")
        
        StartNode = self._GetRankNode(nStart)
        nLength = nEnd - nStart + 1
        while nLength > 0:
            nLength -= 1
            yield StartNode.m_Key
            StartNode = StartNode.m_NextNodes[0]
                
    def Insert(self, key):

        # determine the levels the node should be promoted to
        nLevelToPrompt = 0
        nCurrentLevels = self.GetCurrentLevels()
        for _ in range(nCurrentLevels):
            if not self.ShouldPromote():
                break
            nLevelToPrompt += 1            
            if nLevelToPrompt == nCurrentLevels:
                # create new level
                self.CreateNewLevel()
                break

        NewNode = SkipListNode(nLevelToPrompt+1, key, 0)

        Node = self.m_Head
        nCurrentLevels = self.GetCurrentLevels()
        
        listHops = [0] * (nCurrentLevels)
        listNodes = [None] * nCurrentLevels

        for nLevel in reversed(range(nCurrentLevels)):
            NextNode = Node.m_NextNodes[nLevel]
            # find the first node that is greater than the key
            while NextNode and self._LessThan(NextNode.m_Key, key):                
                listHops[nLevel] += Node.m_nHops[nLevel]
                Node = NextNode                
                NextNode = NextNode.m_NextNodes[nLevel]                
            listNodes[nLevel] = Node

        nSteps = 0
        for nLevel, Node in enumerate(listNodes):
            NextNode = Node.m_NextNodes[nLevel]
            
            if nLevel <= nLevelToPrompt:                
                # insert node to this level                           
                NewNode.m_NextNodes[nLevel] = NextNode                                
                Node.m_NextNodes[nLevel] = NewNode
                # split the link                
                NewNode.m_nHops[nLevel] = Node.m_nHops[nLevel] - nSteps
                Node.m_nHops[nLevel] = nSteps

            # a new node has been inserted, all hops should be increased by 1
            Node.m_nHops[nLevel] += 1            
            nSteps += listHops[nLevel]
            
            if nLevel == 0:
                # update the tail
                if not NextNode:
                    self.m_Tail = NewNode

        self.m_nLength += 1
        self._Truncate()
        self.m_bDirty = True

    def Delete(self, key):        
        Node = self.m_Head
        TargetNode = None
        listNodes = [None] * self.GetCurrentLevels()          
        for nLevel in reversed(range(self.GetCurrentLevels())):
            TargetNode = Node.m_NextNodes[nLevel]
            while TargetNode and self._LessThan(TargetNode.m_Key, key):
                Node = TargetNode
                TargetNode = TargetNode.m_NextNodes[nLevel]
            
            listNodes[nLevel] = Node
                
        if not TargetNode or not self._Equal(key, TargetNode.m_Key):
            return

        nLevels = len(TargetNode.m_NextNodes)        
        for nLevel, Node in enumerate(listNodes):
            if nLevel < nLevels:
                Node.m_NextNodes[nLevel] = TargetNode.m_NextNodes[nLevel]
                Node.m_nHops[nLevel] += TargetNode.m_nHops[nLevel]
                if nLevel == 0:
                    # update the tail
                    if TargetNode == self.m_Tail:
                        self.m_Tail = Node                
            Node.m_nHops[nLevel] -= 1

        self.m_Head.m_NextNodes = [Node for i, Node in enumerate(self.m_Head.m_NextNodes) if i == 0 or Node]

        self.m_bDirty = True
        self.m_nLength -= 1

    def GetLength(self):
        return self.m_nLength
    
    def Search(self, key):        
        Node = self.m_Head
        for nLevel in reversed(range(self.GetCurrentLevels())):
            NextNode = Node.m_NextNodes[nLevel]
            
            while NextNode and self._LessThan(NextNode.m_Key, key):
                Node = NextNode
                NextNode = NextNode.m_NextNodes[nLevel]

            if NextNode and self._Equal(NextNode.m_Key, key):
                return NextNode
    
        return None

    def _Truncate(self):        
        if not self.m_nMaxLength:
            return

        while self.m_nLength > self.m_nMaxLength:
            self.Delete(self.m_Tail.m_Key)

    def GetList(self):
        if not self.m_bDirty:
            return self.m_listCache
 
        Node = self.m_Head.m_NextNodes[0]
        listResult = []
        while Node:
            listResult.append(Node.m_Key)
            Node = Node.m_NextNodes[0]            

        self.m_bDirty = False
        self.m_listCache = listResult
        return listResult
    
    def GetDebugList(self):
        listLevels = []
        
        for i in reversed(range(self.GetCurrentLevels())):
            Node = self.m_Head.m_NextNodes[i]
            listLevel = [("head", self.m_Head.m_nHops[i], self.m_Head.m_NextNodes[i].m_Key if self.m_Head.m_NextNodes[i] else None)]
            while Node:
                listLevel.append((Node.m_Key, Node.m_nHops[i], Node.m_NextNodes[i].m_Key if Node.m_NextNodes[i] else None))
                Node = Node.m_NextNodes[i]
            listLevels.append(listLevel)
        return listLevels

    def GetAllLevels(self):
        listLevels = []
        
        for i in reversed(range(self.GetCurrentLevels())):
            Node = self.m_Head.m_NextNodes[i]
            listLevel = [("head", self.m_Head.m_nHops[i], self.m_Head.m_NextNodes[i].m_Key if self.m_Head.m_NextNodes[i] else None)]
            while Node:
                listLevel.append((Node.m_Key, Node.m_nHops[i], Node.m_NextNodes[i].m_Key if Node.m_NextNodes[i] else None))
                Node = Node.m_NextNodes[i]
            listLevels.append(listLevel)
        return listLevels


def test_skip_list():
    # test creation of SkipList
    for i in range(10):
        s = SkipList(nLength=2)
        assert s.m_Head.m_Key is None
        assert s.GetCurrentLevels() == 1

        # test insertion
        s.Insert(5)
        assert s.Search(5).m_Key == 5
        
        s.Insert(3)
        assert s.Search(3).m_Key == 3

        assert s.GetRank(1) == 3
        assert s.GetRank(2) == 5, "result: {result}, expect: {expect}".format(result=s.GetRank(2), expect=5)

        s.Insert(1)    
        assert s.Search(1).m_Key == 1
        assert s.GetRank(2) == 3

        assert s.Search(5) == None

        # test ordering of nodes
        assert s.GetList() == [1, 3]

        assert s.GetRank(1) == 1    
        assert s.GetRank(2) == 3
        # test deletion
        s.Delete(1)
        assert s.Search(1) == None  # after deleting 5, the search should return the node with key 3

        assert s.GetList() == [3]

    # test order
    for i in range(10):
        s = SkipList(nLength=10)
        listRandom = random.sample(range(1000), 100)        
        for n in listRandom:
            s.Insert(n)
        result = s.GetList() 
        expect = sorted(listRandom)[:10]
        assert result == expect, "result: {result}, expect: {expect}".format(result=result, expect=expect)       

    # test delete
    for i in range(10):
        s = SkipList()
        setRandom = set(random.sample(range(100), 10))
        setRandom2 = set(random.sample(range(100), 10))
        for i in setRandom:
            s.Insert(i)
  
        for i in setRandom2:
            s.Delete(i)

        result = s.GetList()
        expect = sorted(list(setRandom.difference(setRandom2)))
        
        assert result == expect, "list1: {setRandom} list2: {setRandom2} result: {result}, expect: {expect}".format(result=result, expect=expect)
        assert s.GetLength() == len(result)

        for i, n in enumerate(expect, 1):
            assert s.GetRank(i) == n
        assert list(s.GetRankRange(1, s.GetLength())) == expect, "result: {}, expect: {}".format(list(s.GetRankRange(1, s.GetLength())), expect)

        s = SkipList()
        listRandom = random.sample(range(100), 10)
        for i in listRandom:
            s.Insert(i)

        for i in listRandom:
            s.Delete(i)

        for i in listRandom:
            s.Delete(i)

        assert s.GetList() == []
        assert s.GetLength() == 0

    print("all pass")

def test_performance():
    import timeit        
    from  cProfile import Profile
    from pstats import Stats
    trials = 100000
    length = 16
    listSkip = []
    listLength = []
    listSort = []
    for i in range(10):
        length *= 2

        listLength.append(length)

        s = SkipList(nLength=length)
        sequence = [random.random() for i in range(trials)]

        start = timeit.default_timer()    
        for n in sequence:
            s.Insert(n)
        stop = timeit.default_timer()
        print("Time to insert in SkipList: {}".format(stop-start))
        
        s.Delete(0.2)
        s.Insert(0.000000001)
        
        start = timeit.default_timer()
        for n in sequence:
            s.Delete(n)
            s.Insert(n)            
            
        stop = timeit.default_timer()
        listSkip.append(stop - start)
        print("Time to delete in SkipList: {}".format(stop-start))

        start = timeit.default_timer()

        listsort = sequence[:length]
        for n in sequence:
            listsort = [i for i in listsort if i != n]
            listsort.append(n)
            listsort.sort()
            listsort = listsort[:length]
            for i, j in enumerate(listsort):
                if j == n:
                    break

        stop = timeit.default_timer()
        listSort.append(stop - start)
        print("Time to insert in sort: {}".format(stop-start))

    print("Length: ", listLength)
    print("SkipList: ", listSkip)
    print("Sort: ", listSort)
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots()
    ax.plot(listLength, listSkip, label="SkipList")
    ax.plot(listLength, listSort, label="Sort")
    #plt.xscale('log', base=2)
    #plt.yscale('log', base=2)
    ax.set_xlabel('size')
    ax.set_ylabel('time')
    ax.legend()
    plt.show()

#test_skip_list()
#test_performance()
skip = SkipList(nMaxLevels=5)
for i in range(10):
    skip.Insert(i)
print(skip.GetDebugList())

