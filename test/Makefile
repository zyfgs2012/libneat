bins = a.out
target: a.out
a.out: cpp.o c.o
	@echo Compling Binary $@
	@g++ -o $@ $^
%.o: %.cpp
	@g++ -c $< -o $@

%.o: %.c
	@gcc -c $< -o $@

clean:
	@rm -f *.o
	@rm -f $(bins)
