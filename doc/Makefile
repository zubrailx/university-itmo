MAIN_TEX=src/main.tex

BUILD_SUBDIRS=settings templates src
OUTPUT_DIR=build

FIG_DIR=fig
FIG_SOURCES_SVG=$(wildcard $(FIG_DIR)/*.svg)
FIG_TARGETS_PNG=$(patsubst %.svg, %.png, $(FIG_SOURCES_SVG))
DENSITY=120

default: install

install: build
	mv build/main.pdf ./

build: dirs $(FIG_TARGETS_PNG)
	latexmk -pdf -shell-escape -xelatex -outdir=$(OUTPUT_DIR) -interaction=nonstopmode $(MAIN_TEX)

%.png: %.svg
	convert -density $(DENSITY) $< $@

dirs:
	mkdir -p $(OUTPUT_DIR)
	for subdir in $(BUILD_SUBDIRS); do mkdir -p $(OUTPUT_DIR)/$$subdir; done

clean:
	rm -rf $(OUTPUT_DIR)
	rm -rf $(FIG_DIR)/*.png
