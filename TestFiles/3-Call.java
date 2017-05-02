/*teste que chama um método e usa uma variável declarada globalmente*/


class blondie {

	public static int global;

	public static int  main (String[] args){
		boolean resposta;
		boolean chamada;
		resposta = callme(global, chamada);
		return 1;
	}

	public static boolean callme (int inteiro, boolean booleano){
		global = inteiro;
		return booleano;
	}

	public static boolean callme (int inteiro){
		global = inteiro;
		return booleano;
	}
}
