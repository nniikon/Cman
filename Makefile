include config.mk
 
all:
	@$(MAKE) -C ./libs/logs
	@$(MAKE) -C ./libs/memAllocations
	@$(MAKE) -C ./libs/fileToBuffer
	@$(MAKE) -C ./libs/parseArgs
	@$(MAKE) -C ./common/binaryTree
	@$(MAKE) -C ./frontEnd/source

fe: all
	@$(GXX) $(CFLAGS) ./frontEnd/main.cpp -c -o $(BUILD_DIR)/main.o
	@$(GXX) $(CFLAGS) -o $(BUILD_DIR)/$(EXEC_NAME) $(BUILD_DIR)/*.o

be: all
	@$(GXX) $(CFLAGS) ./backEnd/main.cpp -c -o $(BUILD_DIR)/main.o
	@$(GXX) $(CFLAGS) -o $(BUILD_DIR)/$(EXEC_NAME) $(BUILD_DIR)/*.o

clean:
	@rm -rf $(BUILD_DIR)
 
run: 
	$(BUILD_DIR)/$(EXEC_NAME)
