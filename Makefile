MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR_PATH := $(patsubst %/,%,$(dir $(MAKEFILE_PATH)))
MAKEFILE_DIR_NAME := $(notdir $(MAKEFILE_DIR_PATH))
# $(info )
# $(info MAKEFILE_LIST=$(MAKEFILE_LIST))
# $(info MAKEFILE_PATH=$(MAKEFILE_PATH))
# $(info MAKEFILE_DIR_PATH=$(MAKEFILE_DIR_PATH))
# $(info MAKEFILE_DIR_NAME=$(MAKEFILE_DIR_NAME))
# $(info )


# build platform detection to make Makefile logic easier later
# todo: are we re-inventing the wheel?
BUILD_PLATFORMS=
BUILD_PLATFORMS+=WIN32_MINGW64
BUILD_PLATFORMS+=LINUX

CURRENT_BUILD_PLATFORM=

ifeq ($(OS),Windows_NT)
ifneq ($(filter $(MSYSTEM),MINGW64),)
CURRENT_BUILD_PLATFORM=WIN32_MINGW64
SHELL := /bin/bash
else
$(error Unsupported Platform! Use MSYS2 MinGW64 on Windows.)
endif # ifneq ($(filter $(MSYSTEM),MINGW64),)
else
CURRENT_BUILD_PLATFORM=LINUX
SHELL := /bin/bash
endif # ifeq ($(OS),Windows_NT)

$(info CURRENT_BUILD_PLATFORM=$(CURRENT_BUILD_PLATFORM))
$(info OS=$(OS))


ADDITIONAL_CMAKE_OPTIONS ?=
# If 'on', then the progress messages are printed. If 'off', makes it easier
# to detect actual warnings and errors  in the build output.
RULE_MESSAGES ?= on

CMAKE_BUILD_DIR := $(MAKEFILE_DIR_PATH)/build
CMAKE_SOURCE_DIR := $(MAKEFILE_DIR_PATH)
PREFIX ?= $(MAKEFILE_DIR_PATH)/install

CURRENT_DATE := $(shell date "+%d_%b_%Y")
CURRENT_DATE_FULL := $(shell date "+%d_%B_%Y")
CURRENT_TIME := $(shell date +"%H_%M_%S")

# we will use the last tag of the following format for using in the deployment package:
# 'vX.Y.Z-prerelease.*' (examples: v2.0.0-beta, v2.0.0-beta.5, v2.1.0-alpha, v2.1.0-alpha.1 etc.)
# only get upto vX.Y.Z-prerelease, ignore any .* after it.
# to get the latest tag of the semantic versioning: https://stackoverflow.com/a/64438619
# remember to have no 'spaces' after that last paranthesis below, in $(shell ...)<--HERE
VERSION := $(shell git tag -l | grep -o "^v.*\..*\..*-[a-z]*" | sort -V | tail -1 | sed -e 's/[[:space:]]*$$//g')
ifeq ($(VERSION),)
VERSION := v0.0.0-alpha
endif

GIT_HASH := $(shell git rev-parse --short HEAD)
ifeq ($(GIT_HASH),)
GIT_HASH := zyxwvutsrqponmlkjihgfedcba
endif

CURRENT_BRANCH := $(shell git symbolic-ref HEAD --short 2>/dev/null || echo "no_branch")

$(info CURRENT_BRANCH=$(CURRENT_BRANCH))


DEPLOY_PACKAGE_DIR_PATH := $(CMAKE_SOURCE_DIR)/deploy
LINUXDEPLOYQT_APPIMAGE_FILE_PATH := $(CMAKE_BUILD_DIR)/linuxdeployqt-continuous-x86_64.AppImage
MAKESELF_2_3_1_FILE_PATH := $(CMAKE_BUILD_DIR)/makeself-2.3.1/makeself.sh
# win32 style paths, convert using cygpath:
DEPLOY_PACKAGE_DIR_PATH_W=$(shell cygpath -w "$(DEPLOY_PACKAGE_DIR_PATH)" | sed 's/\\/\\\\/g')


.DEFAULT_GOAL := all

.PHONY: all
all: install


.PHONY: install
install: run-cmake
	@rm -rf $(PREFIX)
#$(MAKE) pre-install
	$(MAKE) -C build install
#$(MAKE) post-install
	@echo "local install ready."


# # ref: https://stackoverflow.com/questions/62218250/wrapping-cmake-build-with-makefile/62224300#62224300
# # this rule ensures that if CMakeLists.txt exists, then the Makefile generation using cmake is always run.
# # For every target generated from the CMake, ensure that it depends on $(CMAKE_BUILD_DIR)/Makefile
# $(CMAKE_BUILD_DIR)/Makefile: $(CMAKE_SOURCE_DIR)/CMakeLists.txt
# ifeq ($(OS),Windows_NT)
# ifneq ($(filter $(MSYSTEM),MINGW64),)
# 	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -DSQUINTYMONGREL_VERSION=$(VERSION) -DSQUINTYMONGREL_GIT_HASH=$(GIT_HASH) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR) -G "MSYS Makefiles"
# else # ifneq ($(filter $(MSYSTEM),MINGW64),)
# 	$(info Unsupported Platform! Use MSYS2 MinGW64 on Windows.)
# endif # ifneq ($(filter $(MSYSTEM),MINGW64),)
# else # ifeq ($(OS),Windows_NT)
# 	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -DSQUINTYMONGREL_VERSION=$(VERSION) -DSQUINTYMONGREL_GIT_HASH=$(GIT_HASH) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR)
# endif # ifeq ($(OS),Windows_NT)

# phony target to force cmake run
.PHONY: run-cmake
run-cmake:
ifeq ($(CURRENT_BUILD_PLATFORM),WIN32_MINGW64)
	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -DSQUINTYMONGREL_VERSION=$(VERSION) -DSQUINTYMONGREL_GIT_HASH=$(GIT_HASH) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR) -G "MSYS Makefiles"
else ifeq ($(CURRENT_BUILD_PLATFORM),LINUX)
	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -DSQUINTYMONGREL_VERSION=$(VERSION) -DSQUINTYMONGREL_GIT_HASH=$(GIT_HASH) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BUILD_DIR)
endif # CURRENT_BUILD_PLATFORM test


# phony target to enable repo update
.PHONY: update
update:
	git fetch --tags
	git fetch
	git submodule update --init --recursive

# https://github.com/AppImage/AppImageKit/wiki/AppDir
.PHONY: deploy
deploy: install
ifeq ($(CURRENT_BUILD_PLATFORM),WIN32_MINGW64)

	$(error deploy on $(CURRENT_BUILD_PLATFORM) is not supported yet!)

else ifeq ($(CURRENT_BUILD_PLATFORM),LINUX)

# create a fresh 'deploy' package dir
	@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)
	@mkdir -p $(DEPLOY_PACKAGE_DIR_PATH)
# copy the build into the deploy package dir, use a filter as needed, to put stuff into the deploy package
	@cd $(PREFIX) && find . | grep -f $(CMAKE_SOURCE_DIR)/deployment_filter.txt | xargs -d '\n' cp -r --parents -t $(DEPLOY_PACKAGE_DIR_PATH)/usr
# copy docs into 'AppDir'/usr/docs

# create 'AppImage' using linuxdeployqt
	@echo "creating deployment package using linuxdeployqt..."
ifeq ("$(wildcard $(LINUXDEPLOYQT_APPIMAGE_FILE_PATH))","")
	@wget -q -O $(LINUXDEPLOYQT_APPIMAGE_FILE_PATH) https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
	@chmod +x $(LINUXDEPLOYQT_APPIMAGE_FILE_PATH)
endif # if LINUXDEPLOYQT_APPIMAGE_FILE_PATH does not exist
	cd $(DEPLOY_PACKAGE_DIR_PATH) && $(LINUXDEPLOYQT_APPIMAGE_FILE_PATH) $(DEPLOY_PACKAGE_DIR_PATH)/usr/share/applications/squintymongrel.desktop -appimage -no-translations &> $(CMAKE_SOURCE_DIR)/linuxdeployqt.log || true

# remove the artifacts from linuxdeployqt
#@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)/*.AppImage
	@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)/AppRun
	@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)/squintymongrel.desktop
	@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)/squintymongrel.png
	@rm -rf $(DEPLOY_PACKAGE_DIR_PATH)/.DirIcon
# rename the 'AppDir'/usr into the application name and this is our 'portable' deploy package
	@mv $(DEPLOY_PACKAGE_DIR_PATH)/usr $(DEPLOY_PACKAGE_DIR_PATH)/squintymongrel_$(VERSION)

# create a Makeself self-extracting archive
	@echo "creating self-extracting archive using makeself..."
ifeq ("$(wildcard $(MAKESELF_2_3_1_FILE_PATH))","")
	@cd $(CMAKE_BUILD_DIR) && wget -q https://github.com/megastep/makeself/releases/download/release-2.3.1/makeself-2.3.1.run
	@cd $(CMAKE_BUILD_DIR) && chmod +x makeself-2.3.1.run
	@cd $(CMAKE_BUILD_DIR) && ./makeself-2.3.1.run -q --nox11 1>/dev/null
endif # if MAKESELF_2_3_1_FILE_PATH does not exist
	@$(MAKESELF_2_3_1_FILE_PATH) --notemp --nox11 --gzip "$(DEPLOY_PACKAGE_DIR_PATH)/squintymongrel_$(VERSION)" "$(DEPLOY_PACKAGE_DIR_PATH)/squintymongrel_$(VERSION).gz.run" "squintymongrel Installer" echo "squintymongrel Installation Done!" &> $(CMAKE_SOURCE_DIR)/makeself.log

endif # CURRENT_BUILD_PLATFORM


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