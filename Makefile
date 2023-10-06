MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR_PATH := $(patsubst %/,%,$(dir $(MAKEFILE_PATH)))
MAKEFILE_DIR_NAME := $(notdir $(MAKEFILE_DIR_PATH))
# $(info )
# $(info MAKEFILE_LIST=$(MAKEFILE_LIST))
# $(info MAKEFILE_PATH=$(MAKEFILE_PATH))
# $(info MAKEFILE_DIR_PATH=$(MAKEFILE_DIR_PATH))
# $(info MAKEFILE_DIR_NAME=$(MAKEFILE_DIR_NAME))
# $(info )

PREFIX ?= $(MAKEFILE_DIR_PATH)/install
$(info PREFIX=$(PREFIX))

SHELL := /bin/bash
DEFAULT_BRANCH := main

CMAKE_BUILD_DIR := $(MAKEFILE_DIR_PATH)/build
CMAKE_SOURCE_DIR := $(MAKEFILE_DIR_PATH)


CURRENT_DATE := $(shell date "+%d_%b_%Y")
CURRENT_DATE_FULL := $(shell date "+%d_%B_%Y")
CURRENT_TIME := $(shell date +"%H_%M_%S")
$(info CURRENT_DATE=$(CURRENT_DATE))
$(info CURRENT_TIME=$(CURRENT_TIME))


VERSION := $(shell git describe --tags --abbrev=0)
ifeq ($(VERSION),)
VERSION := v0.0.0
endif
$(info VERSION=$(VERSION))


GIT_REPO_URL := $(shell git remote get-url origin | sed -E 's,https\://api\:.+github,github,g')
ifeq ($(GIT_REPO_URL),)
GIT_REPO_URL := unknown_url
endif
$(info GIT_REPO_URL=$(GIT_REPO_URL))


GIT_HASH := $(shell git rev-parse HEAD)
ifeq ($(GIT_HASH),)
GIT_HASH := zyxwvutsrqponmlkjihgfedcba
endif
$(info GIT_HASH=$(GIT_HASH))


CURRENT_BRANCH := $(shell git symbolic-ref HEAD --short 2>/dev/null || echo "no_branch")
$(info CURRENT_BRANCH=$(CURRENT_BRANCH))


# Qt specific variables to choose a specific Qt installation.
QMAKE_PATH ?=
ifneq ($(QMAKE_PATH),)
QMAKE_DIR_PATH := $(patsubst %/,%,$(dir $(QMAKE_PATH)))
CMAKE_PREFIX_PATH := $(abspath $(QMAKE_DIR_PATH)/..)
export PATH := $(CMAKE_PREFIX_PATH)/bin:$(PATH)
$(info QMAKE_PATH=$(QMAKE_PATH))
$(info CMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH))
$(info PATH=$(PATH))
endif


.DEFAULT_GOAL := all

.PHONY: all
all: install


.PHONY: install
install: run-cmake
	@rm -rf $(PREFIX)
#$(MAKE) pre-install
	$(MAKE) -C build install
#$(MAKE) post-install
	@echo
	@echo
	@echo "local install ready:" $(shell realpath --relative-to="$(CMAKE_SOURCE_DIR)" "$(PREFIX)")"/bin/squintymongrel"


# phony target to force cmake run
.PHONY: run-cmake
run-cmake:
	cmake \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(CMAKE_PREFIX_PATH) \
		-DBUILD_DATE=$(CURRENT_DATE) \
		-DBUILD_TIME=$(CURRENT_TIME) \
		-DBUILD_VERSION=$(VERSION) \
		-DBUILD_GIT_HASH=$(GIT_HASH) \
		-DBUILD_GIT_REPO_URL=$(GIT_REPO_URL) \
		-S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR)


.PHONY: clean
clean:
ifeq ("$(wildcard $(CMAKE_BUILD_DIR))","")
	$(info nothing to clean, CMake build directory does not exist!)
else
	$(MAKE) -C build clean
endif


.PHONY: distclean
distclean:
ifeq ("$(wildcard $(CMAKE_BUILD_DIR))","")
	$(info nothing to clean, CMake build directory does not exist!)
else
	$(MAKE) -C build clean
endif
	@rm -rf $(CMAKE_BUILD_DIR)
	@rm -rf $(CMAKE_SOURCE_DIR)/resources/monaco-editor/
	@rm -rf $(CMAKE_SOURCE_DIR)/resources/monaco-editor-*.tgz
	@rm -rf $(PREFIX)


# phony target to enable repo update
.PHONY: update
update:
	git fetch --tags
	git fetch
	git submodule update --init --recursive


.PHONY: prune
prune:
ifneq ($(CURRENT_BRANCH), $(DEFAULT_BRANCH))
	$(error must be on the default branch \'$(DEFAULT_BRANCH)\' to make prune!)
endif
# clean up branches
# remove pointers to remote branches that don't exist
	git fetch --prune
	git fetch --tags --force --prune
# delete local branches which don't have remotes (merged only)
#git branch -vv | grep ': gone]' | awk '{print $1}' | xargs git branch -d || true
# force delete local branches without remotes (unmerged)
	git branch -vv | grep ': gone]' | awk '{print $1}' | xargs git branch -D || true


# phony target to reset repo to pristine state (use with caution!)
.PHONY: pristine
pristine:
ifneq ($(CURRENT_BRANCH), $(DEFAULT_BRANCH))
	$(error must be on the default branch \'$(DEFAULT_BRANCH)\' to make pristine!)
endif
	git fetch --tags --force --prune
	git fetch
	git reset --hard HEAD
# we want to continue with current branch itself.
	git pull --ff-only || true
	git submodule update --init --recursive
	git submodule foreach --recursive git reset --hard

.PHONY: dummy
dummy:
	$(info doing nothing here.)

# examples:
# make distclean
# make -j$(nproc) 2>&1 | tee build_default.log
# make -j$(nproc) QMAKE_PATH=/usr/lib/x86_64-linux-gnu/qt5/bin/qmake 2>&1 | tee build_5_12_8.log
# make -j$(nproc) QMAKE_PATH=/home/${USER}/qt/5.15.2/gcc_64/bin/qmake 2>&1 | tee build_5_15_2.log

