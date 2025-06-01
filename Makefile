BUILD_DIR = build
TEST_BUILD_DIR = build_test

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(TEST_BUILD_DIR)

run: all
	@cd $(BUILD_DIR) && ./bin/viewer

.PHONY: test

test:
	@mkdir -p $(TEST_BUILD_DIR)
	@cd $(TEST_BUILD_DIR) && cmake ../test && make
	@cd $(TEST_BUILD_DIR) && ./3DViewerTests