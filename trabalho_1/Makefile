
all: pipara piseq scholespara scholesseq

scholesseq_tests: scholesseq
	time ./scholesseq < entrada_blackscholes
	time ./scholesseq < entrada_blackscholes
	time ./scholesseq < entrada_blackscholes
	time ./scholesseq < entrada_blackscholes
	time ./scholesseq < entrada_blackscholes

scholespara_tests: scholespara
	time ./scholespara < entrada_blackscholes
	time ./scholespara < entrada_blackscholes
	time ./scholespara < entrada_blackscholes
	time ./scholespara < entrada_blackscholes
	time ./scholespara < entrada_blackscholes

pipara_tests: pipara
	time ./pipara
	time ./pipara
	time ./pipara
	time ./pipara
	time ./pipara

piseq_tests: piseq	
	time ./piseq
	time ./piseq
	time ./piseq
	time ./piseq
	time ./piseq

piseq: piseq.c
	gcc piseq.c -o piseq

pipara: pipara.c
	gcc pipara.c -o pipara -lpthread

scholesseq: scholesseq.c
	gcc scholesseq.c -o scholesseq -lm

scholespara: scholespara.c
	gcc scholespara.c -o scholespara -lpthread -lm

clean:
	rm piseq
	rm pipara
	rm scholesseq
	rm scholespara
