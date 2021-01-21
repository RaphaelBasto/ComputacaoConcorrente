class SomaVetor {
    static final int N = 2;

    public static void main(String[] args){
        Thread[] threads = new Thread[N];

        int[] valoresDeA = new int[]{14, 66, 50, 80, 53, 50, 22, 32, 86, 72};
        int[] valoresDeB = new int[]{67, 68, 32, 28, 44, 62, 33, 3, 95, 41};
        Vetor vetA = new Vetor(10);
        vetA.inicializaVetor(valoresDeA);
        //vetA.inicializaVetorAleatorio();
        vetA.printaVetor('A');

        Vetor vetB = new Vetor(10);
        vetB.inicializaVetor(valoresDeB);
        //vetB.inicializaVetorAleatorio();
        vetB.printaVetor('B');

        Vetor vetC = new Vetor(10);
        vetC.printaVetor('C');

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new T(i,vetC,vetB,vetA,N);
        }
        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }
        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
        vetC.printaVetor('C');
    }
}
class T extends Thread {
    //identificador da thread
    private int id;
    private int nthreads;
    //objeto compartilhado com outras threads
    Vetor vetA;
    Vetor vetB;
    Vetor vetC;

    //construtor
    public T(int tid, Vetor vetC, Vetor vetB, Vetor vetA, int nthreads) {
        this.id = tid;
        this.vetC = vetC;
        this.vetA = vetA;
        this.vetB = vetB;
        this.nthreads = nthreads;
    }

    //metodo main da thread
    public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        for (int i=id; i<vetC.getTam(); i+=nthreads) {
            this.vetC.somaVetores(this.vetA.getVet()[i], this.vetB.getVet()[i], i);
        }
        System.out.println("Thread " + this.id + " terminou!");
    }
}
class Vetor{
    public int[] vet;
    public int tam;

    public Vetor(int tam){
        this.vet = new int[tam];
        this.tam = tam;
    }

    public void inicializaVetorAleatorio(){
        for (int i = 0; i<this.tam; i++){
            vet[i] = (int) (Math.random()* 100);
        }
    }

    public void inicializaVetor(int[] vetor){
        for (int i = 0; i<this.tam; i++){
            this.vet[i] = vetor[i];
        }
    }

    public void printaVetor(char ch){
        System.out.print("O vetor " + ch + " é: \n[");
        for (int i = 0; i<this.tam; i++){
            if (i != tam-1){
                System.out.print(vet[i] + ", ");
            }
            else System.out.println(vet[i] + "]");
        }
    }

    public int getTam() {
        return tam;
    }

    public int[] getVet() {
        return vet;
    }

    public void setVet(int pos, int value) {
        this.vet[pos] = value;
    }

    public synchronized void somaVetores(int A, int B, int i ){
        this.vet[i] = A+B;
    }
}