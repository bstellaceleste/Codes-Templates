#!/bin/bash
for pid in $(ps ax | awk '{print $1}')
do
	taskset -pc 0 $pid
done 
taskset -pc 1 $1
