ssu_cp : ssu_cp.o ssu_option_s.o ssu_option_p.o ssu_option_r.o ssu_option_l.o 
	gcc -o ssu_cp ssu_cp_2.o ssu_option_s.o ssu_option_p.o ssu_option_r.o ssu_option_l.o

ssu_cp.o : ssu_cp.c
	gcc -c ssu_cp.c

ssu_option_p.o : ssu_option_p.c
	gcc -c ssu_option_p.c

ssu_option_s.o : ssu_option_s.c
	gcc -c ssu_option_s.c

ssu_option_r.o : ssu_option_r.c
	gcc -c ssu_option_r.c

ssu_option_d.o: ssu_option_d.c
	gcc -c ssu_option_d.c

