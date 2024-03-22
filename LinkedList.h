// ASU CSE310 Spring 2024 Assignment #1
// Name: Daniel Budavari
// ASU ID: 1224612207
// Description: this contains the code for all the methods and constuctors/lists:
    //findCar, addCar, removeByVin, removeByModelAndMake, changeCarInfo, updateCarPrice, printCarListByMake, printCarList.

#include <iostream>
#include <iomanip>
#include <string>

//splitting words in changeCarInfo
#include <sstream>
#include <vector>

using namespace std;

//Car represents a Car information
struct Car
{
    string model, make;
    int vin;            //vehicle identification number
    double price;
    struct Car* next;
};

//class LinkedList will contains a linked list of Cars
class LinkedList
{
private:
    struct Car* head;

public:
    LinkedList();
    ~LinkedList();
    bool findCar(int aVin);
    bool addCar(string model, string make, int vin, double price);
    bool removeByVin(int aVin);
    bool removeByModelAndMake(string model, string make);
    bool changeCarInfo(int aVin, string newModelAndMake);
    bool updateCarPrice(int aVin, double newPrice);
    void printCarListByMake(string make);
    void printCarList();
};

//Constructor to initialize an empty linked list
LinkedList::LinkedList() {
    head = NULL;
}

//Destructor
//Before termination, the destructor is called to free the associated memory occupied by the existing linked list.
//It deletes all the nodes including the head and finally prints the number of nodes deleted by it.
//Return value: Prints the number of nodes deleted by it.
LinkedList::~LinkedList() {
    
    int carCount = 0;
   
    while (head != NULL) {
            struct Car* delList = head;
            head = head->next;
            delete delList;
            carCount++;
    }
    cout << "The number of deleted Car is: " << carCount << "\n";
}

//A function to identify if the parameterized Car is inside the LinkedList or not.
//Return true if it exists and false otherwise.
bool LinkedList::findCar(int aVin) {
   
    struct Car* carList = head;

    while(carList != NULL) {
        if (aVin == carList->vin) {
            return true;
        } 
        else {
            carList = carList->next;
        }
    }
    return false;
}

//Creates a new Car and inserts it into the list at the right place.
//It also maintains an alphabetical ordering of Cars by their make and model, i.e.
//first by make, if two cars have same makes, then we will list them by model. Note: each Car
//has a unique vin, the vehicle identification number, but may have the same make and model.
//In case two Cars have same make and model, they should be inserted according to the
//increasing order of their vins.
//Return value: true if it is successfully inserted and false in case of failures.
bool LinkedList::addCar(string model, string make, int vin, double price) {
    //create new car using inputted info
    Car* addedCar = new Car{model, make, vin, price, NULL};
    
    //pointers for traversal
    Car* curr = head;
    Car* prev = NULL;

    // If the list is empty adds car to the head position
    if (head == NULL) {
        addedCar->next = head;
        head = addedCar;
        return true;
    }
    //if the head needs to be replaced by the new car, ie make is different
    if (head->make > addedCar->make) {
        head = addedCar;
        addedCar->next = curr;
        return true;
    }

    //checks if makes are the same, if so then checks if model are the same, then checks which order the car should be in according to vin
    if (head->make == addedCar->make) {
        if (head->model == addedCar->model) {
            if (head->vin > addedCar->vin) {
                head = addedCar;
                addedCar->next = curr;
                return true;
            }
        }
        else {
            //if models are different inserts accordingly
            if (head->model > addedCar->model) {
                head = addedCar;
                addedCar->next = curr;
                return true;
            }
        }
    }

    //Find correct postion
    while (curr != nullptr && 
        (make > curr->make || 
            (make == curr->make && (model > curr->model || 
                (model == curr->model && vin > curr->vin))) )) {
        prev = curr;
        curr = curr->next;
    }

    // Check if a Car with the same vin already exists, deletes the car and returns false
    if (curr != NULL && vin == curr->vin) {
        delete addedCar;
        return false;
    }

    //If all is valid, adds the car to the newest proper position
    prev->next = addedCar;
    addedCar->next = curr;
    return true;
}

//Removes the specified Car from the list, releases the memory and updates pointers.
//Return true if it is successfully removed, false otherwise.
bool LinkedList::removeByVin(int aVin) {
    //navigation pointers
    Car* curr = head;
    Car* prev = NULL;
    Car* hold; //temporary hold pointer variables
    
    //list is empty no car can be removed
    if (head == NULL) {
        
        return false;
    }

    //if the car is at the head position
    if (aVin == head->vin) {
        hold = head;
        head = head->next;
        delete hold; //cleans garbage
        
        return true;
    }
    //searches through list to find the car
    while (curr != NULL) {
        if (aVin == curr->vin) {
            prev->next = curr->next; //deletes the specified car
            delete curr;            //cleans up garbage

            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}


//Removes the given Car from the list, releases the memory and updates pointers.
//Return true if it is successfully removed, false otherwise. Note: all Cars with
//the same model and make should be removed from the list.

//FIX!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool LinkedList::removeByModelAndMake(string model, string make) {
    
    Car* prev = NULL; //navigation pointer
    Car* hold;        //temp hold pointer
    int carsRemoved = 0; //variable for return false or true sequence

    //list is empty
    if (head == NULL) {
        return false;
    }

    //the head node needs to be removed
    while (make == head->make && model == head->model) {
        
        hold = head;
        head = head->next;
        delete hold; //garbage clean up
        carsRemoved++;
        
        if (head == NULL) { //if the list is now empty from removing the cars
            return true;
        }
    }
    //if the cars to remove are not the head
    Car* curr = head;
    while (curr != NULL) {
        //found the car
        if (make == curr->make && model == curr->model) {
            prev->next = curr->next;
            curr = prev->next;
            carsRemoved++;
            continue; //if there are more cars of the same type to remove
        }
        prev = curr;
        curr = curr->next;
    }
    //return cases determined by our int, this is because we return after possible multiple cars are removed
    if (carsRemoved != 0) {
        return true;
    }
    return false;
}

//Modifies the data of the given Car. Return true if it modifies successfully and
//false otherwise. Note: after changing a Car model and make, the linked list must still
//maintain the alphabetical order.
bool LinkedList::changeCarInfo(int aVin, string newModelAndMake) {

    Car* carList = head;
    int oldVin = 0;
    int oldPrice = 0;

    std::string newModel;
    std::string newMake;
    std::istringstream iss(newModelAndMake);
    //split newModelAndMake string to get model and make accordingly

    if (iss >> newModel) {
        if (iss >> newMake) {
            while (carList != NULL) {
                if (carList->vin == aVin) {
                    //Variables to hold the price and vin of the car we are updating
                    oldPrice = carList->price;
                    oldVin = carList->vin;

                    //call removeByVin to allow us to insert the updated car info properly
                    removeByVin(aVin);
                    //add Car using the old info of the car and the new info
                    addCar(newModel, newMake, oldVin, oldPrice);
                    return true;
                }
                carList = carList->next; //else continue searching
            }
        }
    }
    return false;
}

bool LinkedList::updateCarPrice(int aVin, double newPrice) {
   
    Car* carList = head;
    
    while (carList != NULL) {
        if (carList->vin == aVin) {
            carList->price = newPrice; //sets the desired car with a new price
            return true;
        }
        carList = carList->next;
    }
    return false;
}

//Prints all Cars in the list with the same make.
void LinkedList::printCarListByMake(string make) {
    
   Car* temp = head;
   int carCount = 0;

   if (head == NULL) {
       cout << "The list is empty\n"; //if the list is empty
       return;
   }
   while(temp != NULL) {
       if(temp->make == make) { //if the car matches the desired make
             cout << left << setw(12) << temp->model
                << left << setw(12) << temp->make
                << right << setw(8) << temp->vin
                << setw(10) << fixed << setprecision(2) << temp->price << "\n";
             carCount++;
       } 
       temp = temp->next;
   }
   if (carCount == 0) {
       cout << "No Cars with the specified make found.\n";
   }
}


//Prints all Cars in the linked list starting from the head node.
void LinkedList::printCarList() {
    
    Car* temp = head;

    if (head == NULL) { //if list is empty
        cout << "The list is empty\n";
        return;
    }
    while (temp != NULL) { //displays all the cars
        cout << left << setw(12) << temp->model
            << left << setw(12) << temp->make
            << right << setw(8) << temp->vin
            << setw(10) << fixed << setprecision(2) << temp->price << "\n";
        temp = temp->next;
    }
} 