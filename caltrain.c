#include <pthread.h>
#include "caltrain.h"
#include "stdio.h"

void message(char* message) {
//    printf(message);
}

void debug (struct station *station) {
//    printf("S: %d, W : %d, F : %d\n", station -> inStation, station -> walkingToTrain, station -> freeSeats);
}

/**
 * called at start pf the program for initialization.
 * @param station object for the train station.
 */
void station_init (struct station *station) {
    station -> inStation = 0;
    station -> walkingToTrain = 0;
    station -> freeSeats = 0;
    pthread_mutex_init(&station -> mutex, NULL);
    pthread_cond_init(&station -> availableSeats, NULL);
    pthread_cond_init(&station -> passengerReachedTheTrain, NULL);

    message("station_init: ");
    debug(station);
}

/**
 * called when a train arrives in the station and has opened its doors.
 * @param station object for the train station.
 * @param count indicates how many seats are available on the train.
 */
void station_load_train (struct station *station, int count) {
    message("station_load_train start: ");
    debug(station);

    if (station -> inStation == 0) {
        // no people in station so train will leave immediately
        return;
    }

    station -> freeSeats = count;
    pthread_cond_broadcast(&station -> availableSeats); // notify passengers that train arrived

    while (station -> walkingToTrain > 0 || (station -> freeSeats > 0 && station -> inStation > 0)) {
        // waiting until station is empty or all seats are full.
        pthread_cond_wait(&station -> passengerReachedTheTrain, &station -> mutex);
    }

    // In Case people in station < free seats
    // Reset the counter for the next trains.
    station -> freeSeats = 0;

    message("station_load_train end: ");
    debug(station);
}

/**
 * called when a passenger robot arrives in a station.
 * @param station object for the train station.
 */
void station_wait_for_train (struct station *station) {
    pthread_mutex_lock(&station -> mutex);

    message("station_wait_for_train start: ");
    debug(station);

    // new person arrived to station.
    station -> inStation++;
    while (station -> freeSeats == station -> walkingToTrain) {
        // waits until there is a free seat for him in the train.
        pthread_cond_wait(&station -> availableSeats, &station -> mutex);
    }

    // new Seats available then move towards the train
    station -> inStation--;
    station -> walkingToTrain++;

    message("station_wait_for_train end: ");
    debug(station);

    pthread_mutex_unlock(&station -> mutex);
}

/**
 * called once the passenger is seated in the train
 * @param station object for the train station.
 */
void station_on_board (struct station *station) {
    pthread_mutex_lock(&station -> mutex);

    message("station_on_board start: ");
    debug(station);

    // reached the train successfully.
    station -> walkingToTrain--;
    station -> freeSeats--;

    message("station_on_board end: ");
    debug(station);

    pthread_mutex_unlock(&station -> mutex);

    if (station -> walkingToTrain == 0 && (station -> freeSeats == 0 || station -> inStation == 0)) {
        // tells the train that he reached the train successfully
        pthread_cond_signal(&station -> passengerReachedTheTrain);
    }
}
