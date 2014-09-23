MUKS = $(wildcard *.muk1)
SEED = $(patsubst %.muk1, %, $(MUKS))

all: $(SEED)

%: %.muk1
	@echo $<
	@echo Konvertiere nach MiniSEED
	@../build/kumy2seed $<
	@echo Konvertiere nach WAV
	@../build/kumy2wav $<
	@touch $@

clean:
	rm -rf $(SEED)
