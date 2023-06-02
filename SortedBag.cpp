#include "SortedBag.h"
#include "SortedBagIterator.h"

SortedBag::SortedBag(Relation r) {
    // theta(1)
    // Constructor for SortedBag class
    root = nullptr;
    relation = r;
    currentSize = 0;
}

void SortedBag::add(TComp e) {
    // Best Case: theta(1), the element to be added is either the root or the bag is empty
    // Worst Case: theta(n), all the elements have only right or left children and our element has to be added at the end
    // Total: O(n)
    // Adds an element to the sorted bag
    currentSize++; // Increase the size of the bag
    bool ok = false;
    Node* n = new (Node); // Create a new node to store the element
    n->parent = nullptr; // Initialize parent pointer to null
    n->leftChild = nullptr; // Initialize left child pointer to null
    n->rightChild = nullptr; // Initialize right child pointer to null
    n->value = e; // Set the value of the node to the element
    n->frequency = 1; // Set the frequency of the node to 1

    if (root == nullptr){
        // If the bag is empty, make the new node the root
        root = n;
        return;
    }

    Node* search = root;
    while (!ok)
    {
        if (e == search->value) {
            // If the element already exists in the bag, increase its frequency
            search->frequency++;
            ok = true; // Element added successfully
            delete n; // Delete the unused node
            n = nullptr; // Set the unused node pointer to null
        }
        else if (relation(search->value, e)) {
            // If the element is greater than the current node value
            if (search->rightChild == nullptr) {
                // If the current node doesn't have a right child, add the new node as the right child
                search->rightChild = n;
                n->parent = search;
                ok = true; // Element added successfully
            }
            else {
                // Move to the right child and continue searching
                search = search->rightChild;
            }
        }
        else if (relation(e, search->value)) {
            // If the element is less than the current node value
            if (search->leftChild == nullptr) {
                // If the current node doesn't have a left child, add the new node as the left child
                search->leftChild = n;
                n->parent = search;
                ok = true; // Element added successfully
            }
            else {
                // Move to the left child and continue searching
                search = search->leftChild;
            }
        }
    }
}


bool SortedBag::remove(TComp e) {
    // Best Case: theta(1), the bag is empty
    // Worst Case: theta(n), all the elements have only right or left children and our element has to be added at the end
    // Total: O(n)
    // Removes an element from the sorted bag

    // Check if the element exists in the bag
    if (!search(e))
        return false; // If the element doesn't exist, return false

    currentSize--; // Decrease the size of the bag

    Node* search = root; // Start searching from the head of the bag

    while (true) {
        // Iterate until the element is found or the loop is broken

        // If the element is found
        if (e == search->value) {

            // If the element has a frequency greater than 1, decrease the frequency
            if (search->frequency > 1) {
                search->frequency--;
                return true; // Element removed successfully
            }
            else {
                // If the element has a frequency of 1

                // If the element is a leaf node (has no children)
                if (search->leftChild == nullptr && search->rightChild == nullptr) {

                    // If the element is the root, set the root to null
                    if (search == root) {
                        root = nullptr;
                    }
                    // If the element is a left child
                    else if (search == search->parent->leftChild)
                        search->parent->leftChild = nullptr;
                    // If the element is a right child
                    else
                        search->parent->rightChild = nullptr;
                }

                    // If the element has no left child
                else if (search->leftChild == nullptr) {

                    // If the element is the root, set the root to the right child
                    if (search == root) {
                        root = search->rightChild;
                        search->rightChild->parent = nullptr;
                    }
                    // If the element is a left child
                    else if (search == search->parent->leftChild)
                        // Set the right child of our elem to be the left child of the elem's parent
                        search->parent->leftChild = search->rightChild;
                    // If the element is a right child
                    else if (search == search->parent->rightChild)
                        // Set the right child of our elem to be the right child of the elem's parent
                        search->parent->rightChild = search->rightChild;
                }

                    // If the element has no right child
                else if (search->rightChild == nullptr) {

                    // If the element is the root, set the root to the left child
                    if (search == root) {
                        root = search->leftChild;
                        search->leftChild->parent = nullptr;
                    }
                    // If the element is a left child
                    else if (search == search->parent->leftChild)
                        // Set the left child of our elem to be the left child of the elem's parent
                        search->parent->leftChild = search->leftChild;
                    // If the element is a right child
                    else if (search == search->parent->rightChild)
                        // Set the left child of our elem to be the right child of the elem's parent
                        search->parent->rightChild = search->leftChild;
                }

                    // If the element has both left and right children
                else {
                    // Find the highest left child of the element
                    Node* searchHighestLeft = search->leftChild;
                    while (searchHighestLeft->rightChild != nullptr)
                        searchHighestLeft = searchHighestLeft->rightChild;

                    // Replace the element with the highest left child
                    searchHighestLeft->parent->rightChild = searchHighestLeft->leftChild;

                    // If the element is the root
                    if (search == root) {
                        root = searchHighestLeft;

                        // Update parent-child relationships
                        if (searchHighestLeft->leftChild != nullptr)
                            searchHighestLeft->leftChild->parent = searchHighestLeft->parent;
                        searchHighestLeft->parent = nullptr;
                        searchHighestLeft->rightChild = search->rightChild;
                        if (search->rightChild != nullptr)
                            search->rightChild->parent = searchHighestLeft;
                        searchHighestLeft->leftChild = search->leftChild;
                        if (search->leftChild != nullptr)
                            search->leftChild->parent = searchHighestLeft;
                        search->leftChild = nullptr;
                        search->rightChild = nullptr;
                        search->parent = nullptr;
                        delete search;
                        return true; // Element removed successfully
                    }
                        // If the element is the left child of its parent
                    else if (search->parent->leftChild == search) {
                        search->parent->leftChild = searchHighestLeft;
                    }
                        // If the element is the right child of its parent
                    else if (search->parent->rightChild == search) {
                        search->parent->rightChild = searchHighestLeft;
                    }

                    // If the left child of the element is the highest left child
                    if (search->leftChild == searchHighestLeft) {
                        searchHighestLeft->rightChild = search->rightChild;
                        searchHighestLeft->parent = search->parent;
                    }
                    else {
                        // Update parent-child relationships
                        if (searchHighestLeft->leftChild != nullptr)
                            searchHighestLeft->leftChild->parent = searchHighestLeft->parent;
                        searchHighestLeft->parent = search->parent;
                        search->parent->rightChild = searchHighestLeft;
                        searchHighestLeft->rightChild = search->rightChild;
                        search->rightChild->parent = searchHighestLeft;
                        searchHighestLeft->leftChild = search->leftChild;
                        search->leftChild->parent = searchHighestLeft;
                    }
                }

                // Clear references and delete the element
                search->leftChild = nullptr;
                search->rightChild = nullptr;
                search->parent = nullptr;
                delete search;
                return true; // Element removed successfully
            }
        }
            // If the element is greater than the current node value, move to the right child
        else if (relation(search->value, e)) {
            search = search->rightChild;
        }
            // If the element is less than the current node value, move to the left child
        else if (relation(e, search->value)) {
            search = search->leftChild;
        }
    }
}

bool SortedBag::search(TComp elem) const {
    // Best Case: theta(1), the bag is empty or the element is the root
    // Worst Case: theta(n), all the elements have only right or left children and our element is the leaf
    // Total: O(n)
    // Searches for an element in the sorted bag
    Node* search = root;

    if (root == nullptr)
        return false; // If the bag is empty, return false

    while (true)
    {
        if (elem == search->value) {
            return true; // Element found
        }
        else if (relation(search->value, elem)) {
            // If the element is greater than the current node value
            if (search->rightChild == nullptr) {
                return false; // Element not found
            }
            else {
                search = search->rightChild; // Move to the right child and continue searching
            }
        }
        else if (relation(elem, search->value)) {
            // If the element is less than the current node value
            if (search->leftChild == nullptr) {
                return false; // Element not found
            }
            else {
                search = search->leftChild; // Move to the left child and continue searching
            }
        }
    }
}

int SortedBag::nrOccurrences(TComp elem) const {
    // Returns the number of occurrences of an element in the sorted bag
    if (!search(elem))
        return 0; // If the element doesn't exist in the bag, return 0

    Node* search = root;
    while (true)
    {
        if (elem == search->value) {
            return search->frequency; // Return the frequency of the element
        }
        else if (relation(search->value, elem)) {
            // If the element is greater than the current node value
            search = search->rightChild; // Move to the right child
        }
        else if (relation(elem, search->value)) {
            // If the element is less than the current node value
            search = search->leftChild; // Move to the left child
        }
    }
}

int SortedBag::size() const {
    // theta(1)
    // Returns the size of the sorted bag
    return currentSize;
}

bool SortedBag::isEmpty() const {
    // theta(1)
    // Checks if the sorted bag is empty
    return currentSize == 0;
}

SortedBagIterator SortedBag::iterator() const {
    // theta(1)
    // Returns an iterator for the sorted bag
    return SortedBagIterator(*this);
}

SortedBag::~SortedBag() {
    // Destructor for SortedBag class
    Node* deleteNode = root;
    while (deleteNode != root) {
        if (deleteNode->leftChild != nullptr) {
            deleteNode = deleteNode->leftChild;
        }
        else if (deleteNode->rightChild != nullptr) {
            deleteNode = deleteNode->rightChild;
        }
        else {
            Node* toDelete = deleteNode;
            deleteNode = deleteNode->parent;
            if (deleteNode->leftChild == toDelete)
                deleteNode->leftChild = nullptr;
            else
                deleteNode->rightChild = nullptr;
            delete toDelete;
        }
    }
}