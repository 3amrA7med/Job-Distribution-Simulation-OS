#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include<cstring>
#include <fstream>
using namespace std;

// Global Variables

int time_interval; // Time interval for every run
int dp[100][500][1000]; 
int maxJobs; // Max number of jobs during the whole program
vector<int> currentState; // Current state of the processed jobs through the whole runtime
vector<int> tempState; // Temp state for every run
int maxCount; // Max count of instructions for every run 
int count_inst; // Instruction count used in  recursionJobDistribution

// Functions

void readInput(vector<vector<int>>&);
int jobDistribution(vector<vector<int>>&);
int recursionJobDistribution(vector<vector<int>>&, int, int, int, vector<int>);
void updateJobs(vector<vector<int>>&, vector<vector<int>>&);
bool endOfJobs(vector<vector<int>>&);

///////////////////////////////
////////////Main///////////////
///////////////////////////////
int main() {
	vector<vector<int>> jobs, unprocessedJobs;
	int option;
	bool isFinished;
	readInput(jobs);
	unprocessedJobs = jobs;
	cout << "Choose '1' to enter time interval '2' to exit: ";
	cin >> option;
	while (true) {
		switch (option)
		{
		case 1:
			cout << "Enter the time interval : ";
			cin >> time_interval;
			cout << "Total number of processed instructions : " << jobDistribution(unprocessedJobs) << endl;
			cout << "Number of instrustions executed per job(status): ";
			updateJobs(jobs, unprocessedJobs); // Update states and unprocessed jobs vector
			isFinished = endOfJobs(jobs);
			for (int i = 0; i < currentState.size(); i++)
				cout << " " << currentState[i];
			break;
		case 2:
			return 0;
			break;
		default:
			return 0;
			break;
		}
		// Check if all the jobs have been processed
		if (isFinished)
		{
			cout << "\nAll the jobs have been processed, Closing program.." << endl;
			return 0;
		}
		else
		{
			cout << "\nChoose '1' to enter time interval and '2' to exit : ";
			cin >> option;
		}
	}

	return 0;
}
//////////////////////////////////////////////////
/////////////////FUNCTIONS////////////////////////
//////////////////////////////////////////////////

// Function which reads input
void readInput(vector<vector<int>>& jobs) {
	cout << "Enter the number of jobs: ";
	cin >> maxJobs;
	vector<int>tempJob;
	// Loop to get the instructions of every job
	for (int i = 0; i < maxJobs; i++) {
		int timeIntervalOfInstruction, numberOfInstrustions;
		cout << "Enter the number of instructions for job #" << i+1 << " :";
		cin >> numberOfInstrustions;
		cout << "Enter time interval needed for each instruction : ";
		for (int J = 0; J < numberOfInstrustions; J++) {
			cin >> timeIntervalOfInstruction;
			tempJob.push_back(timeIntervalOfInstruction);
		}
		// Initialize current state and temp state to zeros. as every job is unprocessed at first
		currentState.push_back(0);
		tempState.push_back(0);
		jobs.push_back(tempJob);
		// Clear vector temp job to prepare it for the next job.
		tempJob.clear();
	}
	maxCount = count_inst = 0;

}

// Checks if all of the jobs have finished their instructions.
bool endOfJobs(vector<vector<int>>& jobs) {
	bool isFinished = true;
	for (int i=0;i<currentState.size();i++)
	{
		if (currentState[i] != jobs[i].size())
		{
			isFinished = false;
			break;
		}
			
	}
	return isFinished;
}

// Update the processed jobs to exclude them from the unprocessed vector
// Also update the current state for the jobs
void updateJobs(vector<vector<int>>& jobs, vector<vector<int>>& unprocessedJobs)
{
	// Update state of each job. 
	for (int i = 0; i < currentState.size(); i++)
		currentState[i] += tempState[i];

	unprocessedJobs.clear();
	int index;
	vector<int>tempJob;
	bool isUpdated = false;
	// Update the unprocessed jobs
	for (int i = 0; i < maxJobs; i++) {

		for (int j = currentState[i]; j < jobs[i].size(); j++) {
			tempJob.push_back(jobs[i][j]);
			
		}
		unprocessedJobs.push_back(tempJob);
		tempJob.clear();
	}
	tempState.clear();
}

// Prepare needed data for the first recursion call
int jobDistribution(vector<vector<int>>& jobs)
{
	vector<int>initialState;
	// Initial state of the unprocessed jobs is zero as there is no jobs processed.
	for (int i = 0; i < maxJobs; i++)
		initialState.push_back(0);
	maxCount = count_inst = 0;
	memset(dp, -1, 100 * 1000 * 500);
	return recursionJobDistribution(jobs, 0, 0, time_interval, initialState);
}

// Recursion function that calculate the max number of instructions can be executed in the unprocessed jobs vector
int recursionJobDistribution(vector<vector<int>>& jobs, int i, int j, int remainingTime, vector<int>state)
{
	// Exist recursion if there is no other jobs or there is no remaining time. 
	if (i >= jobs.size() || remainingTime == 0)
	{
		// Count the number of instructions executed for this path and compare it to the max count
		int count = 0;
		for (int index = 0; index < state.size(); index++)
			count += state[index];
		if (count > maxCount)
		{
			maxCount = count;
			tempState = state;
		}
		return 0;
	}
	// If the instrustions finished from one job go to the following job.
	if (j >= jobs[i].size())
		return recursionJobDistribution(jobs, i + 1, 0, remainingTime, state);
	if (dp[i][j][remainingTime] != -1)
		return dp[i][j][remainingTime];
	if (remainingTime - jobs[i][j] < 0)
		return recursionJobDistribution(jobs, i + 1, 0, remainingTime, state);
	else {
		vector<int>newState = state;
		newState[i]++; // Add the instruction count to the current path
		count_inst = max(recursionJobDistribution(jobs, i + 1, 0, remainingTime, state),
			recursionJobDistribution(jobs, i, j + 1, remainingTime - jobs[i][j], newState) + 1);
	}
	dp[i][j][remainingTime] = count_inst;
	return count_inst;
}