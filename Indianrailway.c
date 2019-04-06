#include<stdio.h>
#include<pthread.h>
#include<conio.h>
#include<windows.h>

int waiting=0;  //global variable representing passengers waiting at station
int seats;
pthread_mutex_t lock1;
pthread_cond_t aCond;


struct station
{
    int person_no;  //here pass_no represent number of people waiting on station for train
    int person_boarded;
    struct lock *lock1;
    struct condition *cond;
};
struct station station_init()
{
    struct station a_station;
    return a_station;
}
void station_load_train(struct station *station, int count)
{
    int remaining;
    remaining=0;
    if(station->person_no<=count)
    {
        if(station->person_no==0)
        {
            printf("\n  No passenger present for boarding. \n\n");
            station->person_boarded=0;
        }
        else
        {
            printf("\n  Seats available for all waiting passengers. All %d passengers can board.\n\n",station->person_no);
            station->person_boarded=station->person_no;
        }
    }
    else
    {
        remaining=station->person_no-count;
        printf("\n  Seats not available for all passengers.\n\n  %d passengers can board.\n  %d passengers need to wait for next train. We are sorry for the inconvenience.\n\n",count,remaining);
        station->person_boarded=count;
    }
    //Allocating passengers to their seats
    printf("  Train boarding in progress");
    for(int a=0;a<5;a++)
    {
        Sleep(500);
        printf(".");
    }
    printf("\n\n");
    station->person_no=remaining;
    waiting=station->person_no;
}
void station_wait_for_train(struct station *station)
{
    lock_acquire(&lock1);
    station->person_no++;
    if(station->person_no>500)
    {
        pthread_cond_wait(&aCond,&lock1);
    }
    else
    {
        pthread_cond_signal(&aCond);
    }
    lock_release(&lock1);
}
void station_on_board(struct station *station)
{
    printf("  Boarding successful. Boarding Summary : \n\n");
    printf("  _______________________________________________________________________________\n");
    printf("                       Total seats available : %d\n",seats);
    printf("                    Number of passengers boarded : %d\n",station->person_boarded);
    printf("                  Number of passengers left on station : %d\n",station->person_no);
    printf("  _______________________________________________________________________________\n\n");
}

void lock_init(struct lock *lock)
{
    pthread_mutex_init(&lock1 , NULL);
}
void lock_acquire(struct lock *lock1)
{
    pthread_mutex_lock(&lock1);
}
void lock_release(struct lock *lock1)
{
    pthread_mutex_unlock(&lock1);
}

void cond_wait(struct condition *aCond , struct lock *lock1)
{
    pthread_cond_wait(&aCond,&lock1);
}
void cond_signal(struct condition *aCond)
{
    pthread_cond_signal(&aCond);
}
void cond_broadcast(struct condition *aCond)
{
    pthread_cond_broadcast(&aCond);
}

int main()
{   int a,train_no=0;
    char choice;
    struct station station=station_init();
    station.person_no=0;

    X:
    printf("\n\n  Number of people already waiting on station : %d\n",waiting);
    printf("  Enter the number of people arrived at station : ");
    scanf("%d",&a);  // a represent then newly arrived passengers
    waiting=waiting+a;

    for(int i=0;i<a;i++)
    {
        pthread_t thread[i];
        pthread_create(&thread[i] , NULL , station_wait_for_train , &station);
        pthread_join(thread[i] , NULL);
    }
    printf("  Data updated.");
    printf("  Updated number of passengers waiting on station : %d\n",station.person_no);
    Sleep(1000);

    printf("\n  Train has arrived.\n");
    printf("  Checking seats and information");
    for(a=0;a<5;a++)
    {
        Sleep(500);
        printf(".");
    }
    printf("\n\n");
    if(train_no%3==0)
    {
            seats=120;
            printf("  Jammutavi Express has arrived. Available number of seats : %d",seats);
    }
    else if(train_no%3==1)
    {
            seats=100;
            printf("  Chennai Express has arrived. Available number of seats : %d",seats);
    }
    else
    {
            seats=150;
            printf("  Rajdhani Express has arrived. Available number of seats : %d",seats);
    }
    train_no++;

    station_load_train(&station , seats);
    station_on_board(&station);  //Function called as seats have been allocated

    Y:
    printf("  Do you want to continue the simulation? (Y/N)...  ");
    scanf("%s",&choice);
    switch(choice)
    {
        case 'Y': goto X;
                  break;
        case 'N': exit(0);
                  break;
        default : printf("  Invalid choice. Please try again.\n\n");
                  goto Y;
                  break;
    }
}
