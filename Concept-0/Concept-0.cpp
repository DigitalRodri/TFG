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
	long id = 0;
	Employee employee;
	std::time_t date{};

public:
	DomainValue() {
	}

	DomainValue(long id, Employee employee, std::time_t date) {
		this->id = id;
		this->employee = employee;
		this->date = date;
	}

	void printDomainValue() {

		std::cout << this->id << ' ';
		std::cout << this->employee.name << ' ';
		struct std::tm hora;
		localtime_s(&hora, &date);
		std::cout << std::put_time(&hora, "%Y-%m-%dT%H:%M:%S") << endl;
		cout << endl;
	}

};

class TaskVariable {
public:
	long id = 0;
	Task task;
	std::vector<DomainValue> domains;
	std::vector<std::pair<long, long>> mutex;

public:
	TaskVariable() {
	}

	TaskVariable(long id, Task task, Employee employee, std::vector<DomainValue> domains, std::vector<std::pair<long, long>> mutex) {
		this->id = id;
		this->task = task;
		this->domains = domains;
		this->mutex = mutex;
	}

	void printTaskVariable() {

		std::cout << "--- " << this->id << ' ';
		std::cout << this->task.name << " ---" << endl;
		for (size_t i = 0; i < this->domains.size(); i++)
		{
			domains.at(i).printDomainValue();
		}
		for (size_t i = 0; i < mutex.size(); i++)
		{
			std::cout << this->mutex.at(i).first << " with " << this->mutex.at(i).second << ' ';
		}
		cout << endl;
	}

	void printMutex() {

		cout << "Printing mutex list of  " << this->id << endl;
		for (size_t i = 0; i < mutex.size(); i++)
		{
			std::cout << this->mutex.at(i).first << " with " << this->mutex.at(i).second << ' ';
		}
		cout << endl;
	}

	void printMutexSize() {

		cout << "Mutex size of " << this->id << " is: " << this->mutex.size() << endl;
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


// Static variables and those taken from input
static size_t labourDays = 5;
static size_t startHour = 9;
static size_t finishHour = 17;
static size_t numberOfWeeks = 6;
static std::vector<std::pair<Task, std::pair<int, int>>> taskIDs;
static std::string lunchTime;
static std::string scrumTime;
static std::tm startDate;

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

void printTaskVariables(std::vector<TaskVariable> possibleCombinations) {

	cout << "Printing combinations" << endl;
	cout << "------------" << endl;

	for (size_t i = 0; i < possibleCombinations.size(); i++)
	{
		possibleCombinations.at(i).printTaskVariable();
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

std::vector<TaskVariable> grounding(std::vector<Employee> employeeList, std::vector<Task> taskList) {

	cout << "Grounding of tasks" << endl;
	cout << "------------" << endl;

	// Auxiliary variables
	std::vector<TaskVariable> TaskVariables;
	TaskVariable temporalTaskVariable;
	DomainValue temporalDomain;
	std::time_t temporalDate = mktime(&startDate);
	std::pair<int, int> temporalPair1;
	std::pair<Task, std::pair<int, int>> temporalPair2;

	//cout << "Time in epoch: " << temporalDate << endl;
	int iterations = 0;
	int taskCounter = 0;

	// For each Task
	for (size_t i = 0; i < taskList.size(); i++) {

		/*cout << "------------" << endl;
		cout << "Starting task" << endl;
		cout << "------------" << endl;*/

		// We get the starting ID of the task
		temporalPair1.first = iterations;

		// We assign the task
		temporalTaskVariable.task = taskList.at(i);
		temporalTaskVariable.id = taskCounter;

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

								//cout << "Day: " << hora.tm_mday << endl;
								//cout << "Hour: " << hora.tm_hour << endl;

								temporalDomain.date = temporalDate;
								temporalDomain.id = iterations;
								temporalTaskVariable.domains.push_back(temporalDomain);

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

		taskCounter++;

		// We push the data into the vector
		TaskVariables.push_back(temporalTaskVariable);
		taskIDs.push_back(temporalPair2);

		// We clean the temporal variable
		temporalTaskVariable.domains.clear();

		cout << "Task: " << taskList.at(i).name << " IDs " << taskIDs.at(i).second.first << " to " << taskIDs.at(i).second.second << endl;

	}
	//cout << "Iterations: " << iterations << endl;
	cout << endl;
	cout << "All tasks have been grounded" << endl;
	cout << endl;
	return TaskVariables;
}

std::vector<TaskVariable> mutex(std::vector<TaskVariable> TaskVariables) {
	cout << "Calculating mutex" << endl;
	cout << "------------" << endl;

	int counter = 0;
	std::pair<int, int> temporalPair;

	// For each Task
	for (size_t i = 0; i < TaskVariables.size(); i++)
	{
		// For each value in the domain of the task
		for (size_t j = 0; j < TaskVariables.at(i).domains.size(); j++)
		{
			// We compare it against the domain of the rest of the tasks
			// Each value is mutex with the rest of its domain
			for (size_t t = 0; t < TaskVariables.size(); t++)
			{
				for (size_t d = 0; d < TaskVariables.at(t).domains.size(); d++)
				{
					// We check that we are not in the same task
					if (TaskVariables.at(i).id != TaskVariables.at(t).id)
					{
						// We add all the combinations that happen at that time
						if (TaskVariables.at(i).domains.at(j).date == TaskVariables.at(t).domains.at(d).date)
						{
							temporalPair.first = TaskVariables.at(i).domains.at(j).id;
							temporalPair.second = TaskVariables.at(t).domains.at(d).id;
							TaskVariables.at(i).mutex.push_back(temporalPair);
							counter++;
						}
						// We add all combinations of the same employee at the duration of the task
						// Duration of the task is 3600secs*hours
						else if ((TaskVariables.at(i).domains.at(j).employee.name.compare(TaskVariables.at(t).domains.at(d).employee.name) == 0) &&
							(TaskVariables.at(i).domains.at(j).date + (static_cast<unsigned __int64>(TaskVariables.at(i).getTaskDuration(TaskVariables.at(i).domains.at(j).employee.role)) * 3600)) 
							>= TaskVariables.at(t).domains.at(d).date)
							{
							temporalPair.first = TaskVariables.at(i).domains.at(j).id;
							temporalPair.second = TaskVariables.at(t).domains.at(d).id;
							TaskVariables.at(i).mutex.push_back(temporalPair);
							counter++;
						}
						// We add all the combinations that have tasks that depend on the actual one and are done before it
						else if (TaskVariables.at(t).task.dependsOn(TaskVariables.at(i).task.name) && TaskVariables.at(i).domains.at(j).date > TaskVariables.at(t).domains.at(d).date) {
							temporalPair.first = TaskVariables.at(i).domains.at(j).id;
							temporalPair.second = TaskVariables.at(t).domains.at(d).id;
							TaskVariables.at(i).mutex.push_back(temporalPair);
							counter++;
						}
					}
				}
			}

			//cout << "Mutex size of IDs " << TaskVariables.at(i).id << "-" << TaskVariables.at(i).domains.at(j).id << ": " << counter << endl;
			counter = 0;

		}

		TaskVariables.at(i).printMutexSize();

	}

	return TaskVariables;

}

// Receives the number of tasks and a list with all the possible combinations to iterate over them
void DFS(int taskNumber, std::vector<TaskVariable> TaskVariables) {

	cout << "Executing DFS algorithm" << endl;
	cout << "------------" << endl;

	/* A pair of TaskVariable and DomainValue act as nodes of the graph we travel */
	/* Each pair is a node that is visited */ 

	// List where we store if a pair has been visited
	std::vector<std::pair<std::pair<int, int>, bool>> visitedNodes;

	// We assign the list as not visited (false)
	//for (size_t i = 0; i < TaskVariables.size(); i++)
	//{
		//AssignedTask assignedTask(possibleCombinations.at(i).first, possibleCombinations.at(i).second);
		//std::pair<AssignedTask, bool> pair(assignedTask, false);
		//visitedNodes.push_back(pair);
	//}

	// Queue for the TaskVariable being traversed
	std::queue<TaskVariable> taskQueue;

	// Queue for the DomainValue being traversed
	std::queue<DomainValue> domainQueue;

	// We insert the first assignedTask of the queue and mark it as visited (true)
	std::pair<int, int> firstTask(TaskVariables.at(0).id, TaskVariables.at(0).domains.at(0).id);
	std::pair<std::pair<int, int>, bool> firstPairPair(firstTask, true);
	taskQueue.push(TaskVariables.at(0));
	domainQueue.push();
	traversalQueue.push(firstPairPair);

	// We check if list is empty
	//if (traversalQueue.empty()) return;

	// We initialize the traversal
	while (!traversalQueue.empty()) {

		// Get task from queue and remove it
		TaskVariable node = taskQueue.front();
		taskQueue.pop();
		for (size_t i = 0; i < visitedNodes.size(); i++)
		{
			if (visitedNodes.at(i).first.task.name.compare(node.task.name) == 0)
			{
				visitedNodes.at(i).second = true;
			}
		}
		//cout << "Traversing node: " << node.employee.name << " & " << node.task.name << endl;

		// We get the nodes adjacent to the current one and add them to the queue
		for (size_t i = 0; i < visitedNodes.size(); i++)
		{
			// If the Node is not the current task and has not been visited
			if (visitedNodes.at(i).first.task.name.compare(node.task.name) == 1 && visitedNodes.at(i).second == false) {

				for (size_t j = 0; j < visitedNodes.at(i).first.task.dependencies.size(); j++)
				{
					for (size_t k = 0; k < visitedNodes.size(); k++)
					{
						// If the dependent nodes have been visited
						if (visitedNodes.at(i).first.task.dependencies.at(j).compare(visitedNodes.at(k).first.task.name) == 0
							&& visitedNodes.at(i).first.task.dependencies.at(j).compare("-") != 0) {
							// We add the node to the queue
							cout << visitedNodes.at(i).first.task.name << " added to queue" << endl;
							traversalQueue.push(visitedNodes.at(i).first);
						}
					}

				}

			}
		}
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

	std::vector<TaskVariable> possibleCombinations = grounding(employeeList, taskList);
	//printTaskVariables(possibleCombinations);

	possibleCombinations = mutex(possibleCombinations);

	DFS(taskList.size(), possibleCombinations);

	cout << endl;
	cout << "*////////// Program ended //////////*" << endl;
	cout << "" << endl;
}