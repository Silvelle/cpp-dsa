.DEFAULT_GOAL := build

P ?= 01
T ?= 01

PRACTICAL := practicals/$(P)
TASK_NAME := task$(T)
TASK := $(PRACTICAL)/$(TASK_NAME)
BUILD_DIR := $(PRACTICAL)/build
TARGET_DIR := $(BUILD_DIR)/$(TASK_NAME)-build
TARGET := $(TARGET_DIR)/$(TASK_NAME)

DOCS := docs/p$(P)
TASK_REPORT := $(DOCS)/$(TASK_NAME).typ

ifneq ($(wildcard $(TASK_REPORT)),)
REPORT := $(TASK_REPORT)
REPORT_OUT := $(DOCS)/output/practical-$(P)-$(TASK_NAME)-report.pdf
else
REPORT := $(DOCS)/main.typ
REPORT_OUT := $(DOCS)/output/practical-$(P)-report.pdf
endif

CPP_FILES := $(shell find "$(TASK)" -type f \( -name '*.cpp' -o -name '*.hpp' \) 2>/dev/null)

.PHONY: configure build run test results report format verify clean help

configure:
	@test -f "$(PRACTICAL)/CMakeLists.txt" || { \
		echo "Practical $(P) does not exist: $(PRACTICAL)"; exit 1; \
	}
	@test -f "$(TASK)/CMakeLists.txt" || { \
		echo "Task $(T) does not exist: $(TASK)"; exit 1; \
	}
	cmake -S "$(PRACTICAL)" -B "$(BUILD_DIR)"

build: configure
	cmake --build "$(BUILD_DIR)" --target "$(TASK_NAME)" --parallel

run: build
	@cd "$(TARGET_DIR)" && ./"$(TASK_NAME)"

test: configure
	cmake --build "$(BUILD_DIR)" --target "$(TASK_NAME)" --parallel
	ctest --test-dir "$(BUILD_DIR)" --output-on-failure -R "^$(TASK_NAME)$$"

# The task writes generated data and CSV files to its working directory.
# Running from TARGET_DIR keeps those artifacts inside the build tree.
results: run
	@echo "Results: $(TARGET_DIR)/results"

report:
	@test -f "$(REPORT)" || { echo "Report source not found: $(REPORT)"; exit 1; }
	@mkdir -p "$(dir $(REPORT_OUT))"
	typst compile --root "$(CURDIR)" "$(REPORT)" "$(REPORT_OUT)"
	@echo "Report: $(REPORT_OUT)"

format:
	@if [ -n "$(strip $(CPP_FILES))" ]; then \
		clang-format -i $(CPP_FILES); \
	else \
		echo "No C++ files found in $(TASK)"; \
	fi

verify: test
	@echo "P=$(P), T=$(T) passed verification"

clean:
	rm -rf "$(BUILD_DIR)"

help:
	@echo ""
	@echo "DSA repository commands"
	@echo ""
	@echo "  make [build] P=01 T=01  Configure and build one task"
	@echo "  make run P=01 T=01      Run it inside the build tree"
	@echo "  make test P=01 T=01     Run its CTest entry"
	@echo "  make results P=01 T=01  Generate CSV files in the build tree"
	@echo "  make format P=01 T=01   Format task C++ sources"
	@echo "  make verify P=01 T=01   Build and test one task"
	@echo "  make report P=01 T=01   Compile a task report (or the practical report)"
	@echo "  make clean P=01         Remove that practical's build tree"
	@echo ""
