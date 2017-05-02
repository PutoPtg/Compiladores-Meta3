/*teste que chama um método e usa uma variável declarada
 globalmente e localmente com o mesmo nome*/

class thousandmiles {

	public static boolean global;

	public static int  main (String[] args){
		int inteiro;
		boolean resposta;
		boolean chamada;
		boolean global;
		global = true;
		resposta = carlton(inteiro, global);
		return 1;
	}

	public static boolean carlton (int inteiro, boolean booleano){
		global = booleano;
		return booleano;
	}

	public static boolean carlton (int inteiro, boolean booleano){
		global = booleano;
		return booleano;
	}
}
