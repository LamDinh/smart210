cc=arm-linux-gcc
strip=arm-linux-strip

clock_lcd7inch: main.o lcd_driver.o date_time.o
	$(cc) -o clock_lcd7inch *.o -lpthread -lm
	$(strip) clock_lcd7inch

main.o: main.c ./images/*.h ./includes/*.h
	$(cc) -c main.c

lcd_driver.o: lcd_driver.c ./images/*.h ./includes/*.h
	$(cc) -c lcd_driver.c

date_time.o: date_time.c ./images/*.h ./includes/*.h
	$(cc) -c date_time.c

clean:
	rm -rf clock_lcd7inch *.o
