CXX := clang++
CXXFLAGS := -std=c++98 # -Wall -Wextra -Werror 
INC := -Iinc/

TARGET := ft_irc

SRC_DIR := src/
OBJ_DIR := obj/

SRC := $(wildcard $(SRC_DIR)*.cpp)
OBJ := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC))

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m


all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CXX) $(CXXFLAGS)  $^ -o $@
	@echo "$(GREEN)Executable created.$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp inc/*.hpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files removed.$(RESET)"

fclean: clean
	@rm -f $(TARGET)
	@echo "$(RED)Executable removed.$(RESET)"

re: fclean all
