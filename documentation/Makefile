HTMLFLAGS = --book --duplex --no-numbered --format pdf14 --title --size letter

%.pdf: %.html
	-htmldoc $(HTMLFLAGS) --outfile $@ $<

all: install

doxygen:
	doxygen

installDoxygenHTML: doxygen
	mkdir -p ../html
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
	rm -f *.pdf
	rm -rf DoxygenHTML 
	rm -rf ../html
