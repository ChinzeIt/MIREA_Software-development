GPP = g++
CPPFLAGS = -Wall -Werror -Wextra -std=c++20
LIBLINK = -ltinyxml2 -lminizip -lncurses

CPPFILES = $(filter-out main.cpp, $(wildcard *.cpp))
OBJFILES = $(CPPFILES:.cpp=.o)

all: clean libraly build

libraly: $(OBJFILES)
	@ar rcs ../libfiles.a $(OBJFILES)
	@echo "|-- BUILD LIBRARY SUCCESS --|"
	@rm -rf $(OBJFILES)

%.o: %.cpp
	@$(GPP) $(CPPFLAGS) -c $< -o $@

build:
	@$(GPP) $(CPPFLAGS) main.cpp -L.. -lfiles -o files $(LIBLINK)
	@echo "|-- BUILD --|"

clean:
	@rm -rf files ../libfiles.a *.o *.txt *.json *.xml *.zip
	@echo "|-- CLEAN --|"

cpp:
	@cppcheck --enable=all $(CPPFILES) main.cpp --suppress=unusedFunction --suppress=missingIncludeSystem $(HFILES)

clang:
	@clang-format -n $(CPPFILES) main.cpp $(HFILES)
	@clang-format -i $(CPPFILES) main.cpp $(HFILES)
	@echo "|-- MAKE CLANG --|"
