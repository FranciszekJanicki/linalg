PROJECT_DIR := $(shell pwd)
THIRD_PARTY_DIR := ${PROJECT_DIR}/third_party
BUILD_DIR := ${PROJECT_DIR}/build
LIB_DIR := ${PROJECT_DIR}/lib
TEST_DIR := ${PROJECT_DIR }/test

.PHONY: build
build: 
	cd ${BUILD_DIR} && make

.PHONY: run-kalman
run-kalman:
	cd ${BUILD_DIR} && ./Kalman

.PHONY: run-polynomial
run-polynomial:
	cd ${BUILD_DIR} && ./Polynomial

.PHONY: run-pid
run-pid:
	cd ${BUILD_DIR} && ./Pid

.PHONY: run-dft
run-dft:
	cd ${BUILD_DIR} && ./Dft

.PHONY: run-filters
run-filters:
	cd ${BUILD_DIR} && ./Filters

.PHONY: clean
clean:
	cd ${PROJECT_DIR} && rm -rf ${BUILD_DIR}

.PHONY: cmake
cmake:
	cd ${PROJECT_DIR} && make clean && mkdir ${BUILD_DIR} && cmake -S . -B ${BUILD_DIR}

.PHONY: setup-external
setup-external:
	cd $(PROJECT_DIR) && touch .gitmodules && $(MAKE) setup-eigen

.PHONY: clean-external
clean-external: clean-eigen
	git submodule deinit --all
	rm -rf .gitmodules

.PHONY: clang-format
clang-format:
	for ext in h c cpp hpp; do \
		find $(SOURCE_DIR) -iname "*.$$ext" -print0 | xargs -0 -r clang-format -i; \
	done

.PHONY: clang-tidy
clang-tidy:
	for ext in h c cpp hpp; do \
		find $(SOURCE_DIR) -iname "*.$$ext" -print0 | xargs -0 clang-tidy --fix --fix-errors --config-file=.clang-tidy; \
	done

.PHONY: all
all:
	make clang-format && make build && make run-kalman

	