int in();
int out(int v);

int fib(int k){
	if (k < 2) { return 1; }
	return fib(k - 2) + fib(k - 1);
}

int main(){
	/*
	int v;
	int w;
	v = in();
	w = fib(v);
	out(w);
	*/
	return 0;
}