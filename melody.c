#include "melody.h"

const uint16_t imperialMarch[] = {
    0, 100,
    0, 100,
    A4, 500,
    A4, 500,
    A4, 500,
    F4, 350,
    C5, 150,
    A4, 500,
    F4, 350,
    C5, 150,
    A4, 650,

    0, 150,
    //end of first bit

    E5, 500,
    E5, 500,
    E5, 500,
    F5, 350,
    C5, 150,
    Gs4, 500,
    F4, 350,
    C5, 150,
    A4, 650,

    0,150,
    //end of second bit...

    A5, 500,
    A4, 300,
    A4, 150,
    A5, 400,
    Gs5, 200,
    G5, 200,
    Fs5, 125,
    F5, 125,
    Fs5, 250,

    0,250,

    As4, 250,
    Ds5, 400,
    D5, 200,
    Cs5, 200,
    C5, 125,
    B4, 125,
    C5, 250,

    0,250,

    F4, 125,
    Gs4, 500,
    F4, 375,
    A4, 125,
    C5, 500,
    A4, 375,
    C5, 125,
    E5, 650,

    //end of third bit... Though it doesn't play well)
    //let's repeat it

    A5, 500,
    A4, 300,
    A4, 150,
    A5, 400,
    Gs5, 200,
    G5, 200,
    Fs5, 125,
    F5, 125,
    Fs5, 250,

    0, 250,

    As4, 250,
    Ds5, 400,
    D5, 200,
    Cs5, 200,
    C5, 125,
    B4, 125,
    C5, 250,

    0, 250,

    F4, 250,
    Gs4, 500,
    F4, 375,
    C5, 125,
    A4, 500,
    F4, 375,
    C5, 125,
    A4, 650,

    0, 1000
};

const uint16_t imperialMarchSize = sizeof(imperialMarch);

