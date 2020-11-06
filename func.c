
int delta(int x, int y){
	int cnt = 0;
	int cx = 1;
	
	while(cnt <= 31){
		if(cx * x <= cx){
            		return -1;
        	}
		int tx = cx;
		while(tx != 0){
			if(tx % 10 == y){
				return cnt;
			} 
			tx /= 10;
		}
		cx *= x;
		cnt++;
	}
	return -1;
}
