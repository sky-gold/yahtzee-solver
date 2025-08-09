# Variables
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# Targets
.PHONY: build test run clean debug_build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make

debug_build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

test: build
	@$(BUILD_DIR)/runTests

run: build
	@$(BUILD_DIR)/yahtzee_solver

clean:
	@rm -rf $(BUILD_DIR)