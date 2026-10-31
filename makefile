# Compilación con Qt 6 y MinGW (MSYS2 UCRT64).
CMAKE = cmake
SOURCE_DRIVE = T:
SOURCE_DIR = $(SOURCE_DRIVE)/
BUILD_DIR = $(SOURCE_DIR)build
APP = build/bin/Semantico.exe
UCRT64 = C:/msys64/ucrt64

.PHONY: all build run clean rebuild

all: run

build:
	powershell -NoProfile -Command "if (!(Test-Path '$(SOURCE_DRIVE)\\CMakeLists.txt')) { if (Test-Path '$(SOURCE_DRIVE)\\') { throw 'Drive $(SOURCE_DRIVE) is already in use.' }; subst $(SOURCE_DRIVE) (Get-Location).Path }"
	$(CMAKE) -S $(SOURCE_DIR) -B $(BUILD_DIR) -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=$(UCRT64) -DCMAKE_CXX_COMPILER=$(UCRT64)/bin/g++.exe -DCMAKE_MAKE_PROGRAM=$(UCRT64)/bin/mingw32-make.exe
	$(CMAKE) --build $(BUILD_DIR) --config Release
	powershell -NoProfile -Command "subst $(SOURCE_DRIVE) /D"

run: build
	$(APP)

clean:
	powershell -NoProfile -Command "if (!(Test-Path '$(SOURCE_DRIVE)\\CMakeLists.txt')) { if (Test-Path '$(SOURCE_DRIVE)\\') { throw 'Drive $(SOURCE_DRIVE) is already in use.' }; subst $(SOURCE_DRIVE) (Get-Location).Path }"
	$(CMAKE) -E rm -rf $(BUILD_DIR)
	powershell -NoProfile -Command "subst $(SOURCE_DRIVE) /D"

rebuild:
	$(MAKE) clean
	$(MAKE) build
