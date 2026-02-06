GPP = g++
CPPFLAGS = -Wall -Werror -Wextra -std=c++20
LIBLINK = -pthread -lncurses -lcrypto

CPPFILES = *.cpp
HFILES = *.h

all:
	clean build

build:
	@$(GPP) $(CPPFLAGS) $(CPPFILES) -L.. -lfiles -o hash $(LIBLINK)
	@echo "|-- BUILD --|"

rebuild: hash
	@$(GPP) $(CPPFLAGS) $(CPPFILES) -L.. -lfiles -o hash $(LIBLINK)
	@echo "|-- REBUILD --|"

clean:
	@rm -rf hash *.txt *.json *.xml *.zip
	@echo "|-- CLEAN --|"

cpp:
	@cppcheck --enable=all $(CPPFILES) --suppress=unusedFunction --suppress=missingIncludeSystem $(HFILES)

clang:
	@clang-format -n $(CPPFILES) $(HFILES)
	@clang-format -i $(CPPFILES) $(HFILES)
	@echo "|-- MAKE CLANG --|"