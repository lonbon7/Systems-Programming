write(out, &pid, sizeof(pid));
read(in, &pid, sizeof(pid));
total += pid;

close(in);

close(AtoB[0]);
close(CtoA[1]);

pipe(BtoC);

close(BtoC[0]);
close(AtoB[1]);

close(CtoA[0]);

close(BtoC[1]);