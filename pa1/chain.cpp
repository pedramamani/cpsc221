#include "chain.h"
#include "chain_given.cpp"
#include <vector>
using namespace std;

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
  cout << 11 << '\n';
  /* your code here */
  clear();
  length_ = 0;
}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
  cout << 22 << '\n';
  /* your code here */

  Node * newNode = new Node(ndata);
  if(length_ == 0){
    head_->next = newNode;
    head_->prev = newNode;
    newNode->next = head_;
    newNode->prev = head_;
    length_ = 1;
  }
  else{
    Node * tail = head_->next;
    while(tail->next!=head_){
      tail = tail->next;
    }
    tail->next = newNode;
    newNode->prev = tail;
    newNode->next = head_;
    head_->prev = newNode;
    length_++;
  }
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length.
 */
void Chain::moveBack(int startPos, int len, int dist){
  cout << 33 << '\n';
  /* your code here */
  cout << "kos1 " << length_   << '\n';

  Node * curNode = head_->next;
  Node * nextNode;
  Node * startNode;
  Node * endNode;
  Node * beforeStart;
  Node * afterEnd;
  int counter = 1;

  if(len != 0 || dist != 0){
    if(startPos + len - 1 + dist > length_)
      dist = length_ - startPos - len + 1;

    while(counter < startPos){
      curNode = curNode->next;
      counter++;
    }

    cout << "start counter " << counter << '\n';
    startNode = curNode;

    while(counter < startPos+len-1){
      curNode = curNode->next;
      counter++;
    }

    cout << "end counter " << counter << '\n';
    endNode = curNode;

    beforeStart = startNode->prev;
    afterEnd = endNode->next;

    counter = 0;

    while(counter < dist){
      curNode = curNode->next;
      counter++;
    }
    cout << "counter is " << counter << '\n';
    nextNode = curNode->next;

    beforeStart->next = afterEnd;
    afterEnd->prev = beforeStart;

    cout << "kalle shiri " << '\n';
    nextNode->prev = endNode;
    endNode->next = nextNode;
    startNode->prev = curNode;
    curNode->next = startNode;

    counter = 1;
    curNode = head_->next;
    while(counter<length_){
      curNode = curNode->next;
      counter++;
      cout << "suck it " << counter << '\n';
    }

  }
}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
  cout << 44 << '\n';
  /* your code here */
  Node * tail_ = head_;
  for(int i=0; i<length_-1; i++){
    tail_ = tail_->next;
  }

  if(k<length_ && length_ > 1){
    int counter = 1;
    Node * curNode = head_;
    while(counter < length_-k+1){
      counter++;
      curNode = curNode->next;
    }
    tail_->next = head_;
    head_->prev = tail_;
    tail_ = curNode->prev;
    head_ = curNode;
    tail_->next = NULL;
    head_->prev = NULL;
  }
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){
  cout << 55 << '\n';
  /* your code here */


  if(pos2-pos1 > 0){
    Node * curNode = head_->next;
    Node * beginNode;
    Node * endNode;

    Node * beforeBegin;
    Node * afterEnd;

    vector<Node *> vec;

    int counter = 1;

    while(counter < pos1){
      curNode = curNode->next;
      counter++;
    }

    beginNode = curNode;

    while(counter < pos2){
      curNode = curNode->next;
      counter++;
    }

    endNode = curNode;

    beforeBegin = beginNode->prev;
    afterEnd = endNode->next;

    curNode = beginNode;

    vec.push_back(curNode);

    while(curNode != afterEnd){
      curNode = curNode->next;
      vec.push_back(curNode);
    }

    for(unsigned long i=0; i<vec.size()-1; i++){
      vec[i]->prev = vec[i+1];
      vec[i+1]->next = vec[i];
    }

    beforeBegin->next  = endNode;
    endNode->prev = beforeBegin;

    afterEnd->prev = beginNode;
    beginNode->next = afterEnd;

  }
}

/*
* Modifies both the current chain and the "other" chain by removing
* nodes from the other chain and adding them between the nodes
* of the current chain -- one "other" node between two current nodes --
* until one of the two chains is exhausted.  Then the rest of the
* nodes in the non-empty chain follow.
* The length of the resulting chain should be the sum of the lengths
* of current and other. The other chain should have only
* the head_ sentinel at the end of the operation.
* The weave fails and the original
* chains should be unchanged if block sizes are different.
* In that case, the result of the function should be:
* cout << "Block sizes differ." << endl;
*/
void Chain::weave(Chain & other) { // leaves other empty.
  cout << 66 << '\n';
  /* your code here */
  if(length_ == other.length_){
    Node * curNode = head_->next;
    Node * otherNode = other.head_->next;
    int counter = 0;

    while(other.length_ > 0){
      if(counter%2 == 0){
        curNode->next = otherNode;
        otherNode->prev = curNode;
        curNode = curNode->next;
        length_++;
        counter++;
      }
      else{
        otherNode->next = curNode;
        curNode->prev = otherNode;
        otherNode = otherNode->next;
        other.length_--;
        counter++;
      }

    }
    cout << "8==D " << counter << '\n';

    other.head_->next = other.head_;
    other.head_->prev = other.head_;


    cout << "weave done!!" << '\n';
  }
  else{
    cout << "Block sizes differ." << endl;
  }
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
  cout << 77 << '\n';
  /* your code here */
  Node * curNode = head_->next;

  while(length_>0){
    Node * tmp = curNode->next;
    delete curNode;
    curNode = tmp;
    length_--;
  }
}


/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
  cout << 88 << '\n';

  head_ = new Node(other.head_->data);
  Node * curNew = head_;
  Node * curOther = other.head_;

  for(int i=0; i<length_; i++){
    Node * tmp = curNew;
    curOther = curOther->next;
    curNew = new Node(curOther->data);
    curNew->prev = tmp;
    tmp->next = curNew;
  }

  curNew->next = head_;
  head_->prev = curNew;

  length_ = other.length_;
}
