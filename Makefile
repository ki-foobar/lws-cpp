MKDIR = mkdir
CMAKE = cmake

BIN_DIR = bin
PROGRAM = lws


all: test


test: build
	@./$(BIN_DIR)/$(PROGRAM)


build: $(BIN_DIR)
	@cd $(BIN_DIR); $(CMAKE) ..; $(MAKE)


$(BIN_DIR):
	@$(MKDIR) $(BIN_DIR)


clean:
	@$(RM) -rf $(BIN_DIR)


rebuild: clean build



.PHONY: all test build clean rebuild
