include variables.mk

.PHONY: compile_flags.txt
compile_flags.txt:
	echo $(CFLAGS) | tr ' ' '\n' > compile_flags.txt

$(BUILD_DIR)/$(TEST_DIR)/%: $(TEST_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(TEST_CFLAGS) $(TEST_LDFLAGS) $^ -o $@

.PHONY: tests
tests: $(BUILD_DIR)/$(TEST_DIR)/alloc_test $(BUILD_DIR)/$(TEST_DIR)/calloc_test
	$<

clean:
	rm -rf $(BUILD_DIR)

