int page = 0;

void choose_page();
void PAGE1();
void PAGE2();
void PAGE3();
void PAGE4();

void choose_page(){
	if (digitalRead(A1) == 1){
		page = 1;
		return;
	}
	if (digitalRead(A2) == 1){
		page = 2;
		return;
	}
	if (digitalRead(A3) == 1){
		page = 3;
		return;
	}
	if (digitalRead(A4) == 1){
		page = 4;
		return;
	}
}

void PAGE1(){
	
}

void PAGE2(){
	
}

void PAGE3(){
	
}

void PAGE4(){
	
}

void setup(){
	for (int i = 0; i <= 5; i++){
		pinMode(i, INPUT);
	}
	for (int i = 6; i <= 8; i++){
		pinMode(i, OUTPUT);
	}
	for (int i = 9; i <= 12; i++){
		pinMode(i, OUTPUT);
	}
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
}

void loop(){
	choose_page();
	
	switch(page){
		case 1:
			PAGE1();
			break;
		case 2:
			PAGE2();
			break;
		case 3:
			PAGE3();
			break;
		case 4:
			PAGE4();
			break;
	}
}