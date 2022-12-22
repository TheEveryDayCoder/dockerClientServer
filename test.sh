#!/bin/bash


./serverA &
./serverB &
./serverC &
./serverM &
 
#./clientA 
sleep 10
pkill -P $$
