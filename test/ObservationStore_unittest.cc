#include <limits.h>
#include "ObservationStore.h"
#include "gtest/gtest.h"

class ObservationStoreTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    o0_.wind_speed = 12.0;
    o0_.wind_segment = 3;
    o0_.temp = 21.70;
    o0_.time = 1200;
    
    o1_.wind_speed = 12.5;
    o1_.wind_segment = 5;
    o1_.temp = 21.0;
    o1_.time = 5000;

    o2_.wind_speed = 15.2; // gust
    o2_.wind_segment = 6;
    o2_.temp = 19.1;
    o2_.time = 8000;

    o3_.wind_speed = 12.0;
    o3_.wind_segment = 5;
    o3_.temp = 20.5;
    o3_.time = 12000;
  }
 
  virtual void TearDown() {
  }
 
  ObservationStore::Observation o0_;
  ObservationStore::Observation o1_;
  ObservationStore::Observation o2_;
  ObservationStore::Observation o3_;  
};

// Old observations are rolled over
TEST_F(ObservationStoreTest, MaxSize) {
  ObservationStore store(2);
  store.Add(o0_);
  store.Add(o1_);
  store.Add(o2_);
  store.Add(o3_);
  
  // store should have maxed out at 2
  EXPECT_EQ(store.Size(), (unsigned long)2);
}

// Test aggregations over a full load of observations
TEST_F(ObservationStoreTest, AggregationFull) {
  ObservationStore store(120);
  store.Add(o0_);
  store.Add(o1_);
  store.Add(o2_);
  store.Add(o3_);
  
  // store should have all observations
  EXPECT_EQ(store.Size(), (unsigned long)4);
  
  ObservationStore::AggregateObservation a = store.ComputeAggregate();
  EXPECT_EQ(a.average_wind_speed, 12.925f);
  EXPECT_EQ(a.wind_gust, 15.2f);
  EXPECT_EQ(a.wind_gust_time, static_cast<unsigned long>(8000));
  EXPECT_EQ(a.wind_gust_segment, 6);
  EXPECT_EQ(a.most_frequent_wind_segment, 5);
  EXPECT_EQ(a.average_temp, 20.575f);
  EXPECT_EQ(a.elapsed_time, static_cast<unsigned long>(10800));
}

// Test aggregations when store gets maxed and rolls over
TEST_F(ObservationStoreTest, AggregationRollover) {
  ObservationStore store(2);
  store.Add(o0_);
  store.Add(o1_);
  store.Add(o2_);
  store.Add(o3_);
  
  // store should only have 2 observations
  EXPECT_EQ(store.Size(), (unsigned long)2);
  
  ObservationStore::AggregateObservation a = store.ComputeAggregate();
  EXPECT_EQ(a.average_wind_speed, 13.6f);
  EXPECT_EQ(a.wind_gust, 15.2f);
  EXPECT_EQ(a.wind_gust_time, static_cast<unsigned long>(8000));
  EXPECT_EQ(a.wind_gust_segment, 6);
  EXPECT_EQ(a.most_frequent_wind_segment, 5);
  EXPECT_EQ(a.average_temp, 19.8f);
  EXPECT_EQ(a.elapsed_time, static_cast<unsigned long>(4000));
}


// Testing for memory leaks. This is commented out usually as it's an 
// infinite loop. Comment back in and watch the process to ensure no memory 
// is leaked.
// TEST_F(ObservationStoreTest, MemoryLeaks) {
//   // 12 readings a minute times 10 minutes are production values
//   ObservationStore store(120);
//   
//   while (true) {
//     store.Add(o0_);
//   }
// }