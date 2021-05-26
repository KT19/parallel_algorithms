CC = g++
CFLAGS = -std=c++11 -pthread

ALL: Merge P_Merge

Merge: merge_sort.cpp
	$(CC) $(CFLAGS) -o merge merge_sort.cpp

P_Merge: parallel_merge_sort.cpp
	$(CC) $(CFLAGS) -o p_merge parallel_merge_sort.cpp
