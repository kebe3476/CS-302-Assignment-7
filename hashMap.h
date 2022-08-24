/*
CS 302 Assignment 7

Name: Keith Beauvais, 5005338658, CS 302 Assignment 7
*/
#include <string>
#include <cstdlib>

template <typename t1, typename t2>
class hashMap
{
public:

    struct hashPair
    {
        t1 key;
        t2 value;
        hashPair *link;
    };
    
    struct iteratorPair
    {
        t1 *key;
        t2 *value;
        iteratorPair *link;
    };

    class iterator
    {
    public:
        friend class hashMap;
        iterator();
        const iterator &operator++(int);
        bool operator==(const iterator &) const;
        bool operator!=(const iterator &) const;
        t1 first();
        t2 second();

    private:
        iterator(iteratorPair *);
        iteratorPair *element;
    };

    hashMap();
    ~hashMap();
    t2 &operator[](t1);
    iterator begin() const;
    iterator end() const;

private:

    void resize();
    int h(std::string) const;
    int items;
    int size;
    hashPair **table;
    iteratorPair * head;

};


//***************hashMap class***************
/* 
hashMap<t1, t2>::hashMap() - default constructor, sets the size to 5, items to 0, 
sets head with NULL, allocates the hash table table = new hashPair*[size] and 
then sets each element of table with NULL

return value - n/a
*/
template <typename t1, typename t2>
hashMap<t1, t2>::hashMap(){
    items = 0;
    size = 5;
    head = NULL;
    table = new hashPair * [size];
    for(int i =0; i<size; i++){
        table[i] = NULL;
    }
}
/* 
hashMap<t1, t2>::~hashMap() - destructor, deallocate the hash table (deallocate each 
linked list within table and then deallocate table), and then dellocate the linked list 
that head points to

return value - n/a
*/
template <typename t1, typename t2>
hashMap<t1, t2>::~hashMap(){
    
    while(head != NULL){
        iteratorPair * del;
        del = head;
        head = head -> link;
        del -> link = NULL;
        delete del;
    }
    
    for(int i = 0; i<size; i++){
        if(table[i]!=NULL){
            
            if(table[i]->link==NULL){
                delete table[i];
            }
            else if(table[i]->link->link==NULL){
                hashPair * del;     
                del =table[i];
                del = del -> link;
                del -> link = NULL;
                delete del;
                delete table[i];
            }
            
        }
    }
    delete [] table;
}
/* 
t2& hashMap<t1, t2>::operator[](t1 key) -  the bracket operator that performs 
the insert/find for the hash map:
    1. Checks the load factor, if the load factor is 50% or larger, then calls the 
    resize() function
    2. Calls the hash function and store the result into an integer
    3. Checks the location table[x]:
        a: Either this entry is empty, or it only contains 1 node, or it can contain 2 nodes
        b: determines whether the key exists in this entry or if the key needs to be
        inserted into this entry
        c: If the key exists (somewhere in this linked list), then return its value field
        d: If the key does not exist (in the linked list) and there is a vacancy (this location only contains 1 node), 
        creates the entry and creates an entry in linked list of entries that head points to and then returns the 
        value field of the entry.
        e: If a collision occurs (location contains 2 nodes and neither matches the key), then updates x, x = (x + 1) % size
        and try step 3 again

return value - returns the value field of the entry.
*/
template <typename t1, typename t2>
t2 & hashMap<t1, t2>::operator[](t1 key){
    double loadFactor;
    hashPair * newEntry;
    iteratorPair * newLinkedListEntry;
    loadFactor = static_cast<double>(items)/static_cast<double>(size);

    if(loadFactor >= .5){
        resize();
    }

    int x = h(key);

    while(true){
        if(table[x] == NULL){
            hashPair * newEntry;
            newEntry = new hashPair;
            table[x] = newEntry;
            newEntry->key = key;
            newEntry->value = t2();
            newEntry -> link = NULL;

            iteratorPair * newLinkedListEntry;
            newLinkedListEntry = new iteratorPair;
            newLinkedListEntry -> link = head;
            newLinkedListEntry -> key = &(newEntry->key);
            newLinkedListEntry -> value = &(newEntry->value);
            head = newLinkedListEntry;
            
        
            return newEntry -> value; 
        }

        hashPair * i;
        i = table[x];

        if(table[x] != NULL){
            
            if(i->key == key){
                return i->value;
            }

            if(i->link == NULL){
            hashPair * newEntry;
            newEntry = new hashPair;
            i->link = newEntry;
            newEntry->key = key;
            newEntry->value = t2();
            newEntry -> link = NULL;
            
            iteratorPair * newLinkedListEntry;
            newLinkedListEntry = new iteratorPair;
            newLinkedListEntry -> link = head;
            newLinkedListEntry -> key = &(newEntry->key);
            newLinkedListEntry -> value = &(newEntry->value);
            head = newLinkedListEntry;
            items++;

            return newEntry -> value; 
            }

            i=i->link;

            if(i->key == key){
                return i->value;
            }
            
            if(i->link == NULL){
                x=(x+1)%size;
                //delete i; 
            }
        }
    }
    
    
}
/* 
typename hashMap<t1, t2>::iterator hashMap<t1, t2>::begin() - returns an iterator that points to the first 
node in the linked list that head points to

return value - returns an iterator that points to the first node
*/
template <typename t1, typename t2>
typename hashMap<t1, t2>::iterator hashMap<t1, t2>::begin() const{
    return iterator(head);
}
/* 
typename hashMap<t1, t2>::iterator hashMap<t1, t2>::end() - returns a null iterator

return value - returns a null iterator
*/
template <typename t1, typename t2>
typename hashMap<t1, t2>::iterator hashMap<t1, t2>::end() const{
    return iterator(NULL);
}
/* 
void hashMap<t1, t2>::resize() - doubles the size of the hash table, needs to remap all the elements
from the original table to a larger table

return value - n/a
*/
template <typename t1, typename t2>
void hashMap<t1, t2>::resize(){
    int oldTableSize, newTableSize;
    oldTableSize = size;
    newTableSize = oldTableSize * 2;
    size = newTableSize;

    hashPair ** oldTable = table;
    table = new hashPair * [newTableSize];
    

    // New Table
    for(int i = 0; i<newTableSize; i++){
        table[i] = NULL; // setting default value
    } 
    //deallocating linked list iterator pair

    while(head != NULL){
            iteratorPair * del;
            del = head;
            head = head -> link;
            del -> link = NULL;
            delete del;
    }

    items = 0;
    // Rehashing new table
    for(int i = 0; i<oldTableSize; i++){
        if(oldTable[i]!= NULL){
            (*this)[oldTable[i]->key]=oldTable[i]->value;
            if(oldTable[i]->link != NULL){
                 (*this)[oldTable[i]->link->key]=oldTable[i]->link->value;
            }
        }
    }

    //deleting old Table
    
    for(int i = 0; i<oldTableSize; i++){
        if(oldTable[i]!=NULL ){
            
            if(oldTable[i]->link==NULL){
                delete oldTable[i];
            }
            else{
                hashPair * del;     
                del = oldTable[i];
                del = del -> link;
                del -> link = NULL;
                delete del;
                delete oldTable[i];
            }
            
        }
       
    }
    delete [] oldTable;

}
/* 
int hashMap<t1, t2>::h(std::string key) - hash function specific for when the key is of type std::string, 
this function adds up all the ASCII values of the string then mods it by the size and returns this value


return value - returns an int of where the index should be at x after adding up ascii values.
*/
template <typename t1, typename t2>
int hashMap<t1, t2>::h(std::string key) const{
    int asciiSum = 0;

    for(int i = 0; i < key.size(); i++){
        asciiSum += key[i];
    }
    asciiSum = asciiSum % size;

    return asciiSum;
}
//***************Iterator class***************
/* 
int hashMap<t1, t2>::h(std::string key) - default constructor that sets elements with NULL


return value - n/a
*/
template <typename t1, typename t2>
hashMap<t1, t2>::iterator::iterator(){
    element = NULL;
}
/* 
hashMap<t1, t2>::iterator::iterator(iteratorPair * p) - constructor that sets elements with the address in p

return value - n/a
*/
template <typename t1, typename t2>
hashMap<t1, t2>::iterator::iterator(iteratorPair * p){
    element = p;
}
/* 
const typename hashMap<t1, t2>::iterator& hashMap<t1, t2>::iterator::operator++(int) - postfix operator 
that moves the iterator over to the next node in the linked list


return value - returns neighbor in linked list
*/
template <typename t1, typename t2>
const typename hashMap<t1, t2>::iterator& hashMap<t1, t2>::iterator::operator++(int){
    element = element -> link;
    return *this;
}
/* 
bool hashMap<t1, t2>::iterator::operator==(const hashMap<t1, t2>::iterator& rhs) - compares if two 
iterators point to the same node, compares this->element with rhs.element, then it returns true if 
they are the same address or false otherwise


return value - returns either true or false
*/
template <typename t1, typename t2>
bool hashMap<t1, t2>::iterator::operator==(const hashMap<t1, t2>::iterator& rhs) const{
    if(this->element == rhs.element){
        return true;
    }
    else{
        return false;
    }
}
/* 
bool hashMap<t1, t2>::iterator::operator!=(const hashMap<t1, t2>::iterator& rhs) - compares if two 
iterators point to the same node, compares this->element with rhs.element, then it returns false if 
they are the same address or true otherwise


return value - returns either true or false
*/
template <typename t1, typename t2>
bool hashMap<t1, t2>::iterator::operator!=(const hashMap<t1, t2>::iterator& rhs) const{
    if(this->element != rhs.element){
        return true;
    }
    else{
        return false;
    }
}
/* 
t1 hashMap<t1, t2>::iterator::first() -returns the key field of the node, 
since each node in the linked list contains a pointer to a key in the existing table,


return value - returns the key field of the node
*/
template <typename t1, typename t2>
t1 hashMap<t1, t2>::iterator::first(){
    return *(element->key);
}
/* 
t2 hashMap<t1, t2>::iterator::second() - returns the value field of the node, 
since each node in the linked list contains a pointer to a key in the existing table,


return value - returns the value field of the node
*/
template <typename t1, typename t2>
t2 hashMap<t1, t2>::iterator::second(){
    return *(element->value);
}