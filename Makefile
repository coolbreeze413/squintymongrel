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


CMAKE_BUILD_DIR := $(MAKEFILE_DIR_PATH)/build
CMAKE_SOURCE_DIR := $(MAKEFILE_DIR_PATH)


CURRENT_DATE := $(shell date "+%d_%b_%Y")
CURRENT_DATE_FULL := $(shell date "+%d_%B_%Y")
CURRENT_TIME := $(shell date +"%H_%M_%S")
$(info CURRENT_DATE=$(CURRENT_DATE))
$(info CURRENT_TIME=$(CURRENT_TIME))


VERSION := $(shell git describe --abbrev=0)
ifeq ($(VERSION),)
VERSION := v0.0.0-alpha
endif
$(info VERSION=$(VERSION))


GIT_HASH := $(shell git rev-parse HEAD)
ifeq ($(GIT_HASH),)
GIT_HASH := zyxwvutsrqponmlkjihgfedcba
endif
$(info GIT_HASH=$(GIT_HASH))


CURRENT_BRANCH := $(shell git symbolic-ref HEAD --short 2>/dev/null || echo "no_branch")
$(info CURRENT_BRANCH=$(CURRENT_BRANCH))


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
	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DSQUINTYMONGREL_VERSION=$(VERSION) -DSQUINTYMONGREL_GIT_HASH=$(GIT_HASH) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR)


.PHONY: clean
clean:
	$(MAKE) -C build clean

.PHONY: distclean
distclean:
	$(MAKE) -C build clean
	rm -rf $(CMAKE_BUILD_DIR)
	rm -rf $(CMAKE_SOURCE_DIR)/resources/monaco-editor/
	rm -rf $(CMAKE_SOURCE_DIR)/resources/monaco-editor-*.tgz
	rm -rf $(PREFIX)

# phony target to enable repo update
.PHONY: update
update:
	git fetch --tags
	git fetch
	git submodule update --init --recursive


.PHONY: prune
prune:
	echo "TODO ensure we are on master!!!!"
	echo "TODO prune tags as well!!!!"
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
	echo "TODO ensure we are on master!!!!"
	git fetch --tags --force --prune
	git fetch
	git reset --hard HEAD
# we want to continue with current branch itself.
# git checkout master
	git pull --ff-only || true
	git submodule update --init --recursive
	git submodule foreach --recursive git reset --hard

.PHONY: dummy
dummy:
	$(info doing nothing here.)