// FIFO Queue Implementation
// Zoltan Olah (zol@me.com) released under the MIT license on Jan 13 2012.

#ifndef SOLARWINDY_QUEUE_H
#define SOLARWINDY_QUEUE_H

// Implements the FIFO Queue also providing an Iterator to traverse it
class Queue {
 public:
  // Linked List style Node that points to the next one
  struct Node {
    void *data;
    Node *next_node;
  };
  
  // Iterator for the Queue. The Iterator purposefuly allows copy construction
  class Iterator {
   public:
    Iterator(Node *node) : node_(node) {}
    void *Data() { return node_->data; }
    bool Advance();
    bool IsLast();
    
   private:
    Node *node_;
  };
   
  explicit Queue();
  virtual ~Queue();
  
  Iterator Begin();
  void Push(void *data);
  void *Pop();
  bool IsEmpty();
  unsigned long size() { return size_; }
  
 private:
  Node *front_;
  Node *back_;
  unsigned long size_;
};

#endif
