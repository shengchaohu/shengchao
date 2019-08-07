from collections import OrderedDict
import bisect
import load

class Interval:
    '''
    interval from json file, which is string by default
    '''
    def time_start_end(self, label=load.label, question_start_time_delta=8, 
            question_end_time_delta=5):
        '''
        return a list of time interval, consisting of [start,end,(file) index]
        '''
        _start_end=[[label[index][question_start_time_delta],
            label[index][question_end_time_delta],index] for index in range(len(label))]
        return sorted(_start_end,key=lambda x: x[0])

    def merge(self,intervals):
        '''
        merge intervals if overlapping. Intervals is of type list of list
        returns a list of list
        '''
        ans = []
        for interval in sorted(intervals, key=lambda x: x[0]):
            if not ans or interval[0] > ans[-1][1]:
                ans.append(interval)
            else:
                ans[-1][1] = max(ans[-1][1], interval[1])
        return ans

    def insert(self,intervals,newinterval,merge=0):
        '''
        linear time insert
        Insert new interval to a list of non-overlapping intervals (merge if merge=1). 
        if merge=0, the two overlapping intervals will be printed out. 
        The latter interval will not be added. Then Insert will continue.
        Intervals is of type list of list
        returns a list of list
        '''
        ans = []

        index = len(intervals)
        for i in range(len(intervals)):
            if newInterval[0] < intervals[i][0]:
                index = i
                break

        intervals.insert(index, newInterval) # this insert is built_in method for list

        # then repeat the same prodecure as Interval.merge(self, intervals)
        for interval in intervals:
            if not ans or interval[0] > ans[-1][1]:
                ans.append(interval)
            else:
                if(merge==0):
                    print(interval, ans[-1])
                    continue
                elif(merge==1):
                    ans[-1][1] = max(ans[-1][1], interval[1])
        return ans

class Interval_quick(Interval):
    '''
    fast implementation of Interval
    '''
    def time_start_end(self):
        '''
        return a list of time interval, consisting of [start,end,(file) index]
        '''
        c=Interval.time_start_end(self)
        c=[list(map(lambda ccc: int(float(ccc)), cc)) for cc in c]
        return sorted(c,key=lambda x: x[0])

    def insert(self,intervals,newinterval,merge=1):
        '''
        O(logn) time implementation of insert method
        ans and intervals are of type OrderedDict
        a in ans, i in intervals is of form {start time:(end time, index}
        new interval is given via a tuple (start time, end time, index)
        if merge=1, the overlapping intervals will be printed out and then merge.
        '''
        # suppose OrderedDict is implemented using double linked list
        ans = OrderedDict()

        # find left bound
        keys = list(intervals.keys())
        left_key=keys[bisect.bisect_left(keys,newinterval[0])]
        right_key=keys[bisect.bisect_left(keys,newinterval[1])]
        ......
        for i in range(len(intervals)):
            if newInterval[0] < intervals[i][0]:
                index = i
                break

        intervals.insert(index, newInterval) # this insert is built_in method for list

        # then repeat the same prodecure as Interval.merge(self, intervals)
        for interval in intervals:
            if not ans or interval[0] > ans[-1][1]:
                ans.append(interval)
            else:
                if(merge==0):
                    print(interval, ans[-1])
                    continue
                elif(merge==1):
                    ans[-1][1] = max(ans[-1][1], interval[1])
        return ans


class RangeModule():
    def __init__(self):
        self.ranges = []

    def _bounds(self, left, right):
        i, j = 0, len(self.ranges) - 1
        for d in (100, 10, 1):
            while i + d - 1 < len(self.ranges) and self.ranges[i+d-1][1] < left:
                i += d
            while j >= d - 1 and self.ranges[j-d+1][0] > right:
                j -= d
        return i, j

    def addRange(self, left, right):
        i, j = self._bounds(left, right)
        if i <= j:
            left = min(left, self.ranges[i][0])
            right = max(right, self.ranges[j][1])
        self.ranges[i:j+1] = [(left, right)]

    def queryRange(self, left, right):
        i = bisect.bisect_left(self.ranges, (left, float('inf')))
        if i: i -= 1
        return (bool(self.ranges) and
                self.ranges[i][0] <= left and
                right <= self.ranges[i][1])

    def removeRange(self, left, right):
        i, j = self._bounds(left, right)
        merge = []
        for k in range(i, j+1):
            if self.ranges[k][0] < left:
                merge.append((self.ranges[k][0], left))
            if right < self.ranges[k][1]:
                merge.append((right, self.ranges[k][1]))
        self.ranges[i:j+1] = merge

