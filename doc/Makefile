OUTPUT_DIR=build
BUILD_SUBDIRS=settings templates src
MAIN_TEX=src/main.tex

default: build

build: dirs
	latexmk -pdf -shell-escape -xelatex -outdir=$(OUTPUT_DIR) -interaction=nonstopmode $(MAIN_TEX)

dirs:
	mkdir -p $(OUTPUT_DIR)
	for subdir in $(BUILD_SUBDIRS); do mkdir -p $(OUTPUT_DIR)/$$subdir; done

clean:
	rm -rf $(OUTPUT_DIR)
