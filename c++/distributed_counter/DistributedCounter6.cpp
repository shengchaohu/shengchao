#include "DistributedCounter6.h"

namespace mpcs51044 {

bitset<maxCounters> DistributedCounter6::usedCounters;
atomic<unsigned> DistributedCounter6::callsFromDeadCounters[maxCounters];
set<array<atomic<unsigned>, maxCounters> *> DistributedCounter6::liveCounters;
shared_mutex DistributedCounter6::mtx;
thread_local DistributedCounter6::DistributedCounterThrLocal DistributedCounter6::calls;

}