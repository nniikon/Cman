include config.mk
 
all:
	@$(MAKE) -C ./libs/logs
	@$(MAKE) -C ./libs/memAllocations
	@$(MAKE) -C ./libs/fileToBuffer
	@$(MAKE) -C ./libs/parseArgs

fe: all
	@$(MAKE) -C ./common/binaryTree
	@$(MAKE) -C ./frontEnd/source
	@$(GXX) $(CFLAGS) ./frontEnd/main.cpp -c -o $(BUILD_DIR)/main.o
	@$(GXX) $(CFLAGS) -o $(BUILD_DIR)/fe $(BUILD_DIR)/*.o

be: all
	@$(MAKE) -C ./backEnd/source
	@$(GXX) $(CFLAGS) ./backEnd/main.cpp -c -o $(BUILD_DIR)/main.o
	@$(GXX) $(CFLAGS) -o $(BUILD_DIR)/be $(BUILD_DIR)/*.o

yc: all
	@$(MAKE) -C ./stoYACC
	@$(GXX) $(CFLAGS) ./stoYACC/main.cpp -c -o $(BUILD_DIR)/main.o
	@$(GXX) $(CFLAGS) -o $(BUILD_DIR)/yc $(BUILD_DIR)/*.o

clean:
	@rm -rf $(BUILD_DIR)
 
ycrun: 
	$(BUILD_DIR)/yc
	#$(BUILD_DIR)/be
	#../CPU/Assembler/build/Ass -i ass.bnn -o ass.out
	#../CPU/SPU/build/SPU -i ass.out -da ass.help

ferun: 
	$(BUILD_DIR)/fe
	#$(BUILD_DIR)/be
	#../CPU/Assembler/build/Ass -i ass.bnn -o ass.out
	#../CPU/SPU/build/SPU -i ass.out -da ass.help

test: clean yc ycrun fe ferun
