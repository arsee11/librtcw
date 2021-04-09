CC=g++
CXX=g++
CXXFLAGS=-g -DWEBRTC_EXTERNAL_JSON
INCLUDES=-I /home/cl/tests/ -I /home/cl/libs/librtcw  
LINKS=-L/home/cl/libs/librtcw/lib -lrtcsignaling \
      -L/home/cl/libs/librtcw/third_party/jsoncpp -ljsoncpp 


TARGET=sigsvr

$(TARGET): signaling_svr.o 
	$(CXX) -o $(TARGET) $(CXXFLAGS) signaling_svr.o  $(LINKS)


signaling_svr.o: signaling_svr.cpp
	$(CC) -c $(CXXFLAGS) $(INCLUDES) signaling_svr.cpp

clean:
	rm -f *.o $(TARGET)
    
