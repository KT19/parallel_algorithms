CC = g++
CFLAGS = -std=c++11
OMPFLAGS = -Xpreprocessor -fopenmp -lomp

ALL: SCAN P_SCAN

SCAN: scan.cpp
	$(CC) $(CFLAGS) -o scan scan.cpp

P_SCAN: parallel_scan.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) -o p_scan parallel_scan.cpp
