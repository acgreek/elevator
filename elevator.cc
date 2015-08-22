// vim: set ts=2
#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <algorithm>

class Person {
	public :
		std::string name;
		int floorDesired;
		Person(std::string name_, int floorDesired_):name(name_), floorDesired(floorDesired_) { }
		bool operator<(const Person & other) const  {
			return 0< name.compare(other.name);
		}	
};

class Lift {
	public:
		std::set<Person> peopleRiding;
		int floors;
		int curfloor = 0;
		bool stopped = true;
		std::set<int> stops;
};


class Floor {
	public:
		std::set<Person> peopleWaiting;
		Floor(): peopleWaiting() {};
};

class Building {
	public:
		std::vector<Lift> lifts;
		std::vector<Floor> floors;		
};

class AddPerson {
	public:
		std::string name;
		int entryTime;
		int initialFloor;
		int targetFloor;
		AddPerson(std::string name_, int entryTime_, int initialFloor_, int targetFloor_)  :
			name(name_), entryTime(entryTime_), initialFloor(initialFloor_), targetFloor(targetFloor_)
		{

		}	 
};

int main(void) {
	std::cout << "hello world" << std::endl;
	Building building;
	//building.floors.resize(5);
	
	int maxTime = 100;
	std::list<AddPerson> addPeople;
	addPeople.push_back(AddPerson("Jane", 0, 0, 4));

	for (int curTime = 0; curTime < maxTime; curTime++) {
		while (addPeople.size() > 0 && addPeople.front().entryTime == curTime) {
			AddPerson & curPerson = addPeople.front();
			building.floors[curPerson.initialFloor].peopleWaiting.insert(Person(curPerson.name, curPerson.targetFloor));
			addPeople.pop_front();
		}
		for(unsigned curFloor = 0; curFloor < building.floors.size(); curFloor++) {
			std::set<Person>::iterator itr = building.floors[curFloor].peopleWaiting.begin();
			std::set<Person>::iterator eitr = building.floors[curFloor].peopleWaiting.end();
			while(itr != eitr) {
				int closestLift = -1;	
				int cost = -1;	

				for (int curLift =0;curLift < building.lifts.size(); curLift++) {
					
				}
				itr++;
			}
			
		}	
	}
	return 0;
}

