CC = gcc
LDFLAGS = -lm
CFLAGS = -Wall -O3
OBJECTS = imagedup.o stb_image.o stb_image_resize.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

imagedup: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
debug:	CFLAGS= -Wall -O0 -g
debug:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o imagedup-$@ $(LDFLAGS)
clean:
	@echo cleaning oop
	@rm -f $(OBJECTS)
	@rm -f imagedup
	@rm -f imagedup














