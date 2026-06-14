# Nazwa pliku głównego (bez .tex)
MAIN = main

# Katalogi
BUILD_DIR = build

TEX_FILES = $(shell find . -name "*.tex")

PDF = $(BUILD_DIR)/$(MAIN).pdf

all: $(PDF)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PDF): $(TEX_FILES) $(BUILD_DIR)
	pdflatex -output-directory=$(BUILD_DIR) $(MAIN).tex
	pdflatex -output-directory=$(BUILD_DIR) $(MAIN).tex

clean:
	rm -rf $(BUILD_DIR)

view: $(PDF)
	xdg-open $(PDF) &

rebuild: clean all
