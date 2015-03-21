#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/sem.h>
#include <cmath>
#include <ctime>
#include <unistd.h>	
#include <list>

using namespace std;



#define S 512

struct lifetimes{
	int id;
	int lifetime;
	lifetimes(int id_,int life_):id(id_),lifetime(life_){}
};


struct memory{
	bool free_space;
	int start;
	int end;
	int size;
	int id;
	bool left;				//just for buddy algorithm
	memory(bool free_space_ = true,int start_ = 1,int end_= S,int id_ = -1){		//constructor
		start = start_;				//including start/end bit
		end = end_;
		id = id_;
		free_space = free_space_;
		size = end - start + 1; 
	}

};

struct queue{
	int id;
	int size;	
	queue(int x,int y):id(x),size(y){}
};

struct queue_vp_stoper{
	int id;
	int entered;
	int poped;
	int must_stay_in_mem;
	bool in_queue;
};

struct vp_stop_collector{
	int id;
	int life_left_for_stop;
};


void down(int semid);
void up(int semid);	
void print_memory (list<struct memory> mylist);
void poisson(bool param[]);
void best_fit(int id,int size,int vp_start,	list <struct memory> *memory_list,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed);
void worst_fit(int id,int size,int vp_start, list <struct memory> *mylist,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed);
void buddy(int id,int size,int vp_start, list <struct memory> *mylist,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed);
void print_queue(list<struct queue> mylist);
void print_lifetime(list<struct lifetimes> mylist);
void decrease_lifetime_by_1(list <struct lifetimes>* mylist,int id);
void print_queue_stoper(list <struct queue_vp_stoper> mylist);
bool check_if_is_in_w8ing_queue(int id,list <struct queue_vp_stoper> *queue_vp_stoper,int i);
void decrease_time_for_vp_stop(list <struct queue_vp_stoper> *mylist);
void mesh_timh(list<struct memory> mylist,bool);
