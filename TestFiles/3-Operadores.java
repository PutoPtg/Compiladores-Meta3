/*teste de operadores*/


class somamisso {

	public static int global1;
	public static double global2;

	public static int  main (String[] args){
		double resultado;
		resultado = 0;
		resultado = ((1 + 2 + 3) / 3)*5;
		global1 = 10;
		global2 = 10000000000000;
		resultado = global2/(global1+resultado);
		return resultado;
	}

}
