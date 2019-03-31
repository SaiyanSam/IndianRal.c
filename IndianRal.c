#include<stdio.h>
#include<pthread.h>
#include<conio.h>
#include<windows.h>

int waiting=0;  //global variable representing passengers waiting at station
struct lock lock1;  //global lock structure object

struct station
{
    int person_no;  //here pass_no represent number of people waiting on station for train
};
struct station station_init()
{
    struct station a_station;
    return a_station;
}
int station_load_train(struct station *station, int count)
{
    int remaining;
    if(station->person_no<=count)
    {
        remaining=0;
        printf("\n  Seats available for all waiting passengers. All %d passengers can board.\n\n",station->person_no);
    }
    else
    {
        remaining=station->person_no-count;
        printf("\n  Seats not available for all passengers.\n\n  %d passengers can board.\n  %d passengers need to wait for next train. We are sorry for the inconvenience.\n\n",count,remaining);
    }
    return remaining;
}
void station_wait_for_train(struct station *station)
{
    lock_acquire(&lock1);
    int flag=lock_acquire(&lock1);
    station->person_no++;
    lock_release(&lock1);
}
void station_on_board(struct station *station)
{

}

struct lock
{
    int i;  // variable to be used as a flag
};

struct lock lock_init()
{
    //initialize lock structure
}
int lock_acquire(struct lock *lock)
{
    if(lock->i!=1)
    {
        lock->i=1;  //lock acquired
        return 1;   //means has been lock acquired
    }
}
void lock_release(struct lock *lock)
{
    lock->i=0; //lock released so other threads can use critical section
}

int main()
{   int a,seats,train_no=0;
    char choice;
    struct station station=station_init();
    station.person_no=0;

    X:
    printf("\n\n  Enter the number of people arrived at station : ");
    scanf("%d",&a);  // a represent then newly arrived passengers
    waiting=waiting+a;

    for(int i=0;i<a;i++)
    {
        pthread_t thread[i];
        pthread_create(&thread[i] , NULL , station_wait_for_train , &station);
        pthread_join(thread[i] , NULL);
    }
    printf("  Data updated.");
    printf("  Number of passengers waiting on station : %d\n",station.person_no);
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

    waiting=station_load_train(&station , seats);
    station.person_no=waiting;

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
