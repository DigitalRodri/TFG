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
#include <set>
#include <map>
using namespace std;

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
				//cout << "Dependency " << dependencies.at(i) << " compared to " << task << endl;
				return true;
			}
		}

		return false;
	}

};

class Employee {
public:
	std::string name;
	std::string role;
	std::vector<std::string> schedule;

public:
	Employee() {
	}

	Employee(std::string name, std::string role, std::vector<std::string> schedule) {
		this->name = name;
		this->role = role;
		this->schedule = schedule;
	}

	void printEmployee() {

		std::cout << this->name << ' ';
		std::cout << this->role << ' ';
		for (size_t j = 0; j < this->schedule.size(); j++)
		{
			std::cout << this->schedule.at(j) << ' ';
		}
		cout << endl;

	}

};

class DomainValue {
public:
	short int id = 0;
	Employee employee;
	std::time_t date{};

public:
	DomainValue() {
	}

	DomainValue(int id,  Employee employee, std::time_t date) {
		this->id = id;
		this->employee = employee;
		this->date = date;
	}

	void printDomainValue() const {

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
	short int id = 0;
	Task task;
	std::set<DomainValue> domains;

public:
	Variable() {
	}

	Variable(int id, Task task, Employee employee, std::set<DomainValue> domains) {
		this->id = id;
		this->task = task;
		this->domains = domains;
	}

	void printVariable() const {

		std::cout << "--- " << this->task.name << "-" << this->id << " ---" << endl;

		for (auto DOMAIN1 : domains)
		{
			DOMAIN1.printDomainValue();
		}
		cout << endl;
	}

	int getTaskDuration(string role) {
		for (size_t i = 0; i < this->task.compatibilities.size(); i++)
		{
			if (this->task.compatibilities.at(i).compare(role) == 0)
			{
				return this->task.durations.at(i);
			}

			else return 0;
		}
	}
};

// Overloading for sets of Variable and DomainValue
bool operator< (const Variable& obj1, const Variable& obj2) {

	return obj1.id < obj2.id;

}

bool operator< (const DomainValue& obj1, const DomainValue& obj2) {

	return obj1.id < obj2.id;

}

// Static variables and those taken from input
static size_t labourDays = 5;
static size_t startHour = 9;
static size_t finishHour = 17;
static size_t numberOfWeeks = 2;
static std::vector<std::pair<Task, std::pair<int, int>>> taskIDs;
static std::string lunchTime;
static std::string scrumTime;
static std::tm startDate;
static std::set<Variable> VARIABLES;
static std::set<DomainValue> DOMAINS;
static std::map<DomainValue, vector<DomainValue*>> MUTEX;

void writeCSV(std::string filename, std::vector<std::pair<std::string, std::vector<string>>> dataset) {
	// Make a CSV file with one or more columns of integer values
	// The input file (dataset) is composed of a vector that containts the columns
	// Each column of data is represented by a pair <column string, column data (vector)> as std::pair<std::string, std::vector<int>>
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

	cout << "Printing combinations" << endl;
	cout << "------------" << endl;

	for (auto VARIABLE : VARIABLES)
	{
		VARIABLE.printVariable();
		cout << "Size: " << VARIABLE.domains.size() << endl;
	}

	cout << endl;
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
				break;
			case 2:
				tokenStream.str(CSV.at(i).second.at(j));
				while (std::getline(tokenStream, segment, delimiter))
				{
					employeeList.at(j).schedule.push_back(segment);
				}
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

void grounding(std::vector<Employee> employeeList, std::vector<Task> taskList) {

	cout << "Grounding of tasks" << endl;
	cout << "------------" << endl;

	// Auxiliary variables
	Variable temporalVariable;
	DomainValue temporalDomain;
	std::time_t temporalDate = mktime(&startDate);
	std::pair<int, int> temporalPair1;
	std::pair<Task, std::pair<int, int>> temporalPair2;

	//cout << "Time in epoch: " << temporalDate << endl;
	int iterations = 0;

	// For each Task
	for (size_t i = 0; i < taskList.size(); i++) {

		/*cout << "------------" << endl;
		cout << "Starting task" << endl;
		cout << "------------" << endl;*/

		// We get the starting ID of the task
		temporalPair1.first = iterations;

		// We assign the task
		temporalVariable.task = taskList.at(i);
		temporalVariable.id = i;

		// For each Employee
		for (size_t j = 0; j < employeeList.size(); j++)
		{
			// For as many weeks as the project takes
			for (size_t w = 0; w < numberOfWeeks; w++) {

				// For each week
				for (size_t k = 0; k < labourDays; k++) {

					// For each hour of a day
					for (size_t l = 0; l < (finishHour - startHour); l++) {

						// If the task is compatible with the employee, we generate the combination
						for (size_t c = 0; c < taskList.at(i).compatibilities.size(); c++)
						{
							if (taskList.at(i).compatibilities.at(c).compare(employeeList.at(j).role) == 0)
							{

								// We generate a combination of all of the factors
								temporalDomain.employee = employeeList.at(j);
								struct std::tm hora;
								localtime_s(&hora, &temporalDate);
								temporalDomain.date = temporalDate;
								temporalDomain.id = iterations;

								//cout << "Day: " << hora.tm_mday << endl;
								//cout << "Hour: " << hora.tm_hour << endl;

								temporalVariable.domains.insert(temporalDomain);

								DOMAINS.insert(temporalDomain);

								temporalDate += 3600;
								iterations++;
							}
						}
					}
					/*cout << "------------" << endl;
					cout << "Day ended" << endl;
					cout << "------------" << endl;*/
					temporalDate += 57600;
				}
				/*cout << "------------" << endl;
				cout << "Week ended" << endl;
				cout << "------------" << endl;*/
				temporalDate += 176400;

			}
			/*cout << "------------" << endl;
			cout << "Employee ended" << endl;
			cout << "------------" << endl;*/
			temporalDate = mktime(&startDate);

		}
		/*cout << "------------" << endl;
		cout << "Task ended" << endl;
		cout << "------------" << endl;*/

		// We get the finishing ID of the task
		temporalPair1.second = iterations;
		temporalPair2.first = taskList.at(i);
		temporalPair2.second = temporalPair1;

		// We push the data into the set
		VARIABLES.insert(temporalVariable);
		//cout << "Set size: " << temporalVariable.domains.size() << endl;
		taskIDs.push_back(temporalPair2);

		// We clean the temporal variable
		temporalVariable.domains.clear();

		cout << "Task: " << taskList.at(i).name << " IDs " << taskIDs.at(i).second.first << " to " << taskIDs.at(i).second.second << endl;

	}
	//cout << "Iterations: " << iterations << endl;
	cout << endl;
	cout << "All tasks have been grounded" << endl;
	cout << endl;
}

void mutex() {
	cout << "Calculating mutex" << endl;
	cout << "------------" << endl;

	int counter = 0;
	std::pair<DomainValue, vector<DomainValue*>> temporalMutexPair;
	vector<DomainValue*> temporalMutexVector;

	//std::set<Variable>::iterator variableIterator = VARIABLES.begin();
	
	// For each Task
	for (auto VARIABLE1 : VARIABLES)
	{	
		cout << "Variable1 is " << VARIABLE1.task.name << "-" << counter << endl;
		//std::set<DomainValue>::iterator domainIterator = (*variableIterator).domains.begin();
		
		// For each value in the domain of the task
		for (auto VALUE1 : VARIABLE1.domains)
		{
			// We compare it against the domain of the rest of the tasks
			// Each value is mutex with the rest of its domain
			for (auto VARIABLE2 : VARIABLES)
			{
				for (auto VALUE2 : VARIABLE2.domains)
				{
					// We check that we are not in the same task
					if (VARIABLE1.task.name.compare(VARIABLE2.task.name) != 0)
					{
						// We add all the combinations that happen at that time
						if (VALUE1.date == VALUE2.date)
						{
							temporalMutexVector.push_back(&VALUE2);
							counter++;
						}
						// We add all combinations of the same employee at the duration of the task
						// Duration of the task is 3600secs*hours
						else if ((VALUE1.employee.name.compare(VALUE2.employee.name) == 0) && 
							(VALUE1.date + (static_cast<unsigned __int64>(VARIABLE1.getTaskDuration(VALUE1.employee.role))) * 3600) >= VALUE2.date)
						{
							temporalMutexVector.push_back(&VALUE2);
							counter++;
						}
						// We add all the combinations that have tasks that depend on the actual one and are done before it
						else if (VARIABLE2.task.dependsOn(VARIABLE1.task.name) && VALUE1.date > VALUE2.date) {
							temporalMutexVector.push_back(&VALUE2);
							counter++;
						}
					}
				}
			}

			//cout << "Mutex size of " << VARIABLE1.task.name << "-" << counter << endl;
			counter = 0;
			temporalMutexPair.first = VALUE1;
			temporalMutexPair.second = temporalMutexVector;
			
			MUTEX.insert(temporalMutexPair);

		}
		//cout << "Mutex size" << MUTEX.at().size() << endl;
	}

}

// Receives the number of tasks and a list with all the possible combinations to iterate over them
void DFS(std::queue<Variable> variableQueue, std::map<DomainValue, bool> visitedNodes) {

	cout << "Executing DFS algorithm" << endl;
	cout << "------------" << endl;

	// Queue for the solution we have 
	std::queue<Variable> solutionQueue;

	while (!variableQueue.empty())
	{
		Variable solutionVariable = variableQueue.front();
		variableQueue.pop();

		for (auto VALUE : solutionVariable.domains)
		{
			Variable auxVar;
			auxVar.task = solutionVariable.task;
			auxVar.domains.insert(VALUE);
			variableQueue.push(auxVar);
		}
		DFS(variableQueue, visitedNodes);
	}

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

	Employee Employee1("Rodrigo", "CEO", std::vector<std::string>{"9:00-17:00", "9:00-15:00"});
	Employee Employee2("Arturo", "CTO", std::vector<std::string>{"9:00-17:00", "9:00-15:00"});
	Employee Employee3("Sergio", "CFO", std::vector<std::string>{"9:00-17:00", "9:00-15:00"});

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
	cout << "***Printing employee Data***" << endl;
	cout << endl;
	printCSV(employeesData);
	cout << "*////////////////////*" << endl;
	cout << endl;
	cout << "***Printing tasks Data***" << endl;
	cout << endl;
	printCSV(tasksData);
	cout << "*////////////////////*" << endl;
	cout << endl;

	std::vector<Employee> employeeList = fillEmployees(employeesData);
	printEmployees(employeeList);

	cout << endl;
	cout << "*////////////////////*" << endl;
	cout << endl;

	std::vector<Task> taskList = fillTasks(tasksData);
	printTasks(taskList);

	grounding(employeeList, taskList);
	//printVariables();

	mutex();

	/* An instance of a Variable with a Value acts as a node of the graph we travel */

	// List where we store if a node has been visited
	std::map<DomainValue, bool> visitedNodes;

	// We assign the list as not visited (false)
	for (auto VALUE : DOMAINS)
	{
		std::pair<DomainValue, bool> pair(VALUE, false);
		visitedNodes.insert(pair);

	}

	// Queue for input VARIABLES
	std::queue<Variable> variableQueue;

	for (auto VARIABLE : VARIABLES)
	{
		variableQueue.push(VARIABLE);
	}

	DFS(variableQueue, visitedNodes);

	cout << endl;
	cout << "*////////// Program ended //////////*" << endl;
	cout << "" << endl;
}