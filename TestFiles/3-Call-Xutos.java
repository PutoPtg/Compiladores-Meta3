class xutos {

	public static int  main (String[] args){
		int contentores, nAmerica, perfeitoVazio, naoSouUnico;
		boolean mundoAoContrario;
		minhaCasinha(contentores, nAmerica); 								//existe
		vidaMalvada();														//não existe
		vidaMalvada(contentores, nAmerica, perfeitoVazio, naoSouUnico);		//chamado com ints mas são doubles e aceita
		vidaMalvada(mundoAoContrario);										//ambíguo multiplos
		vidaMalvada(mundoAoContrario, contentores);							//chamado com o tipo errado
		chuvaDissolvente();													//ambíguo, multiplo
		vidaMalvada(vidaMalvada);											//ambíguo, 2 aceitam
		double vidaMalvada;
		vidaMalvada(vidaMalvada);											//existe
		return vidaMalvada();												//não existe
	}

	public static int minhaCasinha(int mesa, int elevador){
		return mesa;
	}

	public static int vidaMalvada(int a){}
	public static int vidaMalvada(boolean a){}
	public static int vidaMalvada(boolean a){}
	public static int vidaMalvada(boolean a){}
	public static int vidaMalvada(boolean a){}
	public static int vidaMalvada(double a){}
	public static int vidaMalvada(int a, int b){}
	public static int vidaMalvada(int a, int b, int c){}
	public static int vidaMalvada(double a, double b, double c, int d){}
	public static int chuvaDissolvente(){}
	public static int chuvaDissolvente(){}

	public static int vidaMalvada;
}
