/* vim: set ts=2 noet*/
#include <iostream>
#include <vector>
#include <list>
#include <list>
#include <set>
#include <string>
#include <algorithm>
#include <limits.h>
#include <assert.h>

class Person {
	public :
		std::string name;
		unsigned floorDesired;
		bool remove=false;
		Person(std::string name_, int floorDesired_):name(name_), floorDesired(floorDesired_) { }
		bool operator<(const Person & other) const  {
			return 0< name.compare(other.name);
		}
};

class Lift {
	public:
		Lift() {
			id = nextId++;
		}
		std::list<Person> peopleRiding;
		unsigned curfloor = 0;
		unsigned scheduledFloor = 0;
		bool scheduled_=false;
		bool stopped = true;
		std::set<int> stops;
		bool scheduled() const  {
			return scheduled_;
		}
		void personBoards(const Person & person) {
			std::cout << person.name << " boards lift " << id << std::endl;
			peopleRiding.push_back(person);
			scheduled_ =true;
			scheduledFloor=person.floorDesired;
			stops.insert(person.floorDesired);
		}
		void schedule(unsigned pickupFloor, unsigned dropFloor) {
			scheduled_ =true;
			scheduledFloor=pickupFloor;
			std::cout << id << " going to  " <<scheduledFloor << std::endl;
			stops.insert(dropFloor);
		}
		void moveLift(std::list<Person> & peopleExited) {
			if (scheduled_) {
				unsigned prev= curfloor ;
				if (curfloor < scheduledFloor)
					curfloor++;
				if (curfloor > scheduledFloor)
					curfloor--;
				if (curfloor == scheduledFloor) {
					scheduled_=false;
				}
				if (prev != curfloor) {
					std::cout<< "lift " << id << " at " <<prev << " moved to ";
					std::cout<< curfloor << std::endl;
				}
				peopleRiding.remove_if ([&](Person & p ) {
						if (curfloor == p.floorDesired) {
							std::cout << p.name << " exits lift " << id << std::endl;
							peopleExited.push_back(p);
						}
						stops.erase(curfloor);
						return curfloor == p.floorDesired;
						});
			}
		}
	private:
		int id;
		static int nextId;

};
int Lift::nextId=0;


class Floor {
	public:
		std::list<Person> peopleWaiting;
		Floor(): peopleWaiting() {};
};

class Building {
	public:
		Building & setFloors(int i) {
			floors.resize(i);
			return *this;
		}
		Building & setLifts(int i) {
			lifts.resize(i);
			return *this;
		}
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
static int calcDistance(int a, int b) {
	if (a > b)
		return a - b;
	return b - a;

}
static int findClosestLift(Person &curPerson, unsigned curFloorNum, std::vector<Lift> &lifts ) {
	int closestLift = -1;
	unsigned cheapest_cost = INT_MAX;
	for (unsigned curLiftNum =0;curLiftNum < lifts.size(); curLiftNum++) {
		Lift &curLift= lifts[curLiftNum];
		if (curLift.scheduled() ) {
			std::cout << "lift " << curLiftNum << " already going to [";
			std::for_each (curLift.stops.begin(), curLift.stops.end() , [] (int i){ std::cout << i << " ";});
			std::cout << "] "<< std::endl;
			if (curLift.scheduledFloor == curFloorNum && curLift.stops.count(curPerson.floorDesired) > 0 )
				return -1;
			continue;
		}
		unsigned distance = calcDistance(curLift.curfloor, curFloorNum);
		if (distance < cheapest_cost) {
			cheapest_cost = distance;
			closestLift = curLiftNum;
		}
	}
	return closestLift;
}

static void scheduleLifts(Building & building) {
	for(unsigned curFloorNum = 0; curFloorNum < building.floors.size(); curFloorNum++) {
		Floor &curFloor =  building.floors[curFloorNum];
		if (0 == curFloor.peopleWaiting.size() ) {
			continue;
		}
		std::list<Person>::iterator itr = curFloor.peopleWaiting.begin();
		std::list<Person>::iterator eitr = curFloor.peopleWaiting.end();
		while(itr != eitr) {
			int closestLift = findClosestLift(*itr, curFloorNum, building.lifts);
			if (closestLift != -1) {
				Lift &curLift= building.lifts[closestLift];
				unsigned cheapest_cost= calcDistance(curLift.curfloor, curFloorNum);
				if (cheapest_cost == 0 ) {
					curLift.personBoards(*itr);
					itr->remove=true;
				}
				else {
					curLift.schedule(curFloorNum,itr->floorDesired);
				}
			}
			itr++;
		}
		curFloor.peopleWaiting.remove_if ([&](Person & p ) {
				return p.remove;
		});
	}
}

static void moveLifts(Building &building,std::list<Person> & peopleExited) {
	std::for_each(building.lifts.begin(), building.lifts.end(), [&peopleExited](Lift &l) { l.moveLift(peopleExited);});
}

static void loadPeople(std::list<AddPerson> & addPeople) {
	char name[21];
	int entertime;
	int startfloor;
	int endfloor;
	while (EOF != std::scanf("%20s %d %d %d\n", name, &entertime, &startfloor, &endfloor)) {
		addPeople.push_back(AddPerson(name, entertime, startfloor, endfloor));
	}
}

int main(void) {
	Building building;
	building.setFloors(20).setLifts(2);
	unsigned maxTime = 60;
	std::list<AddPerson> addPeople;
	loadPeople (addPeople);
	int num_people = addPeople.size();
	std::list<Person> peopleExited;
	for (unsigned curTime = 0; curTime < maxTime; curTime++) {
		addPeopleIntoBuilding(curTime, addPeople, building);
		scheduleLifts(building);
		moveLifts(building, peopleExited);
	}
	std::cout << "input " << num_people <<" " << peopleExited.size() << std::endl;
	return 0;
}

