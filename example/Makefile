page_images := img1 img2 img3
column_images := img4
images = $(page_images) $(column_images)

journals := epjc nima prc

generic_targets := $(addsuffix _generic,$(journals))
journal_targets := $(addsuffix _canu,$(journals))

.PHONY: all clean

all: $(generic_targets) $(journal_targets)

papers/generic/%.C : macros/%.C
	root $< -b -q

define GENERIC_RULE
$(1)_generic : papers/$(1)/$(1)_generic.pdf

papers/$(1)/$(1)_generic.pdf : papers/$(1)/$(1)_generic.tex papers/common_generic.tex $(addprefix papers/generic/,$(addsuffix .C,$(images)))
	@echo "Building target: $$@"
	@echo "Dependencies: $$^"
	cd papers/$(1) && pwd && pdflatex $(1)_generic.tex
endef

$(foreach j,$(journals), $(eval $(call GENERIC_RULE,$(j))))

define JOURNAL_RULE

papers/$(1)/canu.hpp : ../include/canu/canu.hpp
	cd papers/$(1) && ln -s ../../../include/canu/canu.hpp .

$(addprefix papers/$(1)/out/,$(addsuffix .pdf,$(page_images))) : papers/$(1)/out/%.pdf : papers/generic/%.C papers/$(1)/canu.hpp
	@echo "Refreshing page images $$@ from $$<"
	cd papers/$(1) && ./wrapper_canu_page.sh ../../$$<

$(addprefix papers/$(1)/out/,$(addsuffix .pdf,$(column_images))) : papers/$(1)/out/%.pdf : papers/generic/%.C papers/$(1)/canu.hpp
	@echo "Refreshing column images $$@ from $$<"
	cd papers/$(1) && ./wrapper_canu_column.sh ../../$$<

$(1)_canu : papers/$(1)/$(1)_canu.pdf $(addprefix papers/$(1)/out/,$(addsuffix .pdf,$(images)))

papers/$(1)/$(1)_canu.pdf : papers/$(1)/$(1)_canu.tex papers/common_journal.tex $(addprefix papers/$(1)/out/,$(addsuffix .pdf,$(images)))
	@echo "Building target: $$@"
	@echo "Dependencies: $$^"
	cd papers/$(1) && pwd && pdflatex $(1)_canu.tex

clean_$(1):
	rm -rf papers/$(1)/out
	rm -rf papers/$(1)/canu.hpp
	cd papers/$(1) && latexmk -C
endef

$(foreach j,$(journals), $(eval $(call JOURNAL_RULE,$(j))))

print-%:
	@echo '$*=$($*)'

clean: $(addprefix clean_,$(journals))
	rm -rf papers/generic/img*
