/*teste que chama um método e usa uma variável declarada globalmente*/


class blondie {

	public static int global;
	public static int callme;

	public static boolean callme (double inteiro, boolean booleano){
		global = inteiro;
		return booleano;
	}

	public static boolean callme (double inteiro, boolean booleano){
		global = inteiro;
		return booleano;
	}



	public static int  main (String[] args){
		boolean resposta;
		boolean chamada;
		resposta = callme(1, chamada);
		chamada = callme();
		return 1;
	}

	

	public static boolean callme (){
		global = inteiro;
		global = b0();
		global = b0(a,b);
		global = b0(a,b,c);
		global = b0(a,b,1,d);
		b0();
		global = b + b0(a,b,1,d);

		return booleano;
	}




	public static int b0(){}

	public static int b0(int a, int b){}

	public static int b0(int a, int b, int c, int d){}


}
