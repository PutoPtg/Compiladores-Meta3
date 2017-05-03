/*teste que chama um método e usa uma variável declarada globalmente*/


class blondieMaria {

	public static boolean callme0 (){}
	public static int callme1 (){}
	public static double callme2 (){}
	public static void callme3 (){}
	public static boolean callme4 (){}
	public static int callme5 (){}
	public static double callme6 (){}
	public static void callme7 (){}
	public static boolean callme8 (){}
	public static int callme9 (){}



	public static int  main (String[] args){
		boolean resposta;
		boolean chamada;
		int	rint;
		double rouble;
		resposta = callme(3, chamada);
		resposta = callme0();
		rint = callme1();
		rouble = callme2();
		callme3();
		resposta = callme4();
		rint = callme5();
		rouble = callme6();
		callme7();
		resposta = callme8();
		rint = callme9();
		rouble = maria0();
		maria1();
		resposta = maria2();
		rint = maria3();
		rouble = maria4();
		maria5();
		resposta = maria6();
		rint = maria7();
		rouble = maria8();
		maria9();
		return 1;
	}

	public static double maria0 (){}
	public static void maria1 (){}
	public static boolean maria2 (){}
	public static int maria3 (){}
	public static double maria4 (){}
	public static void maria5 (){}
	public static boolean maria6 (){}
	public static int maria7 (){}
	public static double maria8 (){}
	public static void maria9 (){}

	public static boolean callme (int inteiro, boolean booleano){
		global = inteiro;
		return booleano;
	}
}
