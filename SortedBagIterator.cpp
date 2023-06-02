#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    // Constructor for the iterator, initializes the iterator with the given SortedBag

    currentFrequency = 1; // Initialize the current frequency to 1
    cnode = bag.root; // Set the current node to the root of the bag

    if (cnode != nullptr) {
        // If the current node is not nullptr (i.e., the bag is not empty)

        // Move to the leftmost node to find the first element in the sorted order
        while (cnode->leftChild != nullptr)
            cnode = cnode->leftChild;
    }
}

TComp SortedBagIterator::getCurrent() {
    // Get the value of the current element

    if (cnode == nullptr)
        throw exception(); // If the current node is nullptr, throw an exception

    return cnode->value; // Return the value of the current node
}

bool SortedBagIterator::valid() {
    // Check if the iterator is valid

    if (cnode == nullptr)
        return false; // If the current node is nullptr, the iterator is not valid
    return true; // Otherwise, the iterator is valid
}

void SortedBagIterator::next() {
    // Move the iterator to the next element

    if (cnode == nullptr)
        throw exception(); // If the current node is nullptr, throw an exception

    if (currentFrequency < cnode->frequency) {
        // If the current frequency is less than the frequency of the current node

        currentFrequency++; // Increment the current frequency
        return;
    }

    currentFrequency = 1; // Reset the current frequency to 1

    if (cnode->rightChild != nullptr) {
        // If the current node has a right child

        // Move to the leftmost node of the right subtree to find the next element in the sorted order
        cnode = cnode->rightChild;
        while (cnode->leftChild != nullptr)
            cnode = cnode->leftChild;
        return;
    }

    if (cnode != bag.root && cnode->parent->leftChild == cnode) {
        // If the current node is not the root and it is the left child of its parent

        // Move to the parent node
        cnode = cnode->parent;
        return;
    }

    if (cnode != bag.root && cnode->parent->rightChild == cnode) {
        // If the current node is not the root and it is the right child of its parent

        // Move up the tree until reaching a node that is the left child of its parent
        while (cnode != bag.root && cnode->parent->rightChild == cnode) {
            cnode = cnode->parent;
        }

        if (cnode != bag.root) {
            // If the current node is not the root

            // Move to the parent node
            cnode = cnode->parent;
            return;
        }
    }

    if (cnode == bag.root) {
        // If the current node is the root

        cnode = nullptr; // Set the current node to nullptr, indicating the end of the iteration
        return;
    }
}

void SortedBagIterator::first() {
    // Move the iterator to the first element

    currentFrequency = 1; // Reset the current frequency to 1
    cnode = bag.root; // Set the current node to the root of the bag

    if (cnode != nullptr) {
        // If the current node is not nullptr (i.e., the bag is not empty)

        // Move to the leftmost node to find the first element in the sorted order
        while (cnode->leftChild != nullptr)
            cnode = cnode->leftChild;
    }
}