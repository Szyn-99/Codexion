#!/usr/bin/env python3

from sys import argv


if __name__ == "__main__":
    """Explanation: if there's even number of coders, there will be two batches of coders one compiling one waiting
    , if there's odd number of coders, there will be three batches of coders one compiling one waiting one waiting for the waiting one to finish"""
    if len(argv) != 5:
        print("Usage: ./fair_tests.py <number_of_coders> <time_to_burnout> <time_to_compile> <dongle_cooldown>")
        exit(1)
    n_coders = int(argv[1])
    time_to_burnout = int(argv[2])
    time_to_compile = int(argv[3])
    dongle_cooldown = int(argv[4])
    if n_coders % 2 == 0:
        if time_to_burnout <= 2 * (time_to_compile + dongle_cooldown):
            print("Mathematically Unfair, Certain burnout coders will never get to compile")
        else:
            print("Mathematically Fair")
    elif n_coders % 2 != 0:
        if time_to_burnout <= 3 * (time_to_compile + dongle_cooldown):
            print("Mathematically Unfair, Certain burnout coders will never get to compile")
        else:
            print("Mathematically Fair")
    
        