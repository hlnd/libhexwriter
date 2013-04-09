CPPUTEST_HOME = /home/omh/Projects/CppUTest-3.3/

SDK_PATH = ../../nrf51/nrf51822/

INCLUDE_PATHS += ../inc/
INCLUDE_PATHS += ../src/
INCLUDE_PATHS += $(SDK_PATH)Include/
INCLUDE_PATHS += $(SDK_PATH)Include/gcc/
INCLUDE_PATHS += $(SDK_PATH)Include/app_common/
INCLUDE_PATHS += $(SDK_PATH)Include/ble/softdevice/
INCLUDE_PATHS += $(CPPUTEST_HOME)include/

CPPFLAGS = -g $(addprefix -I, $(INCLUDE_PATHS))

CXXFLAGS = -include $(CPPUTEST_HOME)include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS = -std=c99 -include $(CPPUTEST_HOME)include/CppUTest/MemoryLeakDetectorMallocMacros.h

TARGET = all_tests

SOURCES_LIB = $(wildcard ../src/*.c)
OBJECTS_LIB = $(SOURCES_LIB:.c=.o)

SOURCES_TESTS = $(wildcard ../tests/*.cpp)
OBJECTS_TESTS = $(SOURCES_TESTS:.cpp=.o)

LDFLAGS = -L$(CPPUTEST_HOME)lib/ -lCppUTest -lstdc++

all: check

rebuild: clean all

check: $(TARGET)
	./$(TARGET)

clean:
	rm -rf *.o

$(TARGET): $(OBJECTS_LIB) $(OBJECTS_TESTS)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: check clean rebuild
