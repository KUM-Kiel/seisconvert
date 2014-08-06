MAKEHTML = groff -t -mandoc -Thtml

all: biquad_filter.html biquad_filter_init.html

%.html: %.3
	$(MAKEHTML) $< > $@

clean:
	rm *.html
