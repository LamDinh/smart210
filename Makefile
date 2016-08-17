cc=arm-linux-gcc
strip=arm-linux-strip

loving_clock: main.o lcd_driver.o date_time.o
	$(cc) -o loving_clock *.o -lpthread -lm
	$(strip) loving_clock

main.o: main.c ./images/*.h ./includes/*.h
	$(cc) -c main.c

lcd_driver.o: lcd_driver.c ./images/*.h ./includes/*.h
	$(cc) -c lcd_driver.c

date_time.o: date_time.c ./images/*.h ./includes/*.h
	$(cc) -c date_time.c

clean:
	rm -rf loving_clock *.o
