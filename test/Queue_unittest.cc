#include <limits.h>
#include "Queue.h"
#include "gtest/gtest.h"

// Test fixtures for our queue, let's allocate them on the heap to
// simulate real world use.
class QueueTest : public ::testing::Test {
 protected:
  struct TestData {
   public:
     TestData(int id, char data) : id(id), data(data) {}
    int id;
    char data;
  };
   
  virtual void SetUp() {
    td1_ = new TestData(0, 'z');
    td2_ = new TestData(1, 'o');
    td3_ = new TestData(2, 'l');
    td4_ = new TestData(3, 'i');
  }

  virtual void TearDown() {
    delete td1_;
    delete td2_;
    delete td3_;
  }

  TestData *td1_;
  TestData *td2_;
  TestData *td3_;
  TestData *td4_;  
};

// Tests whether the queue correctly recognizes it's empty states.
TEST_F(QueueTest, Empty) {
  Queue queue;  
  
  EXPECT_EQ(queue.size(), (unsigned long)0);
  EXPECT_EQ(queue.IsEmpty(), true);
}

// Tests that the queue stores and retrieves a single item
TEST_F(QueueTest, SingleItem) {
  Queue queue;
  queue.Push(td1_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(1));
    
  TestData *td = static_cast<TestData *>(queue.Pop());
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(0));
  EXPECT_EQ(td->data, 'z');
}

// Testing that the queue correctly stores and retrives multiple items.
// In particular, checking that items are returned in the correct order
// and the size is correctly counted.
TEST_F(QueueTest, MultiItem) {
  Queue queue;
  queue.Push(td1_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(1));

  queue.Push(td2_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(2));

  queue.Push(td3_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(3));

  TestData *td = static_cast<TestData *>(queue.Pop());
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(2));
  EXPECT_EQ(td->data, 'z');
  
  queue.Push(td4_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(3));
  
  td = static_cast<TestData *>(queue.Pop());
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(2));
  EXPECT_EQ(td->data, 'o');

  td = static_cast<TestData *>(queue.Pop());
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(1));
  EXPECT_EQ(td->data, 'l');
  
  td = static_cast<TestData *>(queue.Pop());
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(0));
  EXPECT_EQ(td->data, 'i');
}

// Testing that the iterator works correctly
TEST_F(QueueTest, Iterator) {
  Queue queue;
  queue.Push(td1_);
  queue.Push(td2_);
  queue.Push(td3_);
  queue.Push(td4_);
  EXPECT_EQ(queue.size(), static_cast<unsigned long>(4));
  
  int count = 0;
  Queue::Iterator it = queue.Begin();

  do {
    EXPECT_EQ(static_cast<TestData *>(it.Data())->id, count);
    count++;
  } while(it.Advance() != false);
  
  EXPECT_EQ(count, 4);
  EXPECT_EQ(it.IsLast(), true);
}

// Testing for memory leaks. This is commented out usually as it's an 
// infinite loop. Comment back in and watch the process to ensure no memory 
// is leaked.
// TEST_F(QueueTest, MemoryLeaks) {
//   while (true) {
//     Queue queue;
//     queue.Push(td1_);
//     EXPECT_EQ(queue.size(), static_cast<unsigned long>(1));
// 
//     TestData *td = static_cast<TestData *>(queue.Pop());
//     EXPECT_EQ(queue.size(), static_cast<unsigned long>(0));
//     EXPECT_EQ(td->data, 'z');
//   }
// }