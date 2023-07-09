#!/bin/bash

# Simulate the first client
for i in {1..5}
do
    message=$(printf "%0.sa" {1..1000})
    echo $message | nc 0.0.0.0 8080 &
done

# Simulate the second client
for i in {1..5}
do
    message=$(printf "%0.sb" {1..1000})
    echo $message | nc 0.0.0.0 8080 &
done
