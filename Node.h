//
// Created by Benito Echarren on 18/07/2017.
//

#ifndef RUBIKSCUBE_NODE_H
#define RUBIKSCUBE_NODE_H

/**
 * Template for a genetic wrapper class Node, which holds a value and a pointer
 * to the next node. It will be used to define the a generic linkedList
 * @tparam U The type of the value the node will hold
 */
template <class U> class Node {
public:
    Node(U item);
    void setValue(U item);
    void setNext(Node *nextPtr);
    Node<U> *getNext();
    U getValue();
private:
    Node<U> *next;
    U value;
};

/**
 * Constructor of a generic node
 * @tparam U The type of the value the node will hold
 * @param item The value the node will hold
 */
template <class U> Node<U>::Node(U item) {
  setValue(item);
  //By default the pointer to the next node is set to null
  setNext(nullptr);
}

/**
 * Sets the value of the node to the given item
 * @tparam U The type of the value the node will hold
 * @param item The new value the node will hold
 */
template <class U> void Node<U>::setValue(U item) {
  value = item;
}

/**
 * Sets the node's pointer to the next node to the given value
 * @tparam U The type of the value the node will hold
 * @param nextPtr The new pointer to the next node
 */
template <class U> void Node<U>::setNext(Node *nextPtr) {
  next = nextPtr;
}

/**
 * Returns the pointer of the node this node points to
 * @tparam U The type of the value the node will hold
 * @return The pointer of the node which this node points to
 */
template <class U> Node<U> *Node<U>::getNext() {
  return next;
}

/**
 * Returns the value this node contains
 * @tparam U The type of the value the node will hold
 * @return The value held by the node
 */
template <class U> U Node<U>::getValue() {
  return value;
}

#endif //RUBIKSCUBE_NODE_H
