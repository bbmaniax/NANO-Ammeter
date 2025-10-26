SKETCH ?= NANO-Ammeter.ino
CORES ?= arduino:avr
LIBS ?= "Adafruit SSD1306"

BUILD_CONFIG ?= ./arduino-cli.yaml
BUILD_DIR ?= ./build

DEPLOY_CMD ?= /mnt/c/Windows/System32/robocopy.exe
DEPLOY_DIR ?= D:/

TEMP_DIR ?= ./tmp

define build-sketch
	arduino-cli compile \
		--library ./src \
		--fqbn $(1) \
		--export-binaries \
		$(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") \
		./$(2);
endef

.PHONY: all
all: clean install build

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMP_DIR)
	find . -type f -exec chmod -x {} +

.PHONY: build
build: build/nano

.PHONY: build/nano
build/nano:
	$(call build-sketch,arduino:avr:nano,$(SKETCH))

.PHONY: install
install: core/install lib/install

.PHONY: core/install
core/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) core update-index
	arduino-cli --config-file $(BUILD_CONFIG) core install $(CORES)

.PHONY: lib/install
lib/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) lib update-index
	arduino-cli --config-file $(BUILD_CONFIG) lib install $(LIBS)

.PHONY: deploy
deploy:
	-$(DEPLOY_CMD) "$(subst /,\,$(BUILD_DIR)/$(subst :,.,$(FQBN)))" "$(DEPLOY_DIR)" *.uf2
