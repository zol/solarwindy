// The ObservationStore gathers weather observations. You must specify a size
// and it will drop old readings when the store fills up.
// This is useful for say keeping the last 5 minute's worth of 
// observations. The store is also used to compute an aggregate of the data.
// The store used to (can be) backed by a Queue, but I've rewritten it to
// use a dynamic array instead for a smaller memory footprint.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "ObservationStore.h"
#include <stddef.h>
#include <stdlib.h>

bool ObservationStore::Init() {
  // we must use malloc because arduino doesn't support new[]
  observations_ = 
      static_cast<Observation *>(malloc(size_ * sizeof(Observation)));
      
  return observations_ != NULL;
}

ObservationStore::ObservationStore(unsigned int size) {
  size_ = size;
  next_position_ = observation_count_ = 0;
  observations_ = NULL;
}

ObservationStore::~ObservationStore() {
  free(observations_);
}

// Add an observation and push one out if we are maxxed
void ObservationStore::Add(Observation observation) {
  observations_[next_position_] = observation;
  next_position_++;
  
  // Rollover and start writing to the beginning again
  if (next_position_ == size_)
    next_position_ = 0;
  
  // Increment the number of stored observations till we reach size
  if (observation_count_ < size_)
    observation_count_++;
}

// Calculate the aggregate of all the values in the store. Calculate
// whatever makes sense for that particular value, in terms of the wind_gust
// we also want to know what time it happened and where from.
ObservationStore::AggregateObservation ObservationStore::ComputeAggregate() {
  unsigned long count = 0;  
  float wind_speed_accumulator = 0.0;
  float wind_gust = 0.0;
  unsigned long wind_gust_time = 0;
  unsigned char wind_gust_segment = 0;
  unsigned char wind_segment_counts[kNumSegments];
  unsigned long min_time = 0;
  unsigned long max_time = 0;
  
  for (int i = 0; i < kNumSegments; i++)
    wind_segment_counts[i] = 0;

  for (unsigned int i = 0; i < observation_count_; i++) {
    Observation *observation = &observations_[i];
    
    wind_speed_accumulator += observation->wind_speed;
    
    if (observation->wind_speed >= wind_gust) {
      wind_gust = observation->wind_speed;
      wind_gust_time = observation->time;
      wind_gust_segment = observation->wind_segment;
    }
    
    // record the segment unless it's out of acceptable range
    if (observation->wind_segment > 0 && 
        observation->wind_segment < kNumSegments) {
      wind_segment_counts[observation->wind_segment] += 1;
    }
    
    if (min_time == 0 || observation->time < min_time)
      min_time = observation->time;
    
    if (observation->time > max_time)
      max_time = observation->time;
    
    count++;
  }
  
  AggregateObservation aggregate;
  aggregate.average_wind_speed = wind_speed_accumulator / count;
  aggregate.wind_gust = wind_gust;
  aggregate.wind_gust_time = wind_gust_time;
  aggregate.wind_gust_segment = wind_gust_segment;
  
  // find the segment with the largest occurence
  int max_wind_segment_count = 0;
  aggregate.most_frequent_wind_segment = -1;
  for (int i = 0; i < kNumSegments; i++) {
    if (wind_segment_counts[i] > max_wind_segment_count) {
      max_wind_segment_count = wind_segment_counts[i];
      aggregate.most_frequent_wind_segment = i;
    }
  }
  
  aggregate.elapsed_time = max_time - min_time;
  
  return aggregate;
}
