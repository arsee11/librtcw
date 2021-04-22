CC=/usr/bin/g++
CXX=/usr/bin/g++
CXXFLAGS=-g -O0 -Wall -fPIC #-fsanitize=address
INCLUDES=-I /home/arsee/gits/ -I ../../
LINKS=-L../../lib -lrtcsignaling \
      -lboost_json 


TARGET=sigsvr

$(TARGET): signaling_svr.o 
	$(CXX) -o $(TARGET) $(CXXFLAGS) signaling_svr.o  $(LINKS)


signaling_svr.o: signaling_svr.cpp ../../signaling/signaling_msg.h
	$(CC) -c $(CXXFLAGS) $(INCLUDES) signaling_svr.cpp

clean:
	rm -f *.o $(TARGET)
    
