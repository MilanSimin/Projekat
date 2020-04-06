src = $(wildcard *.cpp)

  LDFLAGS = -std=c++11
  SYSTEMC_INCL = -I ${SYSTEMC}/include -L ${SYSTEMC}/lib-linux64 -lsystemc

  myprog: $(src)
	g++ $(LDFLAGS) $(SYSTEMC_INCL) -o $@ $^

  .PHONY: clean
  clean:
	rm -f $(obj) myprog
