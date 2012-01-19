// FIFO Queue implementation. Caller is responsible for allocating memory
// for each data item passed to the queue.
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#include "Queue.h"
#include <cstddef>

// Advance the Iterator to the next node. If the node is advanced it returns
// true, otherwise we are at the end of the list.
bool Queue::Iterator::Advance() {
  if (node_ && node_->next_node) {
    node_ = node_->next_node;
    
    return true;
  }
  
  return false;
}

// Are we at the last item?
bool Queue::Iterator::IsLast() {
  if (!node_)
    return true; // If queue is empty, then let's say we're at the end
    
  return node_->next_node == NULL;
}

Queue::Queue() {
  front_ = back_ = NULL;
  size_ = 0;
}

Queue::~Queue() {
  // reclaim heap memory, node are destroyed in pop()
  void *data;
  
  do {
    data = Pop();
  } while(data != NULL);
}

bool Queue::IsEmpty() {
  return size_ == 0;
}

// Returns an Iterator pointing to the front of the queue.
Queue::Iterator Queue::Begin() {
  return Iterator(front_);
}

// Add an item onto the back of the queue, allocating the Node on the heap
void Queue::Push(void *data) {
  Node *new_node = new Node();
  new_node->data = data;
  new_node->next_node = NULL;
  
  if (back_ == NULL) { //  Queue is empty
    back_ = front_ = new_node;
  } else {
    back_->next_node = new_node;
    back_ = new_node;
  }
  
  size_++;
}

// Take an item off the front of the queue, freeing the memory of the Node
void *Queue::Pop() {
  if (front_ != NULL) {
    void *data = front_->data;
    Node *new_front = front_->next_node;
    delete front_;
    front_ = new_front;
    
    if (front_ == NULL)
      back_ = NULL;
      
    size_--;
    
    return data;
  }
  
  return NULL; //  NULL if queue is empty and Pop is called
}