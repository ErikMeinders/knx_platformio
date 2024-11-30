# PlatformIO environment
PIO = source ~/.platformio/penv/bin/activate && pio

# Directories to watch
SRC_DIR = src
INCLUDE_DIR = include
DATA_DIR = data
LIB_DIR = ../..

# Files to watch for library changes
LIB_FILES = $(shell find $(LIB_DIR)/src $(LIB_DIR)/include -type f 2>/dev/null)
LIB_META = $(LIB_DIR)/library.json

# Source and header files
SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.cpp' -o -name '*.c' 2>/dev/null)
HEADER_FILES = $(shell find $(INCLUDE_DIR) -type f -name '*.h' 2>/dev/null)
DATA_FILES = $(shell find $(DATA_DIR) -type f 2>/dev/null)

# Timestamp files to track last build
FIRMWARE_TIMESTAMP = .pio.nosync/.firmware_timestamp
SPIFFS_TIMESTAMP = .pio.nosync/.spiffs_timestamp

# Create directories if they don't exist
$(shell mkdir -p .pio.nosync)

# Default target
all: build-firmware build-spiffs

# Build and upload firmware
build-firmware: $(FIRMWARE_TIMESTAMP)
$(FIRMWARE_TIMESTAMP): $(SRC_FILES) $(HEADER_FILES) $(LIB_FILES) $(LIB_META)
	@echo "Source files changed, rebuilding and uploading firmware..."
	@$(PIO) run -t upload || (echo "Firmware build/upload failed"; exit 1)
	@touch $(FIRMWARE_TIMESTAMP)

# Build and upload SPIFFS
build-spiffs: $(SPIFFS_TIMESTAMP)
$(SPIFFS_TIMESTAMP): $(DATA_FILES)
	@echo "Data files changed, uploading filesystem..."
	@$(PIO) run -t uploadfs || (echo "SPIFFS upload failed"; exit 1)
	@touch $(SPIFFS_TIMESTAMP)

# Clean only build artifacts in .pio.nosync/build
clean:
	@echo "Cleaning build artifacts in .pio.nosync/build..."
	@rm -f $(FIRMWARE_TIMESTAMP) $(SPIFFS_TIMESTAMP)
	@rm -rf .pio.nosync/build
	@echo "Note: Only removed build artifacts, all source files are preserved"

# Clean everything in .pio.nosync
distclean:
	@echo "Deep cleaning all artifacts in .pio.nosync..."
	@rm -rf .pio.nosync
	@echo "Note: Only removed .pio.nosync directory, all source files are preserved"

# Helper target to show what would be rebuilt
check:
	@echo "Checking for changes..."
	@if [ -n "$$(find $(SRC_FILES) $(HEADER_FILES) $(LIB_FILES) $(LIB_META) -newer $(FIRMWARE_TIMESTAMP) 2>/dev/null 2>/dev/null)" ]; then \
		echo "Firmware needs rebuild"; \
	fi
	@if [ ! -f $(SPIFFS_TIMESTAMP) ] || [ -n "$$(find $(DATA_FILES) -newer $(SPIFFS_TIMESTAMP) 2>/dev/null)" ]; then \
		echo "SPIFFS needs rebuild"; \
	fi

# Force rebuild of specific components
force-firmware:
	@rm -f $(FIRMWARE_TIMESTAMP)
	@$(MAKE) build-firmware

force-spiffs:
	@rm -f $(SPIFFS_TIMESTAMP)
	@$(MAKE) build-spiffs

.PHONY: all clean distclean check build-firmware build-spiffs force-firmware force-spiffs
