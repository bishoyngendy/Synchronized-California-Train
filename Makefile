
runner: caltrain-runner.o caltrain.o
	gcc caltrain-runner.o caltrain.o -pthread -o runner

caltrain-runner.o: caltrain-runner.o
	gcc -c caltrain-runner.c

caltrain.o: caltrain.c
	gcc -c caltrain.c

clean:
	rm *.o runner
