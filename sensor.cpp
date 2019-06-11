#include <stdio.h> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <pthread.h> 
#include <semaphore.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <mutex>
#include <limits.h>
#include <vector>
#include <fstream>

using namespace std;
#define NUM_THREADS 10

int max_temp, count_sem;
double result[10][20];

// Semaphores
sem_t sem_1;
sem_t sem_2;
sem_t sem_3;

void init_semaphore() {
    sem_init(&sem_1,0,0);
    sem_init(&sem_2,0,0);
    sem_init(&sem_3,0,0);
}


// Casting to int
int cast_to_int(string text) {
    int number;
    istringstream iss (text);
    iss >> number;
    return number;
}

vector<vector <int> > read_file(string file_name){
  vector<vector<int> > result1(10);
  ifstream myfile(file_name.c_str());
  for (int j = 0; j < 10; ++j)
  {
    string s;
    getline(myfile, s);
    int m = s.find(',');
    result1[j].push_back(cast_to_int(s.substr(0,m)));
    s = s.substr(m+2);
    for (int i = 0; i < 18; ++i)
    {
      m = s.find(',');
      result1[j].push_back(cast_to_int(s.substr(0,m)));
      s = s.substr(m+2);
    }
    result1[j].push_back(cast_to_int(s));
  }
  return result1;
}


void signal_input_sem() {
    for (int j=0; j<10; j++) {
        sem_post(&sem_3);
    } 
}


void normalize(vector<vector<int> > data) {
    for (int i=0; i<20; i++) {
        sem_wait(&sem_2);

        for (int j=0; j<10; j++) {
            result[j][i] = (double)data[j][i]/max_temp;
        }
        count_sem = 0;
        signal_input_sem();  
        sem_post(&sem_1);
    }
}



void func(vector<int> sensor) {
    for (int i=0; i<20; i++) {
        sem_wait(&sem_3);
        sem_wait(&sem_1);

        if (sensor[i] > max_temp) {
            max_temp = sensor[i];
        }
        if (count_sem < 9) {
            count_sem ++;
            sem_post(&sem_1);
        }
        else {
            sem_post(&sem_2);
        }
    }
    // normalize(temp_data);

}

void save_result() {
    fstream file;
    file.open("result.txt",ios::out);
    for (int i=0; i<10; i++) {
        for(int j = 0; j < 20; j++)
        {
            file << result[i][j] << ", ";
        }
        file << endl;
    }
    file.close();
}

int main()
{   
	//cout<<"m"<<endl;
    max_temp = 0;
    count_sem = 0;
    vector <vector <int> > temp_data;

    init_semaphore();

    temp_data = read_file("inputs.txt");

    // Creating threads
    thread myThreads[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++) {
        myThreads[i] = thread(func,temp_data[i]);
        sem_post(&sem_3);
    }
    sem_post(&sem_1);
    normalize(temp_data);
    for (int i = 0; i < 10; ++i)
    {
        myThreads[i].join();
    }
    save_result();

    // for (int i = 0; i < 10; ++i)
    // {
    // 	for (int j = 0; j < 20; ++j)
    // 	{
    // 		cout<< result[i][j] << ' ';
    // 	}
    // 	cout<<endl;
    // }

    return 0;
}