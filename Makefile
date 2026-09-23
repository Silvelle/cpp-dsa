.DEFAULT_GOAL := build

CXX ?= g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2

P ?= 01
T ?= 01

# Path to tasks
PRACTICAL := practicals/$(P)
TASK := $(PRACTICAL)/task$(T)

SRC := $(wildcard $(TASK)/src/*.cpp)
HEADERS := $(wildcard $(TASK)/include/*.hpp)
TEST_SRC := $(wildcard $(TASK)/tests/*.cpp)
LIB_SRC := $(filter-out $(TASK)/src/main.cpp,$(SRC))

INCLUDES := \
	-I$(TASK)/include \
	-Icommon/include

# C++ Build
BUILD := build/p$(P)/task$(T)
TARGET := $(BUILD)/task$(T)
TEST_TARGET := $(BUILD)/tests


# Experiment Results
RESULTS := $(PRACTICAL)/results


# Report
DOCS := docs/p$(P)
REPORT := $(DOCS)/main.typ
REPORT_OUT := $(DOCS)/output/report.pdf



# Targets
.PHONY: build run test results report format verify clean help


# Default:
# make P=01 T=01
build:
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)


# Compile and run one task:
# make run P=01 T=01
run: build
	$(TARGET)


# Run tests for one task:
# make test P=01 T=01
test:
	@echo "Running tests for P=$(P), T=$(T)"
	@if [ -n "$(strip $(TEST_SRC))" ]; then \
		mkdir -p "$(BUILD)"; \
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIB_SRC) $(TEST_SRC) -o "$(TEST_TARGET)"; \
		"$(TEST_TARGET)"; \
	else \
		echo "No test sources found in $(TASK)/tests; build check only."; \
	fi


# Generate experimental results:
# make results P=01 T=01
results: build
	@mkdir -p $(RESULTS)
	$(TARGET) --experiment > $(RESULTS)/task$(T).csv


# Compile report separately:
# make report P=01
report:
	@mkdir -p $(DOCS)/output
	typst compile $(REPORT) $(REPORT_OUT)


# Format all C++ files in one task.
format:
	clang-format -i $(SRC) $(HEADERS) $(TEST_SRC)


# Check C++ task only.
verify: build test
	@echo "P=$(P), T=$(T) passed verification"


# Remove generated C++ build files.
clean:
	rm -rf build


help:
	@echo ""
	@echo "DSA repository commands"
	@echo ""
	@echo "C++:"
	@echo "  make                     Build P=01 T=01"
	@echo "  make build P=01 T=01     Build task"
	@echo "  make run P=01 T=01       Build and run task"
	@echo "  make test P=01 T=01      Run task tests"
	@echo "  make results P=01 T=01   Generate experiment results"
	@echo "  make format P=01 T=01    Format task C++ files"
	@echo "  make verify P=01 T=01    Build and test task"
	@echo ""
	@echo "Report:"
	@echo "  make report P=01         Compile practical report"
	@echo ""
	@echo "Other:"
	@echo "  make clean               Remove C++ build files"
	@echo ""
