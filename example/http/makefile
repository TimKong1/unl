TARGET = main
OBJ_PATH = objs
PREFIX_BIN =
PROJECT_PATH = ../../

CC = gcc -std=c11 
CPP = g++ -std=c++11 
INCLUDES = -I $(PROJECT_PATH)include -I $(PROJECT_PATH)include/core
LIBS = 
CFLAGS =-Wall -Werror -O2 -g
LINKFLAGS = -levent -pthread

SRCDIR = . $(PROJECT_PATH)src/http $(PROJECT_PATH)src/core $(PROJECT_PATH)src/thread



C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

#CPP_SOURCES = $(wildcard *.cpp)
CPP_SRCDIR = $(SRCDIR)
CPP_SOURCES = $(foreach d,$(CPP_SRCDIR),$(wildcard $(d)/*.cpp) )
CPP_OBJS = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))

default:init compile


$(C_OBJS):$(OBJ_PATH)/%.o:%.c $(PROTO_OBJS)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CPP_OBJS):$(OBJ_PATH)/%.o:%.cpp $(PROTO_OBJS)
	$(CPP) -c $(CFLAGS) $(INCLUDES) $< -o $@

init:
	$(foreach d,$(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)


compile:$(C_OBJS) $(CPP_OBJS) 
	$(CPP)  $^ -o $(TARGET)  $(LINKFLAGS) $(LIBS)

clean:
	rm -rf $(OBJ_PATH)
	rm -rf ../src
	rm -rf $(TARGET)

rebuild: clean init compile
