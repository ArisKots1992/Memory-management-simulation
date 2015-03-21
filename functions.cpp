#define D 300
#define t 5
#include "header.h"

void down(int semid){
        struct sembuf semopr;
        semopr.sem_num = 0;
        semopr.sem_op = -1;//down
        semopr.sem_flg = 0;
        semop(semid, &semopr, 1);
}
void up(int semid){
        struct sembuf semopr;
        semopr.sem_num = 0;
        semopr.sem_op = 1;//down
        semopr.sem_flg = 0;
        semop(semid, &semopr, 1);
}

void poisson(bool ptr[]){
	double random,u;
	double temp[D],sum=0;
	
	for(int i=0;i<D;i++)
		ptr[i] = false;	
	

	for(int i=0;i<D;i++){					//bazw D wste sthn xiroterh(adynath) periptwsh pou kathe 1 sec erxete diergasia
		random=99*(double)rand()/RAND_MAX;
	
	temp[i] = -t*log(random/100);	
	}
	int for_for;
	for(int i=0;i<D;i++){		
		sum+=temp[i];
		for_for = sum;
		i= for_for;
		
		ptr[i] = true;
	}
/*kati ligo diaforetiko
 *EDW SE KATHE DEFTEROLEPTO EXUME 18% pi8anothta na dimiourgithei diergasia
	for(int i=0;i<D;i++){
		random=99*(double)rand()/RAND_MAX;
		
		if(random < 18)
			ptr[i] = true;
		else 
			ptr[i] = false;
*/

/*	edw epilegume emeis to defterolepto
	for(int i=0;i<D;i++)
		ptr[i] = false;
		
	//ptr[5] = true;
	ptr[6] = true;
	ptr[7] = true;
	ptr[8] = true;
	ptr[28] = true;
	ptr[100]=true;
	* */
}		
void mesh_timh(list<struct memory> mylist,bool x=true){
	static double sum_meswn_orwn=0;
	static int fores=0;
	int sum=0,cou=0;

	if(x == true){
	fores++;
	list<struct memory>::iterator current;	
	current = mylist.begin();

	while(current != mylist.end()){	
		if(current -> free_space == true)
			sum+=current ->size;
		
		
		
	current++;
	}
	sum_meswn_orwn += ((double)sum)/S;
	}
	else{
	FILE *fp;
	fp = fopen("log.txt","a");		
	fprintf(fp,"*******************************\n");
	fprintf(fp," MESH TIMH = %f\n",sum_meswn_orwn);	
	fprintf(fp,"*******************************\n");	
	fclose(fp);	
	}
}
void print_memory (list<struct memory> mylist){
	FILE *fp;
	fp = fopen("log.txt","a");
	
	//cout<<"=========MEMORY========="<<endl;
	fprintf(fp,"=========MEMORY=========\n");
	list<struct memory>::iterator current;	
	current = mylist.begin();

while(current != mylist.end()){

	fprintf(fp,"[");
	//cout<<"[";
	if(current->free_space == true){
		fprintf(fp,"Free Space");
		//cout<<"Free Space";
	}
	else{
		fprintf(fp,"Used Space");
	//	cout<<"Used Space";
	}
	fprintf(fp,"ID: %d , Start: %d , End: %d , Size: %d]\n",current->id,current->start,current->end,current->size);
	//cout<<",ID: "<<current->id<<",Start: "<<current->start<<",End: "<<current->end<<",Size: "<<current->size<<"]"<<endl;
	
	current->start = 0;
	
	current++;
}
fclose(fp);

}
void print_queue(list<struct queue> mylist){
	FILE *fp;
	fp = fopen("log.txt","a");
	fprintf(fp,"=========QUEUE=========\n");
	//cout<<"=========QUEUE========="<<endl;
	list<struct queue>::iterator current;	
	current = mylist.begin();
	bool empty = true;
	while(current != mylist.end()){
		empty = false;
	fprintf(fp,"ID: %d , Size: %d\n",current->id,current->size);		
	//cout<<"ID: "<<current->id<<",Size: "<<current->size<<endl;
	current++;
	}
	if(empty == true) 
		fprintf(fp,"--[EMPTY]--\n");
	fclose(fp);
}	
void print_queue_stoper(list <struct queue_vp_stoper> mylist){
	cout<<"=========QUEUE STOPER========="<<endl;
	list<struct queue_vp_stoper>::iterator current;	
	current = mylist.begin();

	while(current != mylist.end()){
	cout<<"ID: "<<current->id<<" ,entered: "<<current->entered<<" ,must wait in memory: "<<current->must_stay_in_mem<<" ,in queue = "<<current->in_queue<<endl;
	current++;
	}	
	
	
	
	
}



void print_lifetime(list<struct lifetimes> mylist){
	cout<<"=========LIFETIME========="<<endl;
	list<struct lifetimes>::iterator current;	
	current = mylist.begin();

	while(current != mylist.end()){
	cout<<"ID: "<<current->id<<",lifetime: "<<current->lifetime<<endl;
	current++;
	}

}	
void decrease_lifetime_by_1(list <struct lifetimes>* mylist,int id){
	
	list<struct lifetimes>::iterator current;	
	
	current = mylist->begin();
	
	while(current != mylist->end()){								//edw brisko to simio pou 8a ginei to insert
		if(current->id != id && current->lifetime >=1 )
			current->lifetime--; 


	current++;	
	}	
}

void decrease_time_for_vp_stop(list <struct queue_vp_stoper> *mylist){
	
	list<struct queue_vp_stoper>::iterator current;	
	
	current = mylist->begin();
	
	while(current != mylist->end()){								//edw miwne ton xrono pou 8asteilw vp_stop gia na kalipso to fake p ir8e
		if(current->must_stay_in_mem > 0 && current->in_queue == false)
			current->must_stay_in_mem--; 

	current++;	
	}	
}
bool check_if_is_in_w8ing_queue(int id,list <struct queue_vp_stoper> *mylist,int i){
	
	list<struct queue_vp_stoper>::iterator current;	
	current = mylist->begin();

	while(current != mylist->end()){
		if(current->id == id && current->must_stay_in_mem != -2 ){			//an einai sthn oura anamonhs tote simiose posa sec thelei akoma gia na teliwsei
			current->must_stay_in_mem = i- current->entered;				//etsi PANTA 8a MENOUN STHN MNIMH OI DIERGASIES GIA xrono vp_start-vp_stop opou kai na einai
			return true;													//dioti afero ton must_stay_in_mem kathe fora kata 1 mexri na mini 0
		}


	current++;
	}		
	return false;
	
	
	
}
/******************************************************************************************************************************************************************************************/
/*****************************************			BEST FIT						*******************************************************************************************************/
/******************************************************************************************************************************************************************************************/	
	
void best_fit(int id,int size,int vp_start,	list <struct memory> *mylist,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed){
	
	
	if(vp_start == true){				//insert
		int min=1000000;
		int place,counter=0;
		
		list<struct memory>::iterator current;			
		
		current = mylist->begin();
		while(current != mylist->end()){								//edw brisko to simio pou 8a ginei to insert
			if (current->free_space == true && current->size < min && current->size >= size){
				min = current->size;
				place = counter;
			}
			counter++;
			current++;	
		}
		
		if(min != 1000000){				//an borei na mpei sthn mnimh dld xwraei
		
			current = mylist->begin();
			counter=0;
			while(current != mylist->end()){								//edw paw safto to simio kanw insert ke swsto diaxorismo
				int save_size,save_start;
				if(counter == place){
					struct memory my_memory(false,current->start,current->start+size-1,id);				
				
					mylist -> insert(current,my_memory);
					save_size = current->size - size;
					save_start = current->start + size;
				
					current = mylist -> erase(current);				
					struct memory my_memory_new(true,save_start,save_start+save_size-1,-1);					
				
					mylist->insert(current,my_memory_new);
				
				}
			
				counter++;
				current++;
			}
			mesh_timh(*mylist);
			*Placed = true;				//BIKE STHN MNIMH APO THN QUEUE aliws den me endiaferei an den itan apo queue
		}
		else{		//alliws bes se oura anamonhs
			
			list<struct queue>::iterator check;		//edw elenxw an einai idi sthn queue
			bool test = false;
			
			check = queue->begin();
	
			while(check != queue->end()){
					if(check -> id == id)
						test = true;
						
				check++;
			}	
			if(test == false){						//an ine idi min grapsi pali
					
			struct queue my_queue(id,size);					
			queue -> push_back(my_queue);
			
			struct queue_vp_stoper stoper;
			stoper.id = id;
			stoper.entered = time_sent_start;
			stoper.poped = -1;
			stoper.in_queue = true;
			stoper.must_stay_in_mem = -1;
			
			queue_vp_stoper	-> push_back(stoper);			
			}
		
		}
	}
	else if(vp_start == false){			//an exoume vp_stop
	
	
		if(check_if_is_in_w8ing_queue(id,queue_vp_stoper,time_sent_start) == true) 	//elenkse an einai sthn oura anamonhs h sthn vp_stop_queue
			return;
		else{
			FILE *fp;
			fp = fopen("log.txt","a");
			fprintf(fp,"Now i will VP STOOP   ---- > %d\n",id);
			fclose(fp);	
		list<struct memory>::iterator current;			
		list<struct memory>::iterator temp_next;
		list<struct memory>::iterator temp_prev;			
		current = mylist->begin();
		while(current != mylist->end()){		
			if(current->id == id){
				bool last,first;
				last=false;
				first=false;
				
				temp_next = current;
				temp_prev = current;

				temp_next++;
				if(temp_next == mylist->end() )				//an tha figi to telefteo stixio
					last = true;
				if(current == mylist->begin() )				//an tha figi to proto
					first = true;
					
				if(first == false)							//an den einai to proto katebase
					temp_prev--;
				
				if( last == false && first ==false && temp_next->free_space == true && temp_prev->free_space == true){			//an aristera ke deksia exume keno tote sinenwse
					int start_new_space,end_new_space;
					start_new_space = temp_prev-> start;
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->start = start_new_space;
					current->end = end_new_space;
					current->id = -1;
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);
					mylist->erase(temp_prev);
					break;
				} 
				else if(first == false && last == false && temp_next->free_space == false && temp_prev->free_space == true){	//an mono aristera exume keno
					int start_new_space;
					start_new_space = temp_prev -> start;
					//den thelei end afu tha teliwsnei sto idio
					current->free_space = true;
					current->start = start_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_prev);	
					break;
				}
				else if(first == false && last == false && temp_next->free_space == true && temp_prev->free_space == false){	//an mono deksia exume keno
					int end_new_space;
					//den thelei start afu tha arxizi sto idio
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->end = end_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);	
					break;
				}				
				else if(first == false && last == false && temp_next->free_space == false && temp_prev->free_space == false){	//an deksia ke aristera einai gemath
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}	
				else if(first == true && last == true ){																		//an exume 1 stixio
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}			
				else if(first == true && last == false && temp_next->free_space == true ){	//an mono deksia exume keno kai arxiko stixio
					int end_new_space;
					//den thelei start afu tha arxizi sto idio
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->end = end_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);	
					break;
				}	
				else if(first == false && last == true && temp_prev->free_space == true){	//an mono aristera exume keno kai telefteo stixio
					int start_new_space;
					start_new_space = temp_prev -> start;
					//den thelei end afu tha teliwsnei sto idio
					current->free_space = true;
					current->start = start_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_prev);	
					break;
				}
				else if(first == true && last == false && temp_next->free_space == false ){	//an mono afto
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}
				else if(first == false && last == true && temp_prev->free_space == false ){	//an mono afto
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}								
			}
			current++;
		}
		}
		
	}
	
	
	
}
/******************************************************************************************************************************************************************************************/
/*****************************************			WORST FIT						*******************************************************************************************************/
/******************************************************************************************************************************************************************************************/
void worst_fit(int id,int size,int vp_start, list <struct memory> *mylist,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed){
	
	
	if(vp_start == true){				//insert
		int max=-1;
		int place,counter=0;
		
		list<struct memory>::iterator current;			
		
		current = mylist->begin();
		while(current != mylist->end()){								//edw brisko to simio pou 8a ginei to insert
			if (current->free_space == true && current->size > max && current->size >= size){
				max = current->size;
				place = counter;
			}
			counter++;
			current++;	
		}
		
		if(max != -1){				//an borei na mpei sthn mnimh dld xwraei
		
			current = mylist->begin();
			counter=0;
			while(current != mylist->end()){								//edw paw safto to simio kanw insert ke swsto diaxorismo
				int save_size,save_start;
				if(counter == place){
					struct memory my_memory(false,current->start,current->start+size-1,id);				
				
					mylist -> insert(current,my_memory);
					save_size = current->size - size;
					save_start = current->start + size;
				
					current = mylist -> erase(current);				
					struct memory my_memory_new(true,save_start,save_start+save_size-1,-1);					
				
					mylist->insert(current,my_memory_new);
				
				}
			
				counter++;
				current++;
			}
			mesh_timh(*mylist);
			*Placed = true;				//BIKE STHN MNIMH APO THN QUEUE aliws den me endiaferei an den itan apo queue
		}
		else{		//alliws bes se oura anamonhs
			
			list<struct queue>::iterator check;		//edw elenxw an einai idi sthn queue
			bool test = false;
			
			check = queue->begin();
	
			while(check != queue->end()){
					if(check -> id == id)
						test = true;
						
				check++;
			}	
			if(test == false){						//an ine idi min grapsi pali
					
			struct queue my_queue(id,size);					
			queue -> push_back(my_queue);
			
			struct queue_vp_stoper stoper;
			stoper.id = id;
			stoper.entered = time_sent_start;
			stoper.poped = -1;
			stoper.in_queue = true;
			stoper.must_stay_in_mem = -1;
			
			queue_vp_stoper	-> push_back(stoper);			
			}
		
		}
	}
	else if(vp_start == false){			//an exoume vp_stop
	
	
		if(check_if_is_in_w8ing_queue(id,queue_vp_stoper,time_sent_start) == true) 	//elenkse an einai sthn oura anamonhs h sthn vp_stop_queue
			return;
		else{
			FILE *fp;
			fp = fopen("log.txt","a");
			fprintf(fp,"Now i will VP STOOP   ---- > %d\n",id);
			fclose(fp);	
		list<struct memory>::iterator current;			
		list<struct memory>::iterator temp_next;
		list<struct memory>::iterator temp_prev;			
		current = mylist->begin();
		while(current != mylist->end()){		
			if(current->id == id){
				bool last,first;
				last=false;
				first=false;
				
				temp_next = current;
				temp_prev = current;

				temp_next++;
				if(temp_next == mylist->end() )				//an tha figi to telefteo stixio
					last = true;
				if(current == mylist->begin() )				//an tha figi to proto
					first = true;
					
				if(first == false)							//an den einai to proto katebase
					temp_prev--;
				
				if( last == false && first ==false && temp_next->free_space == true && temp_prev->free_space == true){			//an aristera ke deksia exume keno tote sinenwse
					int start_new_space,end_new_space;
					start_new_space = temp_prev-> start;
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->start = start_new_space;
					current->end = end_new_space;
					current->id = -1;
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);
					mylist->erase(temp_prev);
					break;
				} 
				else if(first == false && last == false && temp_next->free_space == false && temp_prev->free_space == true){	//an mono aristera exume keno
					int start_new_space;
					start_new_space = temp_prev -> start;
					//den thelei end afu tha teliwsnei sto idio
					current->free_space = true;
					current->start = start_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_prev);	
					break;
				}
				else if(first == false && last == false && temp_next->free_space == true && temp_prev->free_space == false){	//an mono deksia exume keno
					int end_new_space;
					//den thelei start afu tha arxizi sto idio
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->end = end_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);	
					break;
				}				
				else if(first == false && last == false && temp_next->free_space == false && temp_prev->free_space == false){	//an deksia ke aristera einai gemath
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}	
				else if(first == true && last == true ){																		//an exume 1 stixio
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}			
				else if(first == true && last == false && temp_next->free_space == true ){	//an mono deksia exume keno kai arxiko stixio
					int end_new_space;
					//den thelei start afu tha arxizi sto idio
					end_new_space = temp_next -> end;
					current->free_space = true;
					current->end = end_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_next);	
					break;
				}	
				else if(first == false && last == true && temp_prev->free_space == true){	//an mono aristera exume keno kai telefteo stixio
					int start_new_space;
					start_new_space = temp_prev -> start;
					//den thelei end afu tha teliwsnei sto idio
					current->free_space = true;
					current->start = start_new_space;
					current->id = -1;				
					current->size = current->end - current->start +1;
					mylist->erase(temp_prev);	
					break;
				}
				else if(first == true && last == false && temp_next->free_space == false ){	//an mono afto
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}
				else if(first == false && last == true && temp_prev->free_space == false ){	//an mono afto
					//den thelei end afu tha teliwsnei sto idio
					//den thelei start afu tha arxizi sto idio
					current->free_space = true;
					current->id = -1;				
					//to size einai idio
					break;
				}								
			}
			current++;
		}
		}
		
	}
	
	
	
}
/******************************************************************************************************************************************************************************************/
/*****************************************			BEST FIT						*******************************************************************************************************/
/******************************************************************************************************************************************************************************************/	

void buddy(int id,int size,int vp_start,list <struct memory> *mylist,list <struct queue> *queue,list <struct queue_vp_stoper> *queue_vp_stoper,int time_sent_start,bool *Placed){
	
	
	if(vp_start == true){				//insert
		bool yeah = false;
		int place,counter=0;
		
		list<struct memory>::iterator current;			
		
		current = mylist->begin();
		while(current != mylist->end()){								//edw brisko to simio pou 8a ginei to insert
			if (current->free_space == true && current->size >= size){
				yeah = true;
				place = counter;
				break;
			}
			counter++;
			current++;	
		}
		
		if( yeah == true ){				//an borei na mpei sthn mnimh dld xwraei
			
			current = mylist->begin();
			counter=0;
			while(current != mylist->end()){								//edw paw safto to simio kanw insert ke swsto diaxorismo
				int save_size,save_start,save_end;
				if(counter == place){
					while( size <= current -> size){
					//	cout<<"size"<<size<<" <= "<<current->size<<endl;
					
						if( size <= current -> size/2){
							save_size = current->size / 2;
							save_start = current->start;
							save_end = save_start + current->size / 2 -1 ;					
					
							struct memory my_memory_left(true,save_start,save_end,-1);				//----[  256  ]---- ginete	---[  128  ]---[  128  ]--
							struct memory my_memory_right(true,save_end+1,save_end + save_size  ,-1);		
							my_memory_left.left = true;
							my_memory_right.left = false;
							
							
							current = mylist -> erase(current);										//diagrafh genikhs theshs px 256
							mylist -> insert(current,my_memory_left);
							mylist -> insert(current,my_memory_right);
					
							current--;																//na dixnei pali sto 1o free kotuaki apo thn torinh ams mnimh
							current--;
						}	
						else{
							current -> free_space = false;
							current -> id = id;			
							break;				
						}
					}
				}
			
				counter++;
				current++;
			}
			mesh_timh(*mylist);
			*Placed = true;				//BIKE STHN MNIMH APO THN QUEUE aliws den me endiaferei an den itan apo queue
		}
		else{		//alliws bes se oura anamonhs
			
			list<struct queue>::iterator check;		//edw elenxw an einai idi sthn queue
			bool test = false;
			
			check = queue->begin();
	
			while(check != queue->end()){
					if(check -> id == id)
						test = true;
						
				check++;
			}	
			if(test == false){						//an ine idi min grapsi pali
					
			struct queue my_queue(id,size);					
			queue -> push_back(my_queue);
			
			struct queue_vp_stoper stoper;
			stoper.id = id;
			stoper.entered = time_sent_start;
			stoper.poped = -1;
			stoper.in_queue = true;
			stoper.must_stay_in_mem = -1;
			
			queue_vp_stoper	-> push_back(stoper);			
			}
		
		}
	}
	else if(vp_start == false){			//an exoume vp_stop

	
		if(check_if_is_in_w8ing_queue(id,queue_vp_stoper,time_sent_start) == true) 	//elenkse an einai sthn oura anamonhs h sthn vp_stop_queue
			return;
		else{
			FILE *fp;
			fp = fopen("log.txt","a");
			fprintf(fp,"Now i will VP STOOP   ---- > %d\n",id);
			fclose(fp);
		list<struct memory>::iterator current;			
		list<struct memory>::iterator temp_next;
		list<struct memory>::iterator temp_prev;			
		current = mylist->begin();
		while(current != mylist->end()){	

			if(current->id == id){
				int ca = 0;
				while(1){
					bool ent1 = false,ent2 = false;
				if(ca == 0){											//thn prwth fora p tha bei
					temp_next = current;
					temp_prev = current;

					temp_next++;
				
				if(temp_next != mylist->end()){
					if (temp_next -> size == current -> size && current -> left == true ){		//to deksia tu exei idio megethos  kai an einai to aristero komati
						if(temp_next->free_space == true){				//an ine keno to dipla enosta
						
							current -> end = temp_next -> end;
							current -> id =-1;
							current -> size += temp_next->size;
							current -> free_space = true;
														
							temp_next = mylist -> erase(temp_next);
						
							ent1 = true;
																		
						}
						else{											//aliws apla kanto free
							current -> free_space = true;
							current -> id = -1;
							return;
						}		
					}
				}
				
				if(current != mylist->begin()){
					temp_prev--;
					if(temp_prev -> size == current -> size && current -> left == false){
						if(temp_prev->free_space == true){				//an ine keno to dipla enosta

							temp_prev -> end = current -> end;
							temp_prev -> size += current->size;
														
							current = mylist -> erase(current);						
							
							ent2 = true;
						}
						else{											//aliws apla kanto free
							current -> free_space =true;
							current -> id = -1;
							return;
						}						
					}
					
				}
				if(ent1 == false && ent2 == false){
					current -> free_space = true;
					current -> id = -1;			
					
				}
				
				
			}
			else{								//tora an briskis 2 free mazi bgalta me idio size me thn sira
				current = mylist->begin();
				
				while(current != mylist->end()){
					temp_next = current;
					temp_next++;
					
					if( temp_next == mylist->end())
						return;
					if(current->size == temp_next->size && current->free_space == true && temp_next->free_space == true){
						
							current -> end = temp_next -> end;
							current -> size += temp_next->size;
														
							temp_next = mylist -> erase(temp_next);
							break;
					
					}
					
					current++;
					
				}						
			}
			ca++;
			}
		}
	current++;
		}	
			
							
							
	}
		
	}
	
	
}











