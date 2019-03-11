#include "DistributedCounter5.h"

namespace mpcs51044 {

bitset<maxCounters> DistributedCounter5::usedCounters;
atomic<unsigned> DistributedCounter5::callsFromDeadCounters[maxCounters];
set<array<atomic<unsigned>, maxCounters> *> DistributedCounter5::liveCounters;
shared_mutex DistributedCounter5::mtx;
thread_local DistributedCounter5::DistributedCounterThrLocal DistributedCounter5::calls;

}