HTMLFLAGS = --book --duplex --no-numbered --format pdf14 --title --size letter

%.pdf: %.html
	-htmldoc $(HTMLFLAGS) --outfile $@ $<

all: install

doxygen:
	@$(RM) -rf DoxygenHTML 
	doxygen

installDoxygenHTML: doxygen
	@$(RM) -rf ../html
	@$(MKDIR) -p ../html
	cp -r DoxygenHTML/* ../html

ifneq ($(wildcard $(shell which doxygen)),)
install: installDoxygenHTML
endif

install:
	# cp *.html ../html
	# cp *.jpg ../html
	# cp *.png ../html
	# cp *.pdf ../html

clean:

realclean: clean
	$(RM) -f *.pdf
	$(RM) -rf DoxygenHTML 
	$(RM) -rf ../html
