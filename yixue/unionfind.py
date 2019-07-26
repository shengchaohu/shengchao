import os
import numpy as np
import json

'''
implementation of union/find class
'''

class UnionFindSet:
    def __init__(self, n):
        self._parents = [i for i in range(n)]
        self._ranks = [1 for i in range(n)]
    
    def find(self, u):
        while u != self._parents[u]:
            self._parents[u] = self._parents[self._parents[u]]
            u = self._parents[u]
        return u
    
    def union(self, u, v):
        pu, pv = self.find(u), self.find(v)
        if pu == pv: return False
        
        if self._ranks[pu] < self._ranks[pv]:
            self._parents[pu] = pv
        elif self._ranks[pu] > self._ranks[pv]:
            self._parents[pv] = pu
        else:        
            self._parents[pv] = pu
            self._ranks[pu] += 1
        
        return True

    def adjacent(self):
        parents=self._parents
        self._linkedlist={i:[] for i in range(len(parents))}
        for i in range(len(parents)):
            if i != parents[i]:
                self._linkedlist[parents[i]].append(i)
        linkedlist=self._linkedlist.copy()
        return linkedlist

    #先做一个拓扑排序，然后对每个source进行树遍历即可。
    def source(self):
        adjacent_list=self.adjacent()
        mask=[1]*len(self._parents)
        for key,val in adjacent_list.items():
            for v in val:
                mask[v]=0
        source=[i for i in range(len(self._parents)) if mask[i]==1]
        return source

    def postorder(self): # postorder tree traverse
        source=self.source()
        adjacent_list=self.adjacent()
        _cluster=[]
        for s in source: 
            if not adjacent_list[s]:
                _cluster.append([s])
                continue
            c=[]
            self._postorder(s,adjacent_list,c)
            _cluster.append(c)
        return _cluster
            
 
    def _postorder(self, root, adjacent_list, c):
        if not adjacent_list[root]: return
        for node in adjacent_list[root]:
            self._postorder(node, adjacent_list, c)
            c.append(node)






















