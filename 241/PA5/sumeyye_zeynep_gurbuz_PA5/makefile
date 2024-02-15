all: clean main.out run

SRC_DIR := .
OBJ_DIR := .
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS += -std=c++11
CXXFLAGS += -MMD
-include $(OBJ_FILES:.o=.d)

main.out: $(OBJ_FILES)
	@g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -r -f *.d *.o *.out

run:
	@./main.out