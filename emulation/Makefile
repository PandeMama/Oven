web: web.cpp mongoose.c
	$(CXX) $(CXXFLAGS) web.cpp -c -o web.o
	$(CC) $(CCFLAGS) mongoose.c -c -o mongoose.o
	$(CXX) $(LDFLAGS) web.o mongoose.o -o web

mongoose: mongoose.h mongoose.c

mongoose.h:
	curl https://github.com/cesanta/mongoose/raw/master/mongoose.h -L > mongoose.h

mongoose.c:
	curl https://github.com/cesanta/mongoose/raw/master/mongoose.c -L > mongoose.c

clean:
	rm *.o web
