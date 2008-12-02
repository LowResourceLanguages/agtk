CPPFLAGS = -I/usr/lib/perl5/5.8.0/i686-linux/CORE -I../../ag
LDFLAGS = -L../../ag/.libs -lag

ag.so: ag.o
	gcc -shared -o $@ $^ $(LDFLAGS)

ag.cc: ../AGAPI.i
	swig -perl -c++ -module ag -o ag.cc -I.. ../AGAPI.i

clean:
	rm -f *.o *.so

