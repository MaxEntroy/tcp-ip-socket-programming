INC_DIR:=./inc
SRC_DIR:=./src
EXE_DIR:=./bin

TARGET:=$(EXE_DIR)/main
OBJS:= $(wildcard $(SRC_DIR)/*.cc)

CC:=g++
CFLAGS:=-g -Wall -lgflags -lpthread -std=c++11

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -rf $(TARGET)
