// The ObservationStore gathers weather observations. You can set a max
// value and it will drop old readings when the store fills to it's max.
// This is useful for say keeping the last 5 minute's worth of 
// observations. The store is also used to compute an aggregate of the data.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "ObservationStore.h"
#include <stddef.h>

ObservationStore::~ObservationStore() {
  Observation *disgard;
  
  do {
    disgard = static_cast<Observation *>(queue_.Pop());
    if (disgard != NULL)
      delete disgard;
  } while(disgard != NULL);
}

// Add an observation and push one out if we are maxxed
void ObservationStore::Add(Observation observation) {
  Observation *pObservation = new Observation(observation);
  queue_.Push(pObservation);
  
  // If we just stepped over the max size, pop an item off the queue
  if (max_size_ != 0 && queue_.size() > max_size_) {
    Observation *disgard = static_cast<Observation *>(queue_.Pop());
    delete disgard;
  }
}

// Calculate the aggregate of all the values in the store. Calculate
// whatever makes sense for that particular value, in terms of the wind_gust
// we also want to know what time it happened and where from.
ObservationStore::AggregateObservation ObservationStore::ComputeAggregate() {
  unsigned long count = 0;  
  float wind_speed_accumulator = 0.0;
  float wind_gust = 0.0;
  unsigned long wind_gust_time = 0;
  int wind_gust_segment = 0;
  float temp_accumulator = 0.0;
  int wind_segment_counts[kNumSegments];
  unsigned long min_time = 0;
  unsigned long max_time = 0;
  
  for (int i = 0; i < kNumSegments; i++)
    wind_segment_counts[i] = 0;

  Queue::Iterator it = queue_.Begin();

  do {
    Observation *observation = static_cast<Observation *>(it.Data());
    
    wind_speed_accumulator += observation->wind_speed;
    
    if (observation->wind_speed > wind_gust) {
      wind_gust = observation->wind_speed;
      wind_gust_time = observation->time;
      wind_gust_segment = observation->wind_segment;
    }
    
    temp_accumulator += observation->temp;
    
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
  } while(it.Advance() != false);
  
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
  
  aggregate.average_temp = temp_accumulator / count;
  aggregate.elapsed_time = max_time - min_time;
  
  return aggregate;
}
