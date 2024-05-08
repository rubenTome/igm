CC=g++
CXXFLAGS=-Wall -I/usr/include
LDFLAGS=-losg -losgViewer -losgDB -losgGA -losgUtil

SOURCES=OsgSpinningCube.cpp
EXECUTABLE=OsgSpinningCube

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CXXFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
