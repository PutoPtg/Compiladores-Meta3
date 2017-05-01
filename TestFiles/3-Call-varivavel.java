/*teste que chama um método e usa uma variável igual de tipos diferentes*/


class blondie2 {

	public static int  main (String[] args){
		boolean resposta; //aqui a resposta é um booleano
		boolean chamada;
		int inteiro;
		resposta = chamada;
		inteiro = callme(global, chamada);
		return 1;
	}

	public static int callme (int inteiro, boolean booleano){
		int resposta; //aqui a resposta é um inteiro
		return resposta;
	}
}

