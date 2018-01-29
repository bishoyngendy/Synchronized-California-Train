#include <pthread.h>

struct station {

    // counters
    int inStation; // people arriving in station before going to the train.
    int freeSeats; // free seats in the train.
    int walkingToTrain; // people on their way to the train (intermediate state)

    // locking mutex
    pthread_mutex_t mutex;

    // conditions
    pthread_cond_t availableSeats; // to let people in station know when train arrives with empty seats.
    pthread_cond_t passengerReachedTheTrain; // to let the train know when a passenger reaches the train.
};

void station_init (struct station *station);

void station_load_train (struct station *station, int count);

void station_wait_for_train (struct station *station);

void station_on_board (struct station *station);