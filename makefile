ifndef $(CC)
	CC=gcc
endif

ifndef $(LD)
	LD=gcc
endif

ifndef $(DB)
	DB=valgrind
endif

ifndef $(EXECCMD)
	EXECCMD=exec
endif

CCFLAGS+=-I./src/ -fms-extensions -Wall -g -D_XOPEN_SOURCE_EXTENDED
LDFLAGS+=-lncursesw

EXEC=matrice
BINDIR=/usr/bin
SRCDIR=src

OBJECTS=$(SRCDIR)/main.o          \
        $(SRCDIR)/widget/widget.o \
        $(SRCDIR)/widget/panel.o  \
        $(SRCDIR)/widget/label.o  \
        $(SRCDIR)/widget/list.o   \
        $(SRCDIR)/widget/grid.o   \
		$(SRCDIR)/interface.o     \
        $(SRCDIR)/matrice.o       \
        $(SRCDIR)/misc.o

all: mkdir $(EXEC)

$(EXEC): $(OBJECTS)
	@printf "\tLD %s\n" bin/$(EXEC)
	@$(LD) -o bin/$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	@printf "\tCC %s\n" $@
	@$(CC) -o $@ -c $< $(CCFLAGS)

mkdir:
	@mkdir -p bin

clean:
	rm -rf $(OBJECTS)
	rm -rf ./bin/$(EXEC)

run: $(EXEC)
	$(EXECCMD) ./bin/$(EXEC) $(ARGS)

debug: $(EXEC)
	$(DB) bin/$(EXEC)
