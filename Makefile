BUILD_DIR = build

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make

clean:
	@rm -rf $(BUILD_DIR)

run: all
	@cd $(BUILD_DIR) && ./bin/viewer