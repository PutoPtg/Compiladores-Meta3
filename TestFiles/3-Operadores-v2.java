/*teste de operadores - 2*/


class somamisso {

	public static int global1;
	public static double global2;

	public static int  main (String[] args){
		double resultado;
		resultado = 0;
		resultado = (((1 + 2 + 3) / 3.1)*5)%10;
		global1 = 10;
		global2 = 1000000.0000000;
		resultado = -global2/(!global1+resultado);
		return resultado;
	}

}
