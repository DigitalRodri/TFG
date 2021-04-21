#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <execution>
#include <set>
#include <unordered_set>
#include <map>
using namespace std;

// Static variables not dependant on classes
static int labourDays = 5;
static int startHour = 9;
static int finishHour = 17;
static int numberOfWeeks = 2;
static int hoursPerDay = finishHour - startHour;
static std::string lunchTime;
static int scrumTime = -1;
static std::tm startDate;
static std::tm finishDate;


class Task {
public:
	std::string name;
	std::vector<int> durations;
	std::vector<std::string> compatibilities;
	std::vector<std::string> dependencies;

public:
	Task() {
	}

	Task(std::string name, std::vector<int> durations, std::vector<std::string> compatibilities, std::vector<std::string> dependencies) {
		this->name = name;
		this->durations = durations; // In hours
		this->compatibilities = compatibilities;
		this->dependencies = dependencies;
	}

	void printTask() {
		std::cout << this->name << ' ';
		for (size_t j = 0; j < this->durations.size(); j++)
		{
			std::cout << this->durations.at(j) << ' ';
		}
		for (size_t j = 0; j < this->compatibilities.size(); j++)
		{
			std::cout << this->compatibilities.at(j) << ' ';
		}
		for (size_t j = 0; j < this->dependencies.size(); j++)
		{
			std::cout << this->dependencies.at(j) << ' ';
		}
		cout << endl;

	}

	bool dependsOn(std::string task) {

		for (size_t i = 0; i < dependencies.size(); i++)
		{
			if (dependencies.at(i).compare(task) == 0) {
				//cout << name << " depends on " << dependencies.at(i) << endl;
				return true;
			}
		}

		return false;
	}

};

class Employee {
public:
	std::string name;
	std::string role;;

public:
	Employee() {
	}

	Employee(std::string name, std::string role) {
		this->name = name;
		this->role = role;
	}

	void printEmployee() {

		std::cout << this->name << ' ';
		std::cout << this->role << ' ';
		cout << endl;

	}

	void setNull() {
		this->name = "";
		this->role = "";

	}
};

class Value {
public:
	short int id = 0;
	Employee employee;
	std::time_t date{};

public:
	Value() {
	}

	Value(int id, Employee employee, std::time_t date) {
		this->id = id;
		this->employee = employee;
		this->date = date;
	}

	void printValue() const {

		std::cout << this->id << ' ';
		std::cout << this->employee.name << ' ';
		struct std::tm hora;
		localtime_s(&hora, &date);
		std::cout << std::put_time(&hora, "%Y-%m-%dT%H:%M:%S") << endl;
		cout << endl;
	}

};

class Variable {
public:
	size_t id = 0;
	Task task;

public:
	Variable() {
	}

	Variable(int id, Task task) {
		this->id = id;
		this->task = task;
	}

	void printVariable() const {

		std::cout << "--- " << this->task.name << "-" << this->id << " ---" << endl;

		cout << endl;
	}

	int getTaskDuration(string role) {
		for (size_t i = 0; i < this->task.compatibilities.size(); i++)
		{
			//cout << "Comparing " << this->task.compatibilities.at(i) << " to " << role << endl;
			if (this->task.compatibilities.at(i).compare(role) == 0)
			{

				return this->task.durations.at(i);
			}


		}
		return 0;
	}

	time_t getTaskFinishTime(string role, time_t startTime) {

		// Auxiliary variables
		int taskDuration = getTaskDuration(role);
		size_t taskDays = taskDuration / hoursPerDay;
		int remainder = taskDuration % hoursPerDay;
		time_t timePlusDays = startTime;
		time_t timeAtStart;
		time_t timeAtFinish;
		time_t finishTime;

		// For printing and gettint specific measurements
		struct tm tmtime;
		struct tm tmtimeSCRUM;
		char printBuffer[26];


		/*if (task.name.compare("Character Design") == 0 && role.compare("CEO") == 0)
		{
			localtime_s(&tmtime, &startTime);
			strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
			cout << "Task starts at: " << printBuffer << " with duration " << taskDuration << endl;
		}*/


		// If the task spans more than one labour day, we advance the days directly and add the remaining hours
		if (taskDays != 0)
		{
			timePlusDays = timePlusDays + taskDays * (static_cast<unsigned __int64>(24)) * 3600;

			// If SCRUM is enabled, we add as many hours as days we have advanced
			if (scrumTime != -1)
			{
				/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
				{
					cout << "Added " << taskDays << " hour(s) and days" << endl;
				}*/
				timePlusDays = timePlusDays + taskDays * 3600;
			}

			localtime_s(&tmtime, &timePlusDays);
			if (task.name.compare("Character Design") == 0 && role.compare("CEO") == 0)
			{
				localtime_s(&tmtime, &timePlusDays);
				strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
				cout << "Remainder is " << remainder << endl;
				cout << "taskDays is " << taskDays << endl;
				cout << "Initial finish time without remainder: " << printBuffer << " remainder is " << remainder << endl;
			}

			// We add the remainder hours
			timePlusDays = timePlusDays + (static_cast<unsigned __int64>(remainder)) * 3600;

			localtime_s(&tmtimeSCRUM, &timePlusDays);

			// If we have crossed the daily scrum, we add an hour to the total time
			if (tmtime.tm_hour <= scrumTime && tmtimeSCRUM.tm_hour > scrumTime)
			{
				timePlusDays = timePlusDays + 3600;
			}

			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				localtime_s(&tmtime, &timePlusDays);
				strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
				cout << "Initial finish time with remainder: " << printBuffer << endl;
			}*/
		}
		// If the task doesnt span more than a day, we add the original duration directly
		else
		{
			localtime_s(&tmtime, &timePlusDays);
			timePlusDays = timePlusDays + (static_cast<unsigned __int64>(taskDuration)) * 3600;
			localtime_s(&tmtimeSCRUM, &timePlusDays);

			// If we have crossed the daily scrum, we add an hour to the total time
			if ((tmtime.tm_hour <= scrumTime) && (tmtimeSCRUM.tm_hour > scrumTime))
			{
				timePlusDays = timePlusDays + 3600;
			}

			/*if (task.name.compare("Character Design") == 0 && role.compare("CEO") == 0)
			{
				localtime_s(&tmtime, &timePlusDays);
				strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
				cout << "Initial finish time with task duration: " << printBuffer << endl;
			}*/

		}

		// We place the tmtime variable back to the last calculation we made
		localtime_s(&tmtime, &timePlusDays);

		// If we are in Saturday or Sunday, we advance until monday
		// Saturday
		if (tmtime.tm_wday == 6)
		{
			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				cout << "Advancing time from Saturday to Monday" << endl;
			}*/

			timePlusDays = timePlusDays + (static_cast<unsigned __int64>(48)) * 3600;
		}
		// Sunday
		else if (tmtime.tm_wday == 0)
		{
			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				cout << "Advancing time Sunday to Monday" << endl;
			}*/

			timePlusDays = timePlusDays + (static_cast<unsigned __int64>(24)) * 3600;
		}

		finishTime = timePlusDays;

		// We get the time at the schedule finish hour to calculate the remaining hours of the task in case we are out of the schedule
		localtime_s(&tmtime, &timePlusDays);

		// In the case we are at 00:00, we set the date to the previous day to correctly perform the calculation
		if (tmtime.tm_hour == 0)
		{
			time_t auxTime = timePlusDays - 7200;
			localtime_s(&tmtime, &auxTime);
		}
		tmtime.tm_hour = finishHour;
		timeAtFinish = mktime(&tmtime);
		time_t remainingHours = timePlusDays - timeAtFinish;

		// We place the tmtime variable back to the last calculation we made
		localtime_s(&tmtime, &timePlusDays);

		// If we are out of labour hours and still in the same day, we move to the next day at the start time + remaining hours
		if (tmtime.tm_hour > finishHour)
		{
			timePlusDays = timePlusDays + (static_cast<unsigned __int64>(24)) * 3600;
			localtime_s(&tmtime, &timePlusDays);
			tmtime.tm_hour = startHour;
			timeAtStart = mktime(&tmtime);


			localtime_s(&tmtime, &timeAtStart);
			finishTime = timeAtStart + remainingHours;
			localtime_s(&tmtimeSCRUM, &finishTime);

			// If we have crossed the daily scrum, we add an hour to the total time
			if ((tmtime.tm_hour <= scrumTime) && (tmtimeSCRUM.tm_hour > scrumTime))
			{
				finishTime = finishTime + 3600;
				/*if (task.name.compare("Character Design") == 0 && role.compare("CEO") == 0)
				{
					cout << "Added 1h of SCRUM daily" << endl;
				}*/
			}

			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				localtime_s(&tmtime, &finishTime);
				strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
				cout << "1. Time advanced to: " << printBuffer << endl;
			}*/
		}
		// If we are in the next day before labour, we move to the start time + remainding hours
		else if (tmtime.tm_hour < startHour)
		{
			tmtime.tm_hour = startHour;
			timeAtStart = mktime(&tmtime);

			localtime_s(&tmtime, &timeAtStart);
			finishTime = timeAtStart + remainingHours;
			localtime_s(&tmtimeSCRUM, &finishTime);

			// If we have crossed the daily scrum, we add an hour to the total time
			if ((tmtime.tm_hour <= scrumTime) && (tmtimeSCRUM.tm_hour > scrumTime))
			{
				finishTime = finishTime + 3600;
			}

			/*if (task.name.compare("Testing") == 0 && role.compare("CEO") == 0)
			{
				localtime_s(&tmtime, &finishTime);
				strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
				cout << "2. Time advanced to: " << printBuffer << endl;
			}*/
		}

		// We get the pointer to the latest time definition
		localtime_s(&tmtime, &finishTime);

		// If we are in Saturday or Sunday, we advance until monday
		// Saturday
		if (tmtime.tm_wday == 6)
		{
			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				cout << "Advancing time from Saturday to Monday" << endl;
			}*/

			finishTime = finishTime + (static_cast<unsigned __int64>(48)) * 3600;
		}
		// Sunday
		else if (tmtime.tm_wday == 0)
		{
			/*if (task.name.compare("Implementation") == 0 && role.compare("CTO") == 0)
			{
				cout << "Advancing time Sunday to Monday" << endl;
			}*/

			finishTime = finishTime + (static_cast<unsigned __int64>(24)) * 3600;
		}


		/*if (task.name.compare("Character Design") == 0 && role.compare("CEO") == 0)
		{
			localtime_s(&tmtime, &finishTime);
			strftime(printBuffer, 26, "%Y-%m-%d %H:%M:%S", &tmtime);
			cout << "Final finish time: " << printBuffer << endl;
		}*/

		// We return the final finishTime
		return finishTime;

	}
};

class State {
public:
	Variable variable;
	Value value;

public:
	State() {
	}

	State(Variable variable, Value value) {
		this->variable = variable;
		this->value = value;
	}

	void printState() const {

		std::cout << "--- " << this->variable.task.name << "-" << this->value.employee.name << "-" << this->value.date << " ---" << endl;

		cout << endl;
	}

};

// Overloading for sets of Variable and DomainValue
bool operator< (const Variable& obj1, const Variable& obj2) {

	return obj1.id < obj2.id;

}

bool operator< (const Value& obj1, const Value& obj2) {

	return obj1.id < obj2.id;

}

// Static variables dependant on classe
static std::set<Variable> VARIABLES;
static std::set<Value> DOMAINS;
static std::map< pair<std::string, std::string>, vector<pair<int, int>> > MUTEX;
vector<State> SOLUTION;

void writeCSV(std::string filename, std::vector<std::pair<std::string, std::vector<string>>> dataset) {
	// Make a CSV file with one or more columns of integer values
	// The input file (dataset) is composed of a vector that containts the columns
	// Each column of data is represented by a pair <column string, column data (vector)> as std::pair<std::string, std::vector<string>>
	// Note that all columns should be the same size

	// Create an output filestream object
	std::ofstream myFile(filename);

	// We traverse all the input to get the names of the columns
	// For as many columns
	for (size_t j = 0; j < dataset.size(); ++j)
	{
		// Vector j of the dataset
		myFile << dataset.at(j).first;

		// We place commas at the end of each name, except the last one
		if (j != dataset.size() - 1) myFile << ",";
	}

	// Next line
	myFile << "\n";

	// We traverse all the data and send it to the output file
	// For as many elements in the column (all columns have the same size)
	for (size_t i = 0; i < dataset.at(0).second.size(); ++i)
	{
		// For as many columns
		for (size_t j = 0; j < dataset.size(); ++j)
		{
			// Vector j of the dataset, datum i of the column
			myFile << dataset.at(j).second.at(i);

			// We place commas at the end of each datum, except the last one
			if (j != dataset.size() - 1) myFile << ",";
		}
		myFile << "\n";
	}

	// Close the file
	myFile.close();
}

std::vector<std::pair<std::string, std::vector<string>>> readCSV(std::string filename) {
	// Reads a CSV file into a vector of <string, vector<string>> pairs where each pair represents <column name, column values>

	// Create a vector of <string, int vector> pairs to store the result
	std::vector<std::pair<std::string, std::vector<string>>> result;

	// Create an input filestream
	std::ifstream myFile(filename);

	// Make sure the file is open
	if (!myFile.is_open()) throw std::runtime_error("Could not open file");

	// Helper vars
	std::string line, colname, coldata;
	string val;

	// Read the column names
	if (myFile.good())
	{
		// Extract the first line in the file
		std::getline(myFile, line);

		// Create a stringstream from line
		std::stringstream ss(line);

		// Extract each column name
		while (std::getline(ss, colname, ',')) {

			// Initialize and add <colname, int vector> pairs to result
			result.push_back({ colname, std::vector<string> {} });
		}
	}

	// Read data, line by line
	while (std::getline(myFile, line))
	{
		// Create a stringstream of the current line
		std::stringstream ss(line);

		// Keep track of the current column index
		int colIdx = 0;

		// Extract each column data
		while (std::getline(ss, coldata, ',')) {

			// Add each data to the corresponding column
			result.at(colIdx).second.push_back(coldata);

			// Increment the column index
			colIdx++;
		}

	}

	// Close file
	myFile.close();

	return result;
}

void printCSV(std::vector<std::pair<std::string, std::vector<string>>> CSV) {

	for (size_t i = 0; i < CSV.size(); i++) {
		// We print the column names
		cout << CSV.at(i).first << endl;
		cout << "------------" << endl;

		// We print each column data
		for (size_t j = 0; j < CSV.at(0).second.size(); j++)
		{
			cout << CSV.at(i).second.at(j) << endl;
		}

		cout << endl;
	}

}

void printEmployees(std::vector<Employee> employeeList) {

	cout << "Printing employees" << endl;
	cout << "------------" << endl;

	for (size_t i = 0; i < employeeList.size(); i++)
	{
		employeeList.at(i).printEmployee();
	}

	cout << endl;

}

void printTasks(std::vector<Task> taskList) {

	cout << "Printing tasks" << endl;
	cout << "------------" << endl;

	for (size_t i = 0; i < taskList.size(); i++)
	{
		taskList.at(i).printTask();
	}

	cout << endl;
}

void printVariables() {

	cout << "Printing variables" << endl;
	cout << "------------" << endl;

	for (auto VARIABLE : VARIABLES)
	{
		VARIABLE.printVariable();
		//cout << "Size: " << VARIABLE.domains.size() << endl;
	}

	cout << endl;
}

void printDomains() {

	cout << "Printing domains" << endl;
	cout << "------------" << endl;

	for (auto DOMAINX : DOMAINS)
	{
		DOMAINX.printValue();
	}

	cout << endl;
}

bool areStateVectorsEqual(vector<State> vector1, vector<State> vector2) {

	bool allEqual = true;

	// We compare each state of each vector
	for (size_t i = 0; i < vector1.size(); i++)
	{
		// If any of their values dont coincide, they are not equal
		if ( vector1.at(i).value.id != vector2.at(i).value.id )
		{
			allEqual = false;
		}
	}

	return allEqual;
}

void print_map(std::map< pair<std::string, std::string>, vector<pair<Value*, Value*>> > const& m)
{
	for (auto const& pair : m) {
		cout << "{" << pair.first.first << " | " << pair.first.second << "}\n";
		for (auto pais : pair.second)
		{
			cout << "{" << pais.first->id << " | " << pais.second->id << "}\n";
		}

	}
}

std::vector<Employee> fillEmployees(std::vector<std::pair<std::string, std::vector<string>>> CSV) {

	cout << "Filling Employees" << endl;
	cout << "------------" << endl;
	std::vector<Employee> employeeList;

	// Switch variables
	std::string segment;
	std::istringstream tokenStream;
	char delimiter = '|';

	// For each field of employee
	for (size_t i = 0; i < CSV.size(); i++) {

		// For each datum in the field
		for (size_t j = 0; j < CSV.at(0).second.size(); j++)
		{
			Employee employee;
			switch (i) {
				// The object is inserted in the list if we are in the first datum of the object
			case 0:
				employee.name = CSV.at(i).second.at(j);
				employeeList.push_back(employee);
				break;
				// If the object is already in the list we fill its values
			case 1:
				employeeList.at(j).role = CSV.at(i).second.at(j);
				tokenStream.clear();
				break;
			}

		}
		cout << CSV.at(i).first << " filled" << endl;
	}
	cout << "All employees filled" << endl;
	cout << endl;
	return employeeList;
}

std::vector<Task> fillTasks(std::vector<std::pair<std::string, std::vector<string>>> CSV) {

	cout << "Filling tasks" << endl;
	cout << "------------" << endl;
	std::vector<Task> taskList;

	// Switch variables
	std::string segment;
	std::istringstream tokenStream;
	char delimiter = '|';

	// For each field of Task
	for (size_t i = 0; i < CSV.size(); i++) {

		// For each datum in the field
		for (size_t j = 0; j < CSV.at(0).second.size(); j++)
		{
			Task task;
			switch (i) {
				// The object is inserted in the list if we are in the first datum of the object
			case 0:
				task.name = CSV.at(i).second.at(j);
				taskList.push_back(task);
				break;
				// If the object is already in the list we fill its values
			case 1:
				tokenStream.str(CSV.at(i).second.at(j));
				while (std::getline(tokenStream, segment, delimiter))
				{
					taskList.at(j).durations.push_back(stoi(segment));
				}
				tokenStream.clear();
				break;
			case 2:
				tokenStream.str(CSV.at(i).second.at(j));
				while (std::getline(tokenStream, segment, delimiter))
				{
					taskList.at(j).compatibilities.push_back(segment);
				}
				tokenStream.clear();
				break;
			case 3:
				tokenStream.str(CSV.at(i).second.at(j));
				while (std::getline(tokenStream, segment, delimiter))
				{
					taskList.at(j).dependencies.push_back(segment);
				}
				tokenStream.clear();
				break;
			}


		}
		cout << CSV.at(i).first << " filled" << endl;
	}
	cout << "All employees filled" << endl;
	cout << endl;
	return taskList;
}

bool isDomain(Variable VARIABLE, Value VALUE) {
	for (size_t i = 0; i < VARIABLE.task.compatibilities.size(); i++)
	{
		if (VALUE.employee.role.compare(VARIABLE.task.compatibilities.at(i)) == 0)
		{
			//cout << "Value " << VALUE.id << " is domain of " << VARIABLE.task.name << endl;
			return true;
		}
	}

	return false;
}

void grounding(std::vector<Employee> employeeList, std::vector<Task> taskList) {

	cout << "Grounding of variables and values" << endl;
	cout << "------------" << endl;

	// Auxiliary variables
	Variable temporalVariable;
	Value temporalDomain;
	std::time_t temporalDate = mktime(&startDate);
	std::pair<int, int> temporalPair1;

	//cout << "Time in epoch: " << temporalDate << endl;
	int iterations = 0;

	/*cout << "------------" << endl;
	cout << "Starting task" << endl;
	cout << "------------" << endl;*/

	// We get the starting ID of the task
	temporalPair1.first = iterations;

	// For each Employee
	for (Employee EMPLOYEE : employeeList)
	{
		// For as many weeks as the project takes
		for (size_t w = 0; w < numberOfWeeks; w++) {

			// For each week
			for (size_t k = 0; k < labourDays; k++) {

				// For each hour of a day
				for (size_t l = 0; l < hoursPerDay; l++) {

					struct std::tm hora;
					localtime_s(&hora, &temporalDate);

					// If there is SCRUM time and its not that time, we generate the combination
					if (hora.tm_hour != scrumTime)
					{
						// We generate a combination of all of the factors
						temporalDomain.employee = EMPLOYEE;
						temporalDomain.date = temporalDate;
						temporalDomain.id = iterations;

						//cout << "Day: " << hora.tm_mday << endl;
						//cout << "Hour: " << hora.tm_hour << endl;

						DOMAINS.insert(temporalDomain);

						iterations++;
					}
					temporalDate += 3600;

				}
				/*cout << "------------" << endl;
				cout << "Day ended" << endl;
				cout << "------------" << endl;*/
				temporalDate += 57600;
			}
			/*cout << "------------" << endl;
			cout << "Week ended" << endl;
			cout << "------------" << endl;*/
			temporalDate += 172800;

		}
		/*cout << "------------" << endl;
		cout << "Employee ended" << endl;
		cout << "------------" << endl;*/
		temporalDate = mktime(&startDate);

		// We get the finishing ID of the Employee
		temporalPair1.second = iterations;

		cout << "Employee: " << EMPLOYEE.name << " IDs " << temporalPair1.first << " to " << temporalPair1.second << endl;

		temporalPair1.first = iterations;
	}


	for (size_t i = 0; i < taskList.size(); i++) {

		// We assign the task
		temporalVariable.task = taskList.at(i);
		temporalVariable.id = i;
		// We push the data into the set
		VARIABLES.insert(temporalVariable);

		/*cout << "------------" << endl;
		cout << "Task ended" << endl;
		cout << "------------" << endl;*/
	}
	//cout << "Iterations: " << iterations << endl;
	cout << endl;
	cout << "All tasks and domains have been grounded" << endl;
	cout << endl;
}

//void populateMutexParallel(std::vector<Variable> inputVariables) {
//	cout << "Calculating mutex" << endl;
//	cout << "------------" << endl;
//
//	//thread_local int counter = 0;
//	thread_local std::pair< pair<std::string, std::string>, vector<pair<int, int>> > temporalMutexPair;
//	thread_local vector<pair<int, int>> temporalMutexValueVector;
//	thread_local std::pair<std::string, std::string> temporalMutexVariablePair;
//	thread_local std::pair<int, int> temporalMutexValuePair;
//	thread_local Variable temporalVariable1;
//	thread_local Variable temporalVariable2;
//
//	// For each input variable
//	for (auto VARIABLE1 : inputVariables) {
//		cout << "Calculating mutex of " << VARIABLE1.task.name << endl;
//
//		// We compare it against the rest of the tasks
//		for (auto VARIABLE2 : VARIABLES)
//		{
//			temporalMutexVariablePair = std::make_pair(VARIABLE1.task.name, VARIABLE2.task.name);
//
//			/*if (VARIABLE1.task.name.compare("Programming Testing") == 0)
//			{
//				cout << "**Comparing task " << VARIABLE1.task.name << " with " << VARIABLE2.task.name << endl;
//			}*/
//
//			// For each value in the domain of the first task
//			for (auto VALUE1 : DOMAINS)
//			{
//				//cout << "Calculating mutex of " << VARIABLE1.task.name << " with " << VARIABLE2.task.name << endl;
//				if (isDomain(VARIABLE1, VALUE1))
//				{
//
//					// We compare it against the domain of the rest of the tasks
//					for (auto VALUE2 : DOMAINS)
//					{
//						if (isDomain(VARIABLE2, VALUE2))
//						{
//
//							//cout << "Comparing " << VALUE1.id << " to " << VALUE2.id << endl;
//
//							time_t task2FinishTime = VARIABLE2.getTaskFinishTime(VALUE2.employee.role, VALUE2.date);
//
//							// We add all combinations of the same employee at the duration of the task
//							if ((VALUE1.employee.name.compare(VALUE2.employee.name) == 0) && (VALUE2.date <= VALUE1.date) && (VALUE1.date < task2FinishTime))
//							{
//								if (VARIABLE1.task.name.compare("Testing") == 0)
//								{
//									//cout << "1. Value " << VALUE1.id << " mutex with " << VALUE2.id << endl;
//									//cout << "Date " << task2FinishTime << " is > than " << VALUE1.date << endl;
//								}
//
//								temporalMutexValuePair = std::make_pair(VALUE1.id, VALUE2.id);
//								temporalMutexValueVector.push_back(temporalMutexValuePair);
//								//counter++;
//							}
//							// We add all the combinations that have tasks that depend on the actual one and are done before it
//							else if (VARIABLE1.task.dependsOn(VARIABLE2.task.name) && (VALUE1.date < task2FinishTime)) {
//								/*if (VARIABLE1.task.name.compare("Programming Testing") == 0)
//								{
//									cout << "2. Value " << VALUE1.id << " mutex with " << VALUE2.id << endl;
//									cout << "Task " << VARIABLE1.task.name << " depends on " << VARIABLE2.task.name << endl;
//								}*/
//
//								temporalMutexValuePair = std::make_pair(VALUE1.id, VALUE2.id);
//								temporalMutexValueVector.push_back(temporalMutexValuePair);
//								//counter++;
//							}
//
//						}
//					}
//
//				}
//
//			}
//			//cout << "Pair is " << temporalMutexVariablePair.first << " and " << temporalMutexVariablePair.second << endl;
//			temporalMutexPair = std::make_pair(temporalMutexVariablePair, temporalMutexValueVector);
//			MUTEX.insert(temporalMutexPair);
//			temporalMutexValueVector.clear();
//		}
//		//cout << "Mutex size of " << VARIABLE1.task.name << " is " << counter << endl;
//		//counter = 0;
//
//	}
//
//	cout << endl;
//
//}

void populateMutex() {
	cout << "Calculating mutex" << endl;
	cout << "------------" << endl;

	//thread_local int counter = 0;
	thread_local std::pair< pair<std::string, std::string>, vector<pair<int, int>> > temporalMutexPair;
	thread_local vector<pair<int, int>> temporalMutexValueVector;
	thread_local std::pair<std::string, std::string> temporalMutexVariablePair;
	thread_local std::pair<int, int> temporalMutexValuePair;
	thread_local Variable temporalVariable1;
	thread_local Variable temporalVariable2;

	// For each Task
	for (auto VARIABLE1 : VARIABLES) {
		cout << "Calculating mutex of " << VARIABLE1.task.name << endl;

		// We compare it against the rest of the tasks
		for (auto VARIABLE2 : VARIABLES)
		{
			temporalMutexVariablePair = std::make_pair(VARIABLE1.task.name, VARIABLE2.task.name);

			/*if (VARIABLE1.task.name.compare("Programming Testing") == 0)
			{
				cout << "**Comparing task " << VARIABLE1.task.name << " with " << VARIABLE2.task.name << endl;
			}*/

			// For each value in the domain of the first task
			for (auto VALUE1 : DOMAINS)
			{
				//cout << "Calculating mutex of " << VARIABLE1.task.name << " with " << VARIABLE2.task.name << endl;
				if (isDomain(VARIABLE1, VALUE1))
				{

					// We compare it against the domain of the rest of the tasks
					for (auto VALUE2 : DOMAINS)
					{
						if (isDomain(VARIABLE2, VALUE2))
						{

							//cout << "Comparing " << VALUE1.id << " to " << VALUE2.id << endl;

							time_t task2FinishTime = VARIABLE2.getTaskFinishTime(VALUE2.employee.role, VALUE2.date);

							// We add all combinations of the same employee at the duration of the task
							if ((VALUE1.employee.name.compare(VALUE2.employee.name) == 0) && (VALUE2.date <= VALUE1.date) && (VALUE1.date < task2FinishTime))
							{
								if (VARIABLE1.task.name.compare("Testing") == 0)
								{
									//cout << "1. Value " << VALUE1.id << " mutex with " << VALUE2.id << endl;
									//cout << "Date " << task2FinishTime << " is > than " << VALUE1.date << endl;
								}

								temporalMutexValuePair = std::make_pair(VALUE1.id, VALUE2.id);
								temporalMutexValueVector.push_back(temporalMutexValuePair);
								//counter++;
							}
							// We add all the combinations that have tasks that depend on the actual one and are done before it
							else if (VARIABLE1.task.dependsOn(VARIABLE2.task.name) && (VALUE1.date < task2FinishTime)) {
								/*if (VARIABLE1.task.name.compare("Programming Testing") == 0)
								{
									cout << "2. Value " << VALUE1.id << " mutex with " << VALUE2.id << endl;
									cout << "Task " << VARIABLE1.task.name << " depends on " << VARIABLE2.task.name << endl;
								}*/

								temporalMutexValuePair = std::make_pair(VALUE1.id, VALUE2.id);
								temporalMutexValueVector.push_back(temporalMutexValuePair);
								//counter++;
							}

						}
					}

				}

			}
			//cout << "Pair is " << temporalMutexVariablePair.first << " and " << temporalMutexVariablePair.second << endl;
			temporalMutexPair = std::make_pair(temporalMutexVariablePair, temporalMutexValueVector);
			MUTEX.insert(temporalMutexPair);
			temporalMutexValueVector.clear();
		}
		//cout << "Mutex size of " << VARIABLE1.task.name << " is " << counter << endl;
		//counter = 0;

	}

	cout << endl;

}

bool isMutex(State CURRENTSTATE, vector<State> stateAssignments) {

	// For every assigned variable, we check if its assigned value is mutex with the input state
	for (State STATEASSIGNMENT : stateAssignments)
	{
		// If its assigned and we are not in the current assignment
		if (STATEASSIGNMENT.value.id != -1 && STATEASSIGNMENT.variable.id != CURRENTSTATE.variable.id)
		{

			Value value1;
			Value value2;
			vector<pair<int, int>> vectorMutex;

			// If the iterator returns a pointer not to the end, there is a mutex of these pair of variables

			std::map< pair<std::string, std::string>, vector<pair<int, int>> >::iterator iterator = MUTEX.find(std::make_pair(CURRENTSTATE.variable.task.name, STATEASSIGNMENT.variable.task.name));


			if (iterator != MUTEX.end())
			{
				value1 = CURRENTSTATE.value;
				value2 = STATEASSIGNMENT.value;
			}
			else {
				cout << "The current pair is not in the mutex list" << endl;
				return true;
			}


			// We get the vector from the iterator
			vectorMutex = iterator->second;
			//cout << "Checking mutex of " << iterator->first.first << " with " << iterator->first.second << endl;

			// For each value of the mutex, we check if it corresponds to the values of the input States
			for (size_t i = 0; i < vectorMutex.size(); i++)
			{
				//cout << "Mutex value " << vectorMutex.at(i).first << " and " << vectorMutex.at(i).second << endl;
				if (vectorMutex.at(i).first == value1.id && vectorMutex.at(i).second == value2.id) {
					//cout << "Value " << value1.id << " of " << CURRENTSTATE.variable.task.name << " is not compatible with " << value2.id << " of " << STATEASSIGNMENT.variable.task.name << endl;;
					return true;
				}
			}

			//cout << "Value " << value1.id << " is compatible with " << value2.id << endl;

		}

	}

	// If there hasn't been a mutex found, we return the result
	return false;

}

bool allStatesAssigned(vector<State> stateAssignments) {

	//cout << "Checking if all states are assigned" << endl;

	// We iterate through all of the variables and check if they are not null
	bool allAssigned = true;
	for (State STATE : stateAssignments)
	{
		if (STATE.value.id == -1) {
			//cout << "Variable " << STATE.variable.task.name << " is not assigned" << endl;
			allAssigned = false;
		}
	}

	if (allAssigned == true)
	{
		//cout << "All states are assigned" << endl;
	}
	return allAssigned;
}

void printStateAssignments(vector<State> stateAssignments) {

	for (State STATE : stateAssignments)
	{
		cout << STATE.variable.task.name << " " << STATE.value.id << endl;
	}

}

State getUnnassignedState(vector<State> stateAssignments) {

	for (State STATE : stateAssignments)
	{

		if (STATE.value.id == -1) {
			//cout << "Unnassigned state " << STATE.variable.task.name << " retrieved" << endl;
			return STATE;
		}
	}

	//cout << "No unnassigned states" << endl;

}

void pushAssignedState(State CURRENTSTATE, vector<State>& stateAssignments) {

	// We iterate through the state assignments
	for (size_t i = 0; i < stateAssignments.size(); i++)
	{
		// If its variable coincides with the input one, we assign the value to the variable
		if (stateAssignments.at(i).variable.id == CURRENTSTATE.variable.id)
		{
			//	cout << "Pushing value " << CURRENTSTATE.value.id << " for variable " << stateAssignments.at(i).variable.task.name << endl;
			stateAssignments.at(i).value = CURRENTSTATE.value;

		}
	}
}

void pushAssignedValues(vector<Value> candidateValues, Variable VARIABLE, vector<State> currentStateAsignment, vector<vector<State>>& stateAssignments) {

	/*for (vector<State> SOLUTION : stateAssignments)
	{
		cout << "Current solutions:" << endl;
		printStateAssignments(SOLUTION);
		cout << endl;
	}*/

	// We iterate through the vectors of state assignments
	for (size_t i = 0; i < stateAssignments.size(); i++)
	{
		// If the vector is equal to the input one
		if (areStateVectorsEqual(currentStateAsignment, stateAssignments.at(i)))
		{
			//cout << "Vectors are equal" << endl;

			// We go to the corresponding variable
			for (size_t ii = 0; ii < currentStateAsignment.size(); ii++)
			{
				if (currentStateAsignment.at(ii).variable.id == VARIABLE.id) {

					//cout << "State found, updating state" << endl;

					// We create assignments for each of the new values
					for (size_t iii = 0; iii < candidateValues.size(); iii++)
					{
						currentStateAsignment.at(ii).value = candidateValues.at(iii);

						// If it the first value we just assign it
						if (iii == 0)
						{
							//cout << "Pushing replacement of original state" << endl;
							stateAssignments.at(i) = currentStateAsignment;
						}
						// If its not the first value, we push a new assignment with the value
						else {

							//cout << "Pushing alternative state" << endl;
							stateAssignments.push_back(currentStateAsignment);
						}
						
						
					}

					// When we have finished creating the states, we return
					return;
				}
			}


		}
	}
}

void deleteAssignedVariable(State CURRENTSTATE, vector<State>& stateAssignments) {

	// We iterate through the state assignments
	for (size_t i = 0; i < stateAssignments.size(); i++)
	{
		// If its variable coincides with the input one, we reset its value
		if (stateAssignments.at(i).variable.id == CURRENTSTATE.variable.id)
		{
			stateAssignments.at(i).value.date = NULL;
			stateAssignments.at(i).value.employee.setNull();
			stateAssignments.at(i).value.id = -1;
		}
	}

	//// We iterate through the state assignments
	//for (State STATEASSIGNMENT : stateAssignments)
	//{
	//	// If its variable coincides with the input one, we reset its value
	//	if (STATEASSIGNMENT.variable.id == CURRENTSTATE.variable.id)
	//	{
	//		STATEASSIGNMENT.value.date = NULL;
	//		STATEASSIGNMENT.value.employee.setNull();
	//		STATEASSIGNMENT.value.id = -1;
	//	}
	//}
}

void writeSolution(vector<vector<State>> stateAssignments) {

	cout << "Exporting solution" << endl;
	cout << "------------" << endl;

	// Vectors for the column data
	vector<string> employeeData;
	vector<string> taskData;
	vector<string> dateData;
	vector<string> durationData;
	string durationString;
	time_t lastStateDate = 00000;

	// Final vector for inserting solution into the writing CSV function
	std::vector<std::pair<std::string, std::vector<string>>> solutions;

	// For each solution
	for (vector<State> solution : stateAssignments)
	{

		// Collect data from solution and place it into the vectors
		for (State state : solution)
		{
			employeeData.push_back(state.value.employee.name);
			taskData.push_back(state.variable.task.name);
			durationString = to_string(state.variable.getTaskDuration(state.value.employee.role));
			durationData.push_back(durationString);

			stringstream timeString;
			struct std::tm hora;
			localtime_s(&hora, &state.value.date);
			timeString << std::put_time(&hora, "%Y-%m-%dT%H:%M:%S");
			dateData.push_back(timeString.str());

			// If there was no SCRUM implementation, we make the calculation of the finish Date
			if (scrumTime == -1)
			{
				lastStateDate = state.variable.getTaskFinishTime(state.value.employee.role, state.value.date);
			}

		}

		// We add a final state for the end of the project
		employeeData.push_back("-");
		taskData.push_back("Finish");
		durationData.push_back("-");
		stringstream timeString;

		// If there is a SCRUM implementation, we take the value from the static variable
		if (scrumTime != -1) {
			timeString << std::put_time(&finishDate, "%Y-%m-%dT%H:%M:%S");
		}
		else {
			struct std::tm hora;
			localtime_s(&hora, &lastStateDate);
			timeString << std::put_time(&hora, "%Y-%m-%dT%H:%M:%S");
		}
		dateData.push_back(timeString.str());


		// Insert column name and data into solution vector
		solutions.push_back(std::make_pair("Employee", employeeData));
		solutions.push_back(std::make_pair("Task", taskData));
		solutions.push_back(std::make_pair("Date", dateData));
		solutions.push_back(std::make_pair("Duration", durationData));

		employeeData.clear();
		taskData.clear();
		dateData.clear();
		durationData.clear();

	}

	// Call to the writing CSV function with the data
	writeCSV("Solution.csv", solutions);

}

void checkBestSolution(vector<State>& stateAssignments) {

	// Auxiliary variables
	time_t oldCounter = 00000;
	time_t newCounter = 00000;

	// We get the total time of the current solution and the new one
	for (State STATE : SOLUTION)
	{
		oldCounter = oldCounter + STATE.value.date;
	}
	for (State STATE : stateAssignments)
	{
		newCounter = newCounter + STATE.value.date;
	}

	// If the new one has a lower time, it becomes the new solution
	if (newCounter < oldCounter)
	{
		cout << "Better solution found" << endl;
		SOLUTION = stateAssignments;
	}

}

bool compareValuesByDate(Value v1, Value v2) {
	return (v1.date < v2.date);
}

bool compareStateVectorByDate(vector<State> s1, vector<State> s2) {

	time_t s1Date = 00000;
	time_t s2Date = 00000;

	// We traverse the states and get the last value that is not -1
	for (State STATE : s1)
	{
		if (STATE.value.id != -1)
		{
			s1Date = STATE.value.date;
		}
	}

	for (State STATE : s2)
	{
		if (STATE.value.id != -1)
		{
			s2Date = STATE.value.date;
		}
	}

	return (s1Date < s2Date);
}

vector<Value> getMostFavourableValues(vector<Value> valuesVector, int numberOfKCandidates) {

	// Auxiliary variable
	vector<Value> solutionVector;

	// We first sort the array of dates
	std::sort(valuesVector.begin(), valuesVector.end(), compareValuesByDate);
	
	// Counter for how many distinct elements have been pushed
	int counter = 0;
	bool duplicate = false;

	// And then we get the top K distinct values we want
	for (size_t i = 0; i < valuesVector.size(); i++)
	{
		// For each element we compare it against the elements of the solution vector
		for (Value VALUE : solutionVector)
		{
			if (valuesVector.at(i).id == VALUE.id )
			{
				duplicate = true;
			}
		}

		// If it isnt duplicate we push it
		if (duplicate == false)
		{
			solutionVector.push_back(valuesVector.at(i));
			counter++;
		}

		// We reset the variable
		duplicate = false;

		// If we have pushed K values we break the loop
		if (counter == numberOfKCandidates)
		{
			break;
		}
		
	}

	return solutionVector;
}

void purgeSolutions(vector<vector<State>>& solutionsVector, Variable VARIABLE, int numberOfKCandidates) {

	// We sort the solutions in ascendant order, best ones are the ones with the lowest value of the corresponding Variablee
	std::sort(solutionsVector.begin(), solutionsVector.end(), compareStateVectorByDate);

	// We get the K values from the vector
	vector<vector<State>> auxVector(solutionsVector.begin(), solutionsVector.begin() + numberOfKCandidates);
	solutionsVector = auxVector;
}

// Initially receives the states with all variables unnassigned
bool DFS(vector<State>& stateAssignments) {

	cout << "----" << endl;

	//printStateAssignments(stateAssignments);

	// Base case: all variables have been assigned
	if (allStatesAssigned(stateAssignments) == true)
	{
		// For the first solution
		return true;
	}
	else {

		// We get an unnassigned variable
		State currentState = getUnnassignedState(stateAssignments);

		// We iterate through all of the values of its domain
		for (Value VALUE : DOMAINS)
		{
			if (isDomain(currentState.variable, VALUE))
			{
				// If the current value is not mutex with the already asigned variables
				currentState.value = VALUE;
				if (isMutex(currentState, stateAssignments) == false)
				{
					// We push the value into the assignedVariables list and execute the recursive call
					pushAssignedState(currentState, stateAssignments);
					bool result = DFS(stateAssignments);

					// If the call result is failure, we delete the assignment
					if (result == false)
					{
						deleteAssignedVariable(currentState, stateAssignments);
					}
					else {
						return true;
					}

				}
			}
		}

		// All values have been tested, we backtrack
		cout << "End of the variable values" << endl;
		cout << endl;
		return false;

	}
}

bool DFSComplete(vector<State>& stateAssignments) {

	//cout << "----" << endl;

	//printStateAssignments(stateAssignments);

	// Base case: all variables have been assigned
	if (allStatesAssigned(stateAssignments) == true)
	{
		// If its the first solution, we assign it
		if (SOLUTION.size() == 0)
		{
			cout << "First solution found" << endl;
			SOLUTION = stateAssignments;
		}
		// If its not, we compare it against the current one to see if its better
		else {
			checkBestSolution(stateAssignments);
		}

		return false;

	}
	else {

		// We get an unnassigned variable
		State currentState = getUnnassignedState(stateAssignments);

		// We iterate through all of the values of its domain
		for (Value VALUE : DOMAINS)
		{
			if (isDomain(currentState.variable, VALUE))
			{
				// If the current value is not mutex with the already assigned variables
				currentState.value = VALUE;
				if (isMutex(currentState, stateAssignments) == false)
				{
					// We push the value into the assignedVariables list and execute the recursive call
					pushAssignedState(currentState, stateAssignments);
					bool result = DFSComplete(stateAssignments);

					// If the call result is failure, we delete the assignment
					if (result == false)
					{
						deleteAssignedVariable(currentState, stateAssignments);
					}

				}
			}
			//cout << "End of value" << endl;
		}

		// All values have been tested, we backtrack
		//cout << "End of the variable " << currentState.variable.task.name << " values" << endl;
		//cout << endl;
		deleteAssignedVariable(currentState, stateAssignments);
		return false;

	}
}

bool GreedySearch(vector<State>& stateAssignments) {

	// For each value that is compatible with the current one, we choose the most favorable one
	// We compute the heuristic calculation, it being the state with the lowest time
	State tempState;

	for (Variable VARIABLE : VARIABLES)
	{
		vector<Value> candidateValues;
		tempState.variable = VARIABLE;

		// We iterate through all of the values of its domain
		for (Value VALUE : DOMAINS)
		{
			if (isDomain(VARIABLE, VALUE))
			{
				tempState.value = VALUE;
				// If the current value is not mutex with the already asigned variables
				if (isMutex(tempState, stateAssignments) == false)
				{
					// We add it to the vector of candidates
					candidateValues.push_back(VALUE);
				}
			}
		}
		cout << "Vector size: " << candidateValues.size() << endl;

		// We set the K as 5
		vector<Value> bestValues = getMostFavourableValues(candidateValues, 5);

		// We extend these 5 values and execute the algorithm again

		tempState.value = bestValues.at(0);
		pushAssignedState(tempState, stateAssignments);
	}

	// All values have been tested, we backtrack
	cout << "End of the greedy search" << endl;
	cout << endl;
	printStateAssignments(stateAssignments);
	return true;
}

vector<vector<State>> BeamSearch(vector<State>& stateAssignments, int numberOfKCandidates) {
	// For each value that is compatible with the current one, we choose the most favorable one
	// We compute the heuristic calculation, it being the state with the lowest time in the latest instantitead task
	// We do this with various instantiations at a time

	cout << "Beginning Beam Search" << endl;
	cout << "------" << endl;

	// List of vectors of solutions
	vector<vector<State>> solutionsVector;
	solutionsVector.push_back(stateAssignments);

	State tempState;

	// For each variable
	for (Variable VARIABLE : VARIABLES)
	{
		cout << "Number of solution vectors: " << solutionsVector.size() << endl;
		cout << endl;

		vector<Value> candidateValues;
		tempState.variable = VARIABLE;

		vector<vector<State>> solutionsCopy = solutionsVector;

		// For each instantiated assignment
		for (vector<State> VECTOR : solutionsVector)
		{

			cout << "Comprobando vector: " << endl;
			//printStateAssignments(VECTOR);

			// We iterate through all of the values of its domain that are compatible with the already instantiated variable
			for (Value VALUE : DOMAINS)
			{
				if (isDomain(VARIABLE, VALUE))
				{
					tempState.value = VALUE;
					// If the current value is not mutex with the already asigned variables
					if (isMutex(tempState, VECTOR) == false)
					{
						// We add it to the vector of candidates
						candidateValues.push_back(VALUE);
					}
				}
			}

			//cout << "Vector size: " << candidateValues.size() << endl;

			// We get the K best values
			candidateValues = getMostFavourableValues(candidateValues, numberOfKCandidates);

			cout << "Values are: ";
			for (Value VALUE: candidateValues)
			{
				cout << VALUE.id << " ";
			}
			cout << endl;

			// We push the K best values into new assignments
			pushAssignedValues(candidateValues, VARIABLE, VECTOR, solutionsCopy);

			
			for (vector<State> VECTOR : solutionsCopy)
			{
				cout << "- Solution after Values -" << endl;
				printStateAssignments(VECTOR);
			}

			cout << "-- Vector of states finished --" << endl;
			cout << endl;
		}

		cout << "-- Variable finished " << VARIABLE.task.name << " --" << endl;
		cout << endl;

		/*for (vector<State> solution : solutionsCopy)
		{
			cout << "-- Solutions prePurge --" << endl;
			printStateAssignments(solution);
		}*/

		if (solutionsCopy.size() > numberOfKCandidates)
		{
			purgeSolutions(solutionsCopy, VARIABLE, numberOfKCandidates);
		}

		solutionsVector = solutionsCopy;

	}

	// All values have been tested, we backtrack
	cout << "End of the Beam search" << endl;
	cout << endl;

	return solutionsVector;
}

vector<State> createStateVector() {

	vector<State> stateAssignments;
	State auxState;
	int counter = 0;

	// For each variable, we push it into the vector with unassigned value (-1)
	for (Variable VARIABLE : VARIABLES)
	{
		auxState.variable = VARIABLE;
		auxState.value.id = -1;
		stateAssignments.push_back(auxState);

		counter++;
	}

	return stateAssignments;
}

vector<State> insertSCRUM(vector<State>& stateAssignments) {

	cout << "Inserting SCRUM dailies" << endl;
	cout << "------------" << endl;

	// We get the finishing date
	for (State state : stateAssignments)
	{
		time_t tempDate;
		tempDate = state.variable.getTaskFinishTime(state.value.employee.role, state.value.date);
		localtime_s(&finishDate, &tempDate);
	}

	// We copy the static variables into local copies
	time_t localStartDate = mktime(&startDate);
	time_t localFinishDate = mktime(&finishDate);

	// We initialize all the values of the SCRUM task except the date
	Task tempTask;
	tempTask.name = "Daily SCRUM";
	tempTask.compatibilities = { "DAILY" };
	tempTask.durations = { 1 };
	Variable tempVariable(-2, tempTask);
	Employee tempEmployee("All employees", "DAILY");
	Value tempValue(-2, tempEmployee, 00000);
	State tempState(tempVariable, tempValue);
	time_t tempTime = 00000;

	// We initialize the date value to the SCRUM time
	struct std::tm time;
	localtime_s(&time, &localStartDate);
	time.tm_hour = scrumTime;
	localStartDate = mktime(&time);
	// We create as many states as days from the beginning to the end of the project
	// In each of these states, we set the value as SCRUM at the designed time
	while (localFinishDate > localStartDate)
	{
		// We assign the time and insert the state into the solution vector
		tempState.value.date = mktime(&time);
		stateAssignments.push_back(tempState);

		// We advance 24h
		localStartDate = localStartDate + (static_cast<unsigned __int64>(24)) * 3600;

		// We assign the new time to the tm variable
		localtime_s(&time, &localStartDate);

		// If we are in Saturday, we advance until monday
		if (time.tm_wday == 6)
		{
			localStartDate = localStartDate + (static_cast<unsigned __int64>(48)) * 3600;
		}

		// We assign the new time to the tm variable
		localtime_s(&time, &localStartDate);

	}

	// We return the solution with the added states
	return stateAssignments;
}

int main()
{

	startDate = {};
	std::locale mylocale("");
	//std::istringstream ss("2021-02-22T09:00:00Z");
	std::istringstream ss("2021-02-22T09:00:00");
	ss.imbue(std::locale(mylocale));
	ss >> std::get_time(&startDate, "%Y-%m-%dT%H:%M:%S");

	//std::istringstream ss("");
	//startDate = std::get_time(&t, "%Y-%b-%d %H:%M:%S");

	cout << "Welcome to the SCRUM Scheduler, schedule is 9-17, Monday to Friday" << endl;
	// cout << "Please introduce the initial date of the project" << endl;
	if (ss.fail()) {
		std::cout << "Parse failed\n";
	}
	else {
		std::cout << "Initial date of the project is " << std::put_time(&startDate, "%Y-%m-%dT%H:%M:%S") << endl;
	}
	// cout << "Please introduce the estimated duration of the project" << endl;
	cout << "Estimated duration of the project is " << numberOfWeeks << " weeks" << endl;

	Task Task1("UI", std::vector<int>{6}, std::vector<std::string>{"Designer"}, std::vector<std::string>{"-"});
	Task Task2("BackEnd", std::vector<int>{10, 12, 10}, std::vector<std::string>{"CTO", "CTO", "CPO"}, std::vector<std::string>{"-"});
	Task Task3("Marketing", std::vector<int>{8, 4}, std::vector<std::string>{"Designer", "CPO"}, std::vector<std::string>{"UI", "BackEnd"});

	Employee Employee1("Rodrigo", "CEO");
	Employee Employee2("Arturo", "CTO");
	Employee Employee3("Sergio", "CFO");

	/* Writing CSV test */

	// Make three vectors
	std::vector<string> vec1{ "Rodrigo", "Arturo", "Sergio" };
	std::vector<string> vec2{ "CEO", "CTO", "CPO" };
	std::vector<string> vec3{ "9:00-17:00|9:00-15:00", "9:00-17:00|9:00-15:00", "9:00-17:00|9:00-15:00" };

	// Wrap into a vector
	std::vector<std::pair<std::string, std::vector<string>>> values = { {"Name", vec1}, {"Role", vec2}, {"Schedule", vec3} };

	// Write the vector to CSV
	writeCSV("CSVtrial.csv", values);

	/* Reading CSV test */

	// Read both CSVs
	std::vector<std::pair<std::string, std::vector<string>>> employeesData = readCSV("employees.csv");
	std::vector<std::pair<std::string, std::vector<string>>> tasksData = readCSV("tasks.csv");

	// We print both CSVs
	//cout << "***Printing employee Data***" << endl;
	//cout << endl;
	//printCSV(employeesData);
	//cout << "*////////////////////*" << endl;
	//cout << endl;
	//cout << "***Printing tasks Data***" << endl;
	//cout << endl;
	//printCSV(tasksData);
	//cout << "*////////////////////*" << endl;
	//cout << endl;

	std::vector<Employee> employeeList = fillEmployees(employeesData);
	printEmployees(employeeList);

	cout << endl;
	cout << "*////////////////////*" << endl;
	cout << endl;

	std::vector<Task> taskList = fillTasks(tasksData);
	printTasks(taskList);

	grounding(employeeList, taskList);
	printVariables();

	// Number of values = weeks*days*hours*employees
	printDomains();

	/*int numThreads = 2;
	int variablesPerThread = VARIABLES.size() / numThreads;
	int variablesLastThread = VARIABLES.size() % numThreads;
	int counter = 0;
	vector<std::thread::id> threadIDs;

	for (Variable VARIABLE : VARIABLES)
	{

		vector<Variable> inputVariables;
		for (size_t i = 0; i < numThreads; i++)
		{
			inputVariables.push_back(VARIABLE);

		}
		std::thread thread (populateMutexParallel, inputVariables);
		threadIDs.push_back(thread.get_id());
		counter++;
	}

	for (std::thread::id id : threadIDs)
	{

	}*/

	populateMutex();

	/* We create a vector of States with unassigned Variables */
	vector<State> stateAssignments = createStateVector();

	/* DFS is started */
	cout << "Beginning Search algorithm(s)" << endl;
	cout << "------------" << endl;

	// Get the first solution | FAST
	//bool DFSResult = true;
	//bool DFSResult = GreedySearch(stateAssignments);
	vector<vector<State>> solutions = BeamSearch(stateAssignments, 3);
	bool DFSResult = DFS(stateAssignments);
	solutions.push_back(stateAssignments);

	for (vector<State> SOLUTION : solutions)
	{
		printStateAssignments(SOLUTION);
		cout << "--" << endl;
	}
	// Get best solution | TAKES HOURS
	//DFSComplete(stateAssignments);

	if (DFSResult == true)
	{
		cout << "*////////////////////*" << endl;
		cout << endl;
		cout << "***DFS SUCCESS***" << endl;
		cout << endl;
	}
	else {
		cout << "*////////////////////*" << endl;
		cout << endl;
		cout << "***DFS FAILURE***" << endl;
		cout << "Not enough time to finish the project perhaps?" << endl;
		cout << endl;
	}

	SOLUTION = stateAssignments;

	// We insert the SCRUM dailies into the solution if they're enabled
	if (scrumTime != -1)
	{
		stateAssignments = insertSCRUM(stateAssignments);
	}

	// We write the solution into a CSV file
	writeSolution(solutions);


	cout << endl;
	cout << "*////////// Program ended //////////*" << endl;
	cout << "" << endl;
}