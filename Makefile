PROJECT_DIR := $(shell pwd)
THIRD_PARTY_DIR := ${PROJECT_DIR}/third_party
BUILD_DIR := ${PROJECT_DIR}/build
LIB_DIR := ${PROJECT_DIR}/lib
TEST_DIR := ${PROJECT_DIR }/test

.PHONY: build
build: 
	cd ${BUILD_DIR} && make

.PHONY: run-polynomial
run-polynomial:
	cd ${BUILD_DIR} && ./polynomial

.PHONY: run-regulators
run-pid:
	cd ${BUILD_DIR} && ./regulators

.PHONY: run-transforms
run-dft:
	cd ${BUILD_DIR} && ./transforms

.PHONY: run-filters
run-filters:
	cd ${BUILD_DIR} && ./filters

.PHONY: run-signals
run-signals:
	cd ${BUILD_DIR} && ./signals

.PHONY: clean
clean:
	cd ${PROJECT_DIR} && rm -rf ${BUILD_DIR}

.PHONY: cmake
cmake:
	cd ${PROJECT_DIR} && make clean && mkdir ${BUILD_DIR} && cmake -S . -B ${BUILD_DIR}

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

	