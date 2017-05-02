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
		return booleano;
	}

	public static boolean callme (){
		global = inteiro;
		return booleano;
	}

}
