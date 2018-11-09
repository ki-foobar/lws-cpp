MKDIR = mkdir
CMAKE = cmake

BIN_DIR = bin
PROGRAM = lws


all: melee ranged


melee: build
	@$(RM) -f melee.csv
	@./$(BIN_DIR)/$(PROGRAM) > melee.csv


ranged: build
	@$(RM) -f ranged.csv
	@./$(BIN_DIR)/$(PROGRAM) --ranged > ranged.csv


build: $(BIN_DIR)
	@cd $(BIN_DIR); $(CMAKE) ..; $(MAKE)


$(BIN_DIR):
	@$(MKDIR) $(BIN_DIR)


clean:
	@$(RM) -rf $(BIN_DIR)


rebuild: clean build



.PHONY: all melee ranged test build clean rebuild
