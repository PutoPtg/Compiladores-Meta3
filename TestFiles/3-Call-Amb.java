/*teste que chama um método ambíguo e usa uma variável declarada globalmente*/


class blondie {

	public static int global;

	public static int  main (String[] args){
		boolean resposta;
		boolean chamada;
		resposta = callme(global, chamada); //primeiro que existe
		resposta = callme(global);			//segundo c/ mesmo nome e parâmetros diferentes
		resposta = callme(global, chamada, resposta); // terceiro que não existe
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
