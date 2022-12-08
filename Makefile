# Övning 1
test-ovn1a: 
	printf "#define N_PTHREADS (100)\n#define UPDATE_COUNT (100)\n" > ovn1_test.h
	gcc ovn1.c -include ovn1_test.h -o ovn1 -pthread
	rm ovn1_test.h -f
	bash test/test_ovn1.sh 100 100

test-ovn1b : 
	printf "#define N_PTHREADS (5000)\n#define UPDATE_COUNT (10000)\n" > ovn1_test.h
	gcc ovn1.c -include ovn1_test.h -o ovn1 -pthread
	rm ovn1_test.h -f
	bash test/test_ovn1.sh 5000 10000

ovn1: ovn1.c
	gcc ovn1.c -include ovn1.h -o ovn1 -pthread

clean-ovn1:
	rm -f ovn1

# Övning 2
test-ovn2 : ovn2
	bash test/test_ovn2.sh

ovn2: ovn2.c
	gcc ovn2.c -o ovn2 -pthread

clean-ovn2:
	rm -f ovn2

# För att rensa temporära filer
clean: clean-ovn1 clean-ovn2
