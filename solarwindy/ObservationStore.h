// Stores observations, that is, a group of sensor readings
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_OBSERVATIONSTORE_H
#define SOLARWINDY_OBSERVATIONSTORE_H

// Stores observations and computes an aggregate over the collection.
// The store must specify a size, it will allocate all of the memory
// it needs when constructed. Old (first in) values are discarded to make way 
// for new ones.
class ObservationStore {
 public:
  // An observation is a collection of sensor readings
  struct Observation {
    float wind_speed;
    unsigned char wind_segment;
    unsigned long time;
  };
  
  // An aggregate observation is calculated over a series of observations
  struct AggregateObservation {
    float average_wind_speed;
    float wind_gust;
    unsigned long wind_gust_time;
    unsigned char wind_gust_segment;
    unsigned char most_frequent_wind_segment;
    unsigned long elapsed_time;
  };
  
  explicit ObservationStore(unsigned int size);
  virtual ~ObservationStore();
  
  bool Init();
  void Add(Observation observation);
  AggregateObservation ComputeAggregate();
  unsigned int size() { return size_; }
  unsigned int observation_count() { return observation_count_; }
  
 private:
  static const int kNumSegments = 16;
  Observation *observations_;
  unsigned int size_;
  unsigned int next_position_;
  unsigned int observation_count_;
};

#endif
