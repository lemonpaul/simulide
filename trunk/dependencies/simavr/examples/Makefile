boards=board_ds1338 board_i2ctest
all:
	for bi in ${boards}; do echo $$bi; $(MAKE) -C $$bi; done

clean:
	for bi in ${boards}; do echo $$bi; $(MAKE) -C $$bi clean; done
