MUKS = $(wildcard *.muk1)
SEED = $(patsubst %.muk1, %, $(MUKS))

all: $(SEED)

%: %.muk1
	../build/kumy2seed $<
	@touch $@

clean:
	rm -rf $(SEED)
