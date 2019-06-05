$ # U linijama 23-24 u main.c postavlja se varijanta zadatka (A ili B).  Prije
$ # nego sto sam procitao zadatak s razumijevanjem sam ga rijesio pa nisam bio
$ # svjestan da je dovoljno rijesiti samo jednu varijantu.  Moguce da B varijanta
$ # nije u potpunosti tocna.
$ #
$ # Toplo preporucam da u linijama 378-379 u main.c bude osigurano da je varijabla
$ # izlaz postavljena na stdout jer je u tom slucaju ispis lijepo obojan (na
$ # primjer, slobodni dio memorije obojan je u zeleno, a zauzeti u crveno) ---
$ # barem u suvremenim Linux terminalima.
$ #
$ gcc main.c -o main -Wall -O -pthread


$ ./main # A varijanta
10 dretvi: 5 generatora + 5 provjeravaca.

7, 0x5555555555554a94, uzeo broj.
MEM: 777777777777----------------------------------------------------
9, 0x92aaaaaaaaaaa554, uzeo broj.
MEM: 777777777777----------------------------------------------------
9, 0x92aaaaaaaaaaa554, potrosio broj.
9, 0x5555555555555b55, uzeo broj.
MEM: 7777777777779---------------------------------------------------
8, 0xb2aaaaaaaaaaaaaa, uzeo broj.
5, 0xcaaaaaaaaaaaab55, uzeo broj.
MEM: 77777777777798888888888888888885555555555555--------------------
6, 0xd5555555555554aa, uzeo broj.
MEM: 7777777777779888888888888888888555555555555566------------------
MEM: 7777777777779888888888888888888555555555555566------------------
9, 0x5555555555555b55, potrosio broj.
9, 0xd55555555555599b, uzeo broj.
MEM: 777777777777-8888888888888888885555555555555669999999-----------
7, 0x5555555555554a94, potrosio broj.
7, 0x655555555555694b, uzeo broj.
MEM: -------------888888888888888888555555555555566999999977777777777
6, 0xd5555555555554aa, potrosio broj.
6, 0xd55555555555552a, uzeo broj.
MEM: 6666666666---8888888888888888885555555555555--999999977777777777
6, 0xd55555555555552a, potrosio broj.
6, 0x2aaaaaaaaaaaad95, uzeo broj.
MEM: 666666666----8888888888888888885555555555555--999999977777777777
7, 0x655555555555694b, potrosio broj.
7, 0xd555555555554d96, uzeo broj.
MEM: 666666666----8888888888888888885555555555555--99999997777777777-
7, 0xd555555555554d96, potrosio broj.
7, 0x2555555555554b6c, uzeo broj.
MEM: 666666666----8888888888888888885555555555555--9999999-----------
9, 0xd55555555555599b, potrosio broj.
9, 0x52aaaaaaaaaaaad4, uzeo broj.
MEM: 66666666699998888888888888888885555555555555--------------------
7, 0x2555555555554b6c, potrosio broj.
7, 0x9555555555556cd4, uzeo broj.
MEM: 666666666999988888888888888888855555555555557777----------------
8, 0xb2aaaaaaaaaaaaaa, potrosio broj.
8, 0x555555555555552b, uzeo broj.
MEM: 6666666669999------------------55555555555557777888-------------
5, 0xcaaaaaaaaaaaab55, potrosio broj.
5, 0xd2aaaaaaaaaaaa69, uzeo broj.
MEM: 6666666669999-------------------------------777788855555--------
5, 0xd2aaaaaaaaaaaa69, potrosio broj.
5, 0x55555555555556aa, uzeo broj.
MEM: 6666666669999-------------------------------7777888555555-------
5, 0x55555555555556aa, potrosio broj.
5, 0x52aaaaaaaaaaaaad, uzeo broj.
MEM: 6666666669999-------------------------------777788855555--------
5, 0x52aaaaaaaaaaaaad, potrosio broj.
5, 0x2aaaaaaaaaaaa995, uzeo broj.
MEM: 6666666669999-------------------------------777788855555--------
5, 0x2aaaaaaaaaaaa995, potrosio broj.
5, 0xd5555555555555a6, uzeo broj.
MEM: 666666666999955555555555555-----------------7777888-------------
8, 0x555555555555552b, potrosio broj.
8, 0x95555555555552aa, uzeo broj.
MEM: 666666666999955555555555555-----------------7777888888----------
6, 0x2aaaaaaaaaaaad95, potrosio broj.
6, 0xa555555555554a93, uzeo broj.
MEM: ---------99995555555555555566666666666------7777888888----------
9, 0x52aaaaaaaaaaaad4, potrosio broj.
9, 0xb2aaaaaaaaaaaaca, uzeo broj.
MEM: -------------5555555555555566666666666------77778888889999999999
7, 0x9555555555556cd4, potrosio broj.
7, 0xaaaaaaaaaaaaaa95, uzeo broj.
MEM: -------------5555555555555566666666666777777777-8888889999999999
9, 0xb2aaaaaaaaaaaaca, potrosio broj.
9, 0x95555555555556b2, uzeo broj.
8, 0x95555555555552aa, potrosio broj.
8, 0x92aaaaaaaaaaad55, uzeo broj.
MEM: 888888888----5555555555555566666666666777777777-----------------
6, 0xa555555555554a93, potrosio broj.
6, 0x3555555555555655, uzeo broj.
MEM: 888888888----55555555555555666666666--777777777-----------------
5, 0xd5555555555555a6, potrosio broj.
5, 0xa555555555555ab5, uzeo broj.
MEM: 8888888889999999999999999996666666665-777777777-----------------
MEM: 8888888889999999999999999996666666665-777777777-----------------
5, 0xa555555555555ab5, potrosio broj.
5, 0xaaaaaaaaaaaaad5b, uzeo broj.
7, 0xaaaaaaaaaaaaaa95, potrosio broj.
7, 0x3555555555555aab, uzeo broj.
MEM: 8888888889999999999999999996666666667777777777777777777---------
8, 0x92aaaaaaaaaaad55, potrosio broj.
8, 0xaaaaaaaaaaaaa9aa, uzeo broj.
9, 0x95555555555556b2, potrosio broj.
9, 0x95555555555556cd, uzeo broj.
MEM: ---------------------------666666666777777777777777777799999----
6, 0x3555555555555655, potrosio broj.
MEM: 88888888888888----------------------777777777777777777799999----
6, 0x12aaaaaaaaaab2d2, uzeo broj.
MEM: 88888888888888666666----------------777777777777777777799999----
9, 0x95555555555556cd, potrosio broj.
9, 0xa555555555556555, uzeo broj.
MEM: 88888888888888666666----------------77777777777777777779--------
6, 0x12aaaaaaaaaab2d2, potrosio broj.
9, 0xa555555555556555, potrosio broj.
MEM: 888888888888885555555555555555555---7777777777777777777---------
9, 0xb2aaaaaaaaaaa965, uzeo broj.
6, 0xaaaaaaaaaaaaad55, uzeo broj.
7, 0x3555555555555aab, potrosio broj.
7, 0x95555555555554da, uzeo broj.
MEM: 8888888888888855555555555555555557777779999999999999------------
MEM: 8888888888888855555555555555555557777779999999999999------------
8, 0xaaaaaaaaaaaaa9aa, potrosio broj.
8, 0xb2aaaaaaaaaaaaaa, uzeo broj.
7, 0x95555555555554da, potrosio broj.
7, 0x2555555555556a6a, uzeo broj.
MEM: 777777777777775555555555555555555------9999999999999------------
5, 0xaaaaaaaaaaaaad5b, potrosio broj.
5, 0x55555555555554d5, uzeo broj.
MEM: 7777777777777755555555555555555--------9999999999999------------
9, 0xb2aaaaaaaaaaa965, potrosio broj.
9, 0xb555555555555554, uzeo broj.
MEM: 7777777777777755555555555555555888888888888888888---------------
MEM: 7777777777777755555555555555555888888888888888888---------------
9, 0xb555555555555554, potrosio broj.
9, 0xcaaaaaaaaaaaa936, uzeo broj.
MEM: 77777777777777555555555555555558888888888888888889999999999-----
9, 0xcaaaaaaaaaaaa936, potrosio broj.
9, 0x12aaaaaaaaaaaaa6, uzeo broj.
MEM: 777777777777775555555555555555588888888888888888899999999999999-
5, 0x55555555555554d5, potrosio broj.
5, 0x6555555555556aad, uzeo broj.
MEM: 7777777777777755555------------88888888888888888899999999999999-
5, 0x6555555555556aad, potrosio broj.
5, 0x5555555555555556, uzeo broj.
MEM: 77777777777777555555-----------88888888888888888899999999999999-
5, 0x5555555555555556, potrosio broj.
5, 0x555555555555554a, uzeo broj.
MEM: 7777777777777755555555555555---88888888888888888899999999999999-
7, 0x2555555555556a6a, potrosio broj.
7, 0xd555555555555695, uzeo broj.
MEM: 7777777777777-55555555555555---88888888888888888899999999999999-
8, 0xb2aaaaaaaaaaaaaa, potrosio broj.
8, 0x52aaaaaaaaaaa535, uzeo broj.
MEM: 7777777777777-55555555555555888886666666666666---99999999999999-
MEM: 7777777777777-55555555555555888886666666666666---99999999999999-
8, 0x52aaaaaaaaaaa535, potrosio broj.
8, 0x555555555555555b, uzeo broj.
9, 0x12aaaaaaaaaaaaa6, potrosio broj.
9, 0x2555555555556b4a, uzeo broj.
MEM: 7777777777777-55555555555555-----6666666666666999999999999999999
7, 0xd555555555555695, potrosio broj.
7, 0x1555555555554d6a, uzeo broj.
MEM: 7777777777777755555555555555-----6666666666666999999999999999999
5, 0x555555555555554a, potrosio broj.
5, 0x3555555555555aca, uzeo broj.
MEM: 777777777777775555555555---------6666666666666999999999999999999
5, 0x3555555555555aca, potrosio broj.
5, 0x52aaaaaaaaaab595, uzeo broj.
MEM: 7777777777777755555555555555555--6666666666666999999999999999999
6, 0xaaaaaaaaaaaaad55, potrosio broj.
6, 0x3555555555556d2a, uzeo broj.
MEM: 777777777777775555555555555555566666666666666-999999999999999999
9, 0x2555555555556b4a, potrosio broj.
9, 0xd555555555554aca, uzeo broj.
MEM: 77777777777777555555555555555556666666666666699999999999999-----
5, 0x52aaaaaaaaaab595, potrosio broj.
5, 0x65555555555554a9, uzeo broj.
MEM: 77777777777777555555555--------6666666666666699999999999999-----
7, 0x1555555555554d6a, potrosio broj.
7, 0x12aaaaaaaaaaa595, uzeo broj.
6, 0x3555555555556d2a, potrosio broj.
6, 0xd2aaaaaaaaaab552, uzeo broj.
MEM: 88888888888---555555555666666666666666666----99999999999999-----
MEM: 88888888888---555555555666666666666666666----99999999999999-----
8, 0x555555555555555b, potrosio broj.
8, 0xd555555555554aab, uzeo broj.
MEM: --------------555555555666666666666666666888-99999999999999-----
9, 0xd555555555554aca, potrosio broj.
9, 0xa55555555555555a, uzeo broj.
MEM: 9999999999----555555555666666666666666666888--------------------
MEM: 9999999999----55555555566666666666666666688877777777777777777---
9, 0xa55555555555555a, potrosio broj.
9, 0xd2aaaaaaaaaaad55, uzeo broj.
MEM: 9999999999999-55555555566666666666666666688877777777777777777---
5, 0x65555555555554a9, potrosio broj.
5, 0x1555555555554d32, uzeo broj.
6, 0xd2aaaaaaaaaab552, potrosio broj.
MEM: 9999999999999555555555555555555----------88877777777777777777---
7, 0x12aaaaaaaaaaa595, potrosio broj.
8, 0xd555555555554aab, potrosio broj.
9, 0xd2aaaaaaaaaaad55, potrosio broj.
5, 0x1555555555554d32, potrosio broj.



$ ./main # B varijanta zadatka
8 dretvi: 5 generatora + 3 provjeravaca.

6, 0x1555555555554a95, uzeo broj.
5, 0x5555555555555b4d, uzeo broj.
7, 0x52aaaaaaaaaab52d, uzeo broj.
MEM: 66666666655555555555557777777777777-----------------------------
MEM: 66666666655555555555557777777777777-----------------------------
MEM: 66666666655555555555557777777777777-----------------------------
5, 0x5555555555555b4d, potrosio broj.
5, 0x555555555555696d, uzeo broj.
MEM: 666666666555555555----7777777777777-----------------------------
7, 0x52aaaaaaaaaab52d, potrosio broj.
7, 0x52aaaaaaaaaaaaa9, uzeo broj.
MEM: 6666666665555555557---------------------------------------------
6, 0x1555555555554a95, potrosio broj.
6, 0x2555555555555552, uzeo broj.
MEM: ---------555555555766666666666666-------------------------------
7, 0x52aaaaaaaaaaaaa9, potrosio broj.
7, 0x32aaaaaaaaaaad55, uzeo broj.
MEM: ---------555555555-666666666666667777777777777------------------
5, 0x555555555555696d, potrosio broj.
5, 0x9555555555556a54, uzeo broj.
MEM: -------------------6666666666666677777777777775555--------------
7, 0x32aaaaaaaaaaad55, potrosio broj.
7, 0x255555555555555a, uzeo broj.
MEM: -------------------6666666666666677-----------5555--------------
6, 0x2555555555555552, potrosio broj.
6, 0xaaaaaaaaaaaaa69a, uzeo broj.
MEM: ---------------------------------776666666666-5555--------------
6, 0xaaaaaaaaaaaaa69a, potrosio broj.
6, 0x555555555555556b, uzeo broj.
MEM: ---------------------------------776666666----5555--------------
7, 0x255555555555555a, potrosio broj.
7, 0x6555555555556aab, uzeo broj.
MEM: -----------------------------------6666666777-5555--------------
6, 0x555555555555556b, potrosio broj.
6, 0xb555555555556569, uzeo broj.
MEM: 66666666666666666-------------------------777-5555--------------
5, 0x9555555555556a54, potrosio broj.
5, 0x5555555555554aaa, uzeo broj.
MEM: 66666666666666666-------------------------77755555555555555-----
6, 0xb555555555556569, potrosio broj.
6, 0x2aaaaaaaaaaab24c, uzeo broj.
MEM: 6666666666666666--------------------------77755555555555555-----
7, 0x6555555555556aab, potrosio broj.
7, 0x5555555555555b65, uzeo broj.
MEM: 666666666666666677777777777777777------------55555555555555-----
6, 0x2aaaaaaaaaaab24c, potrosio broj.
6, 0xb5555555555554d5, uzeo broj.
MEM: 6666666666666---77777777777777777------------55555555555555-----
7, 0x5555555555555b65, potrosio broj.
7, 0x52aaaaaaaaaaad54, uzeo broj.
MEM: 6666666666666--------------------------------555555555555557777-
5, 0x5555555555554aaa, potrosio broj.
5, 0xaaaaaaaaaaaaaaac, uzeo broj.
MEM: 6666666666666555555555555----------------------------------7777-
6, 0xb5555555555554d5, potrosio broj.
6, 0x5555555555554aa9, uzeo broj.
MEM: 6666666666666555555555555----------------------------------7777-
5, 0xaaaaaaaaaaaaaaac, potrosio broj.
5, 0xa555555555555ad5, uzeo broj.
MEM: 66666666666665555555555555---------------------------------7777-
7, 0x52aaaaaaaaaaad54, potrosio broj.
7, 0x6aaaaaaaaaaaaaad, uzeo broj.
MEM: 66666666666665555555555555777777777-----------------------------
6, 0x5555555555554aa9, potrosio broj.
6, 0x555555555555554a, uzeo broj.
MEM: -------------555555555555577777777766666666666666---------------
5, 0xa555555555555ad5, potrosio broj.
5, 0xaaaaaaaaaaaaa92a, uzeo broj.
MEM: --------------------------77777777766666666666666555555---------
5, 0xaaaaaaaaaaaaa92a, potrosio broj.
5, 0x3555555555556ad3, uzeo broj.
MEM: --------------------------77777777766666666666666555555555555555
5, 0x3555555555556ad3, potrosio broj.
5, 0x3555555555556d5a, uzeo broj.
MEM: --------------------------7777777776666666666666655-------------
7, 0x6aaaaaaaaaaaaaad, potrosio broj.
7, 0x555555555555554b, uzeo broj.
MEM: 777777777777777--------------------6666666666666655-------------
7, 0x555555555555554b, potrosio broj.
7, 0xd555555555554aad, uzeo broj.
MEM: -----------------------------------666666666666665577777--------
7, 0xd555555555554aad, potrosio broj.
7, 0xd555555555554cab, uzeo broj.
MEM: 777777777777777--------------------6666666666666655-------------
7, 0xd555555555554cab, potrosio broj.
7, 0xd555555555555a92, uzeo broj.
MEM: 77777777777777---------------------6666666666666655-------------
6, 0x555555555555554a, potrosio broj.
6, 0x155555555555656a, uzeo broj.
MEM: 77777777777777666666666666666666-----------------55-------------
5, 0x3555555555556d5a, potrosio broj.
5, 0x5555555555556d92, uzeo broj.
MEM: 777777777777776666666666666666665555555555----------------------
5, 0x5555555555556d92, potrosio broj.
5, 0x15555555555552b4, uzeo broj.
MEM: 7777777777777766666666666666666655555555------------------------
6, 0x155555555555656a, potrosio broj.
6, 0xa5555555555555a6, uzeo broj.
MEM: 77777777777777666666------------55555555------------------------
5, 0x15555555555552b4, potrosio broj.
5, 0x4aaaaaaaaaaaaa4a, uzeo broj.
MEM: 77777777777777666666555555555555555555--------------------------
7, 0xd555555555555a92, potrosio broj.
7, 0x555555555555496b, uzeo broj.
MEM: --------------666666555555555555555555777777777777777-----------
7, 0x555555555555496b, potrosio broj.
7, 0x2555555555555ad6, uzeo broj.
MEM: 777777--------666666555555555555555555--------------------------
6, 0xa5555555555555a6, potrosio broj.
6, 0x92aaaaaaaaaaa6aa, uzeo broj.
MEM: 77777766------------555555555555555555--------------------------
7, 0x2555555555555ad6, potrosio broj.
7, 0x1555555555554d55, uzeo broj.
MEM: ------66------------5555555555555555557777777777777-------------
6, 0x92aaaaaaaaaaa6aa, potrosio broj.
6, 0xd2aaaaaaaaaaacb5, uzeo broj.
MEM: --------------------55555555555555555577777777777776666666666666
5, 0x4aaaaaaaaaaaaa4a, potrosio broj.
5, 0xa555555555555a94, uzeo broj.
MEM: 55555555------------------------------77777777777776666666666666
7, 0x1555555555554d55, potrosio broj.
6, 0xd2aaaaaaaaaaacb5, potrosio broj.
5, 0xa555555555555a94, potrosio broj.
