/* vim: set ts=2 noet*/
#include <iostream>
#include <vector>
#include <list>
#include <list>
#include <string>
#include <algorithm>
#include <limits.h>
#include <assert.h>

class Person {
	public :
		std::string name;
		int floorDesired;
		bool remove=false;
		Person(std::string name_, int floorDesired_):name(name_), floorDesired(floorDesired_) { }
		bool operator<(const Person & other) const  {
			return 0< name.compare(other.name);
		}
};

class Lift {
	public:
		std::list<Person> peopleRiding;
		unsigned floors;
		unsigned curfloor = 0;
		unsigned scheduledFloor = 0;
		bool scheduled_=false;

		bool stopped = true;
		std::list<int> stops;
		bool scheduled() const  {
			return scheduled_;
		}
};


class Floor {
	public:
		std::list<Person> peopleWaiting;
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
		unsigned entryTime;
		unsigned initialFloor;
		unsigned targetFloor;
		AddPerson(std::string name_, unsigned entryTime_, unsigned initialFloor_, unsigned targetFloor_)  :
			name(name_), entryTime(entryTime_), initialFloor(initialFloor_), targetFloor(targetFloor_)
		{

		}
};

static void addPeopleIntoBuilding(unsigned curTime, std::list <AddPerson > &addPeople, Building &building) {
	while (addPeople.size() > 0 && addPeople.front().entryTime == curTime) {
		AddPerson & curPerson = addPeople.front();
		std::cout << "at time " << curTime <<  " adding " << curPerson.name << " floor " << curPerson.initialFloor << " desired floor " << curPerson.targetFloor << std::endl;
		building.floors[curPerson.initialFloor].peopleWaiting.push_back(Person(curPerson.name, curPerson.targetFloor));
		addPeople.pop_front();
	}
}

void scheduleLifts(Building & building) {
	for(unsigned curFloorNum = 0; curFloorNum < building.floors.size(); curFloorNum++) {
		Floor &curFloor =  building.floors[curFloorNum];
		if (0 == curFloor.peopleWaiting.size() ) {
			continue;
		}
		std::list<Person>::iterator itr = curFloor.peopleWaiting.begin();
		std::list<Person>::iterator eitr = curFloor.peopleWaiting.end();
		while(itr != eitr) {
			int closestLift = -1;
			unsigned cheapest_cost = INT_MAX;
			for (unsigned curLiftNum =0;curLiftNum < building.lifts.size(); curLiftNum++) {
				Lift &curLift= building.lifts[curLiftNum];
				unsigned distance;
				if (curLift.scheduled() )
					continue;
				if (curLift.curfloor > curFloorNum)
					distance = curLift.curfloor- curFloorNum;
				else
					distance = curFloorNum - curLift.curfloor;
				if (distance < cheapest_cost) {
					cheapest_cost = distance;
					closestLift = curLiftNum;
				}
			}
			if (closestLift != -1) {
				Lift &curLift= building.lifts[closestLift];
				if (cheapest_cost == 0 ) {
					std::cout << itr->name << " boards lift " << closestLift << std::endl;
					curLift.peopleRiding.push_back(*itr);
					itr->remove=true;
					curLift.scheduled_ =true;
					curLift.scheduledFloor=itr->floorDesired;
				}
				else {
					curLift.scheduled_ =true;
					curLift.scheduledFloor=curFloorNum;
					std::cout << closestLift << " going to  " << curFloorNum << std::endl;
				}
			}
			itr++;
		}
		curFloor.peopleWaiting.remove_if ([&](Person & p ) {
				return p.remove;
		});
	}
}
void moveLifts(Building &building) {
	std::vector<Lift>::iterator itr = building.lifts.begin();
	std::vector<Lift>::iterator eitr = building.lifts.end();
	int i=0;
	while (itr != eitr) {
		std::cout<< "lift " << i << " at " <<itr->curfloor << " moved to ";
		if (itr->curfloor < itr->scheduledFloor)
			itr->curfloor++;
		if (itr->curfloor > itr->scheduledFloor)
			itr->curfloor--;
		if (itr->curfloor == itr->scheduledFloor) {
			itr->scheduled_=false;
		}
		std::cout<< itr->curfloor << std::endl;
		itr->peopleRiding.remove_if ([&](Person & p ) {
			if (itr->curfloor == p.floorDesired)
					std::cout << p.name << " exits lift " << i << std::endl;
			return itr->curfloor == p.floorDesired;
		});
		i++;
		itr++;
	}
}

int main(void) {
	std::cout << "hello world" << std::endl;
	Building building;
	building.floors.resize(5);
	building.lifts.resize(2);

	unsigned maxTime = 10;
	std::list<AddPerson> addPeople;
	addPeople.push_back(AddPerson("Jane", 0, 0, 4));
	addPeople.push_back(AddPerson("Joe", 4, 2, 3));

	for (unsigned curTime = 0; curTime < maxTime; curTime++) {
		addPeopleIntoBuilding(curTime, addPeople, building);
		scheduleLifts(building);
		moveLifts(building);
	}
	return 0;
}

