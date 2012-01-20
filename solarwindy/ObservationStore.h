// Stores observations, that is, a group of sensor readings
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_OBSERVATIONSTORE_H
#define SOLARWINDY_OBSERVATIONSTORE_H

#include "Queue.h"

// Stores observations and computes and aggregate over the collection. 
// The store can have a maximum size in which case old (first in) values 
// are discarded to make way for new ones when size reaches the maximum
class ObservationStore {
 public:
  // An observation is a collection of sensor readings
  struct Observation {
    float wind_speed;
    int wind_segment;
    float temp;
    unsigned long time;
  };
  
  // An aggregate observation is calculated over a series of observations
  struct AggregateObservation {
    float average_wind_speed;
    float wind_gust;
    unsigned long wind_gust_time;
    int wind_gust_segment;
    int most_frequent_wind_segment;
    float average_temp;
    unsigned long elapsed_time;
  };
  
  // Use 0 for no maximum size
  explicit ObservationStore(unsigned long max_size) : max_size_(max_size) {}
  virtual ~ObservationStore();
  
  void Add(Observation observation);
  AggregateObservation ComputeAggregate();
  unsigned long Size() { return queue_.size(); }
  
 private:
  static const int kNumSegments = 16;
  Queue queue_;
  unsigned long max_size_;
};

#endif
