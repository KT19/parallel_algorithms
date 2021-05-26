CC = g++
CFLAGS = -std=c++11 -pthread

ALL: Bubble P_Bubble

Bubble: bubble_sort.cpp
	$(CC) $(CFLAGS) -o bubble bubble_sort.cpp

P_Bubble: parallel_bubble_sort.cpp
	$(CC) $(CFLAGS) -o p_bubble parallel_bubble_sort.cpp
