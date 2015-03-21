
#include "header.h"
#define t 15
#define T 2
#define D 300			//xronos prosomiwshs
#define S 512			//megethos mnimhs
#define SHMSZ  200
#define lo 5
#define hi 100
#define ALGORITHM 3		/*  1 (Best_fit),  2 (Worst_fit), 3(Buddy)  */

int main(void){
	FILE* fp;
	list <struct memory> memory_list;		//dhmhiourgia ths kenhs mas mnimhs megethus S
	struct memory my_memory;				//embelia-xrisimopoihte mono ston memory manager
	memory_list.push_back(my_memory);
	
	list <struct queue> queue;				//gia thn oura anamwnhs an einai gemath h mnimh ston (memory manager)
	list <struct queue_vp_stoper> queue_vp_stoper;	//gia diaxirish ths queue
	
	list <struct lifetimes> lifetimes;		//ston (Generator) apoklistika gia na stelnume swsta ta vp_stop
	
	void *request;
	void* received;	
	
	struct VP_start_stop{
		bool do_nothing;
		bool vp_start;
		int id;
		int size;
		int time_start;
	};
	struct VP_start_stop temp;
	request = &temp;
	
	
	bool do_nothing,ptr[D],vp_start;
	double random,forsec,l,u;
	int shmid,process,id,size,lifetime;
	key_t key = 1992;	
	int mutex = semget( (key_t) 9999, 1, IPC_CREAT | 0660); //simaforos
	int empty = semget( (key_t) 8888, 1, IPC_CREAT | 0660); //simaforos
	int semid = semget( (key_t) 1234, 1, IPC_CREAT | 0660); 	
	int diakimansh[S];
	int ginomeno;
	
	union senum{
		int val;
	};
	
	union senum arg,arg1;
	arg.val = 0;        
	arg1.val = 1;      

	semctl(mutex, 0, SETVAL, arg1);    //arxhkopoihsh simaforwn
	semctl(empty, 0, SETVAL, arg1);   
	semctl(semid, 0, SETVAL, arg); 
	
	/* Locate the segment.*/
	if ((shmid = shmget(key, SHMSZ, 0660| IPC_CREAT)) < 0)
		perror("shmget");
	
	fp = fopen("log.txt","w");
	if(ALGORITHM == 1)
		fprintf(fp,"Best Fit\n");
	else if(ALGORITHM == 2)
		fprintf(fp,"Worst Fit\n");
	else if( ALGORITHM ==3 )
		fprintf(fp,"Buddy\n");
	fprintf(fp,"Memory Size = %d",S);
	fprintf(fp,"\nSimulation Time = %d",D);
	fprintf(fp,"\n\n");
	fclose(fp); 

	process = fork();
	if(process == 0){	/*(generator)*/
		srand(time(NULL));
		int sum_ginomeno=0;
		poisson(ptr);			
		
		for(int i=0;i<D;i++){
			
			/*dimiourgia VP*/
			
			if(ptr[i] == true)										//an afto to defterolepto tha stilume VP_start h stop
				do_nothing = false;
			else
				do_nothing = true;
			
			vp_start = true;				
			id = 1000 + i;
			random= (hi-lo+1) *(double)rand()/RAND_MAX  + lo;		//size from [lo,hi] 
			size = random;
		
			random=50*(double)rand()/RAND_MAX;						//ek8etikh aktanomh mexri 55 ke oxi 99
			u = random/100;
			l=-10;		
			forsec = log(u)/l;			
			lifetime = random+1;
		
			struct lifetimes life(id,lifetime);
			
			decrease_lifetime_by_1(&lifetimes,id);
			
			list<struct lifetimes>::iterator current;	
			current = lifetimes.begin();

			while(current != lifetimes.end()){							//psaxnw gia VP_stop
				if(current->lifetime == 0){
					vp_start = false;
					do_nothing = false;
					id = current->id;
					current = lifetimes.erase(current);
					fp = fopen("log.txt","a");
					fprintf(fp,"Time = %d\n",i);
					fprintf(fp,"I just sent a (fake or correct) VP_STOP wtih ID : %d\n",id);
					fclose(fp);
					
					//cout<<"Time = "<<i<<endl;
					//cout<<"(fake or correct) VP_STOP wtih ID : "<<id<<endl;
					break;
				}
				current++;
			}
			
			if(do_nothing == false && vp_start == true){
				if(do_nothing == false)									//an exume start nika to stop
				lifetimes.push_back(life);
					fp = fopen("log.txt","a");
					fprintf(fp,"Time = %d\n",i);
					fprintf(fp,"size = %d\n",size);
					fprintf(fp,"lifetime = %d\n",lifetime);
					fclose(fp);		
					sum_ginomeno+= size * lifetime;						
			//	cout<<"Time = "<<i<<endl;
			//	cout<<"size = "<<size<<endl;
			//	cout<<"lifetime = "<<lifetime<<endl;
			}

			down(empty);
			down(mutex);
			
			if( (request = shmat(shmid, NULL, 0)) == (char *) -1)
				perror("shmat");	
				
			((struct VP_start_stop *)request)->id = id;					//dimiourgia vp_start/vp_stop analoga
			((struct VP_start_stop *)request)->size = size;
			((struct VP_start_stop *)request)->vp_start = vp_start;
			((struct VP_start_stop *)request)->do_nothing = do_nothing;	
			((struct VP_start_stop *)request)->time_start = i;
			
			
				
			up(mutex);
			up(semid);
		if( i == D-1){
			FILE *fp;
			fp=fopen("log.txt","a");
			fprintf(fp,"*******************************\n");
			fprintf(fp," GINOMENO SHORE = %f\n",((double)sum_ginomeno)/(D*S));	
			fprintf(fp,"*******************************\n");
			fclose(fp);
		}
		}
	
	exit(0);	
	}
	else{		/*(memory manager)*/
	
		int rec_size,rec_id,start_time;
		bool rec_vp_start;
		
		
		for(int i=0;i<D;i++){
			down(semid);
			down(mutex);
			bool Placed = false;
		
			if ((received = shmat(shmid, NULL, 0)) == (char*) -1)
				perror("shmat");
			
			
			if( ((struct VP_start_stop *)received)->do_nothing == false){	//an prepei na ginei kati
				rec_size = ((struct VP_start_stop *)received)->size;
				rec_id = ((struct VP_start_stop *)received)->id;
				rec_vp_start = ((struct VP_start_stop *)received)->vp_start;
				start_time = ((struct VP_start_stop *)received)->time_start;
							
				if(ALGORITHM == 1 )
					best_fit(rec_id,rec_size,rec_vp_start,&memory_list,&queue,&queue_vp_stoper,start_time,&Placed);
				else if(ALGORITHM == 2 )
					worst_fit(rec_id,rec_size,rec_vp_start,&memory_list,&queue,&queue_vp_stoper,start_time,&Placed);
				else if(ALGORITHM == 3)
					buddy(rec_id,rec_size,rec_vp_start,&memory_list,&queue,&queue_vp_stoper,start_time,&Placed);
					
					
				print_memory(memory_list);
				print_queue(queue);
				//print_queue_stoper(queue_vp_stoper);
			}									//an brei keno defterolepo as prospathisi na balei aftes p perimenun sthn queue
			else if(queue.size() >0){
			//edw den me niazun oi metablites p tha perasw sthn sianrtisi ara bazw dikes m anagnoristikes afu tha apro ta dedomena apo queue
			//briskw thn diergasia sthn queue me to mikrotero megethos ke prospathw na thn balw memory
			
				list<struct queue>::iterator current;	
				int min = 1000000;						//bgenei kathe fora apo to queue h mikroterh dld afth p xoraei pio efkola xwris na epibarinei tis ales
				int my_id;
				bool Placeds = false;
				
				current = queue.begin();

				while(current != queue.end()){
					if(current->size < min ){
						min = current->size;
						my_id = current->id ;
					}
					current++;
				}
				
				if(ALGORITHM == 1 )
					best_fit(my_id,min,true,&memory_list,&queue,&queue_vp_stoper,start_time,&Placeds);		
				else if(ALGORITHM == 2 )	
					worst_fit(my_id,min,true,&memory_list,&queue,&queue_vp_stoper,start_time,&Placeds);		
				else if(ALGORITHM == 3)		
					buddy(my_id,min,true,&memory_list,&queue,&queue_vp_stoper,start_time,&Placeds);		
				
				if(Placeds == true ){			//an ebala ena stixio sthn mnimh apo thn queue tote bgalto apo oura etc

					list<struct queue>::iterator curr;
					list<struct queue_vp_stoper>::iterator stopper;
					
					curr = queue.begin();
					
					while(curr != queue.end()){
						if(curr -> id == my_id){
								curr = queue.erase(curr);
						}
						curr++;
					}					
												//episis alakse ke thn paralilh oura wste na ginun sosta vp stop meta
					stopper = queue_vp_stoper.begin();
					
					while(stopper != queue_vp_stoper.end()){
						if(stopper -> id == my_id){
								stopper->in_queue = false;
								stopper->poped = start_time;
						}
						stopper++;
					}			
		
		
		
					
				}	
			
			}
			//edw se kathe defterolepto anexartita me to ti tha ginei parapanw miwse kata 1 ton xrono p apomenun stis diergasies sto queue_vp_stopper nane sthn mnimh
			//kai sthn sinexia bgaltes stile vp stop an ftasei sto 0
			bool without_use=false;
			
			decrease_time_for_vp_stop(&queue_vp_stoper);
			
			list<struct queue_vp_stoper>::iterator stopper;				//kai edw tha elenksw an brw kapia me timh 0 na thn bgalw apo thn mnimh me xeirokinito vp_stop
			stopper = queue_vp_stoper.begin();
					
			while(stopper != queue_vp_stoper.end()){
				if(stopper -> must_stay_in_mem == 0 && stopper->in_queue == false){
						stopper->must_stay_in_mem = -2;					//wste na min ksanaelenx8ei
					
				if(ALGORITHM == 1 )
					best_fit(stopper->id,0,false,&memory_list,&queue,&queue_vp_stoper,start_time,&without_use);	//stile vp stop gia to katalilo id	
				else if(ALGORITHM == 2)
					worst_fit(stopper->id,0,false,&memory_list,&queue,&queue_vp_stoper,start_time,&without_use);	//stile vp stop gia to katalilo id					
				else if(ALGORITHM == 3)
					buddy(stopper->id,0,false,&memory_list,&queue,&queue_vp_stoper,start_time,&without_use);	//stile vp stop gia to katalilo id	
						
				}
				stopper++;
			}				
			
			
			up(mutex);
			up(empty);


		}
		
	}
	
mesh_timh(memory_list,false);
semctl(semid, 0, IPC_RMID,0);		//delete semaphore
semctl(mutex, 0, IPC_RMID,0);		//delete semaphore
semctl(empty, 0, IPC_RMID,0);		//delete semaphore
shmctl(shmid,0,IPC_RMID);			//delete the segment
	return 0;
	
}


