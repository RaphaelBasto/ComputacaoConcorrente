import java.util.Scanner;

// Monitor
class UsuarioImpressao {
    //numero maximo de threads esperando para acessar o processador
    static final int N = 5;
    //atributos de estado da execucao
    private int esperando; //qtde de threads esperando
    private int ocupado; //estado do processador (0: desocupado; 1: ocupado)
    public Buffer buffer;

    // Construtor
    UsuarioImpressao() {
        this.esperando = 0; //(no maximo N=50)
        this.ocupado = 0;
        this.buffer = new Buffer(N);
    }

    // Metodo das threads
    public synchronized void EntraConsumo() {
        while (this.buffer.isEmpty() || ocupado ==1){
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        try {
            while (this.buffer.getString(0)==null) this.wait();
            this.ocupado = 1;
        } catch (InterruptedException e) {}
    }

    // Metodo do processador para sinalizar que terminou a execução de uma thread
    public synchronized void SaiConsumo() {
        this.esperando--;
        this.ocupado = 0;
        this.notifyAll();
    }

    // Entrada para Produtores
    public synchronized String EntraProducao(int id) {
        while (ocupado!=0){
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        while (this.buffer.isFull()){
            System.out.println("Fila de impressão cheia");
            try {
                this.notifyAll();

                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Scanner scanner = new Scanner(System.in);
        System.out.println("Envie a sua requisição ou digite 'FDI' para ver a fila de impressão:");

        return scanner.nextLine();

    }
    public synchronized void SaiProducao() {
        this.esperando++; //registra que o Produtor saiu
        notify(); //libera Consumidores e Produtores (caso existam Consumidores ou Produtores bloqueados)
    }
}

class Buffer{
    private final String[] buffer;

    public Buffer(int N) {
        this.buffer = new String[N];
    }

    public String getString(int i) {
        return buffer[0];
    }

    public synchronized void print(){
        System.out.println("Imprimindo...\n" + buffer[0]);
    }

    public synchronized void printAll(){
        System.out.println("Fila de impressão:");
        for (String s : buffer){
            System.out.print(s + " ");
        }
        System.out.println();
    }

    public synchronized void remove(){
        buffer[0] = null;
        for (int j = 0; j< buffer.length-1; j++){
            if (buffer[j] == null){
                buffer[j] = buffer[j+1];
                buffer[j+1] = null;
            }
        }
    }

    public synchronized void insere(String s){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] == null){
                buffer[i] = s;
                break;
            }
        }
    }

    public synchronized boolean isFull(){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] == null){
                return false;
            }
        }
        return true;
    }

    public synchronized boolean isEmpty(){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] != null){
                return false;
            }
        }
        return true;
    }
}


// Consumidor
class Consumidor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    UsuarioImpressao monitor;//objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Consumidor(int id, int delayTime, UsuarioImpressao m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraConsumo();

                this.monitor.buffer.print();

                this.monitor.buffer.remove();
                this.monitor.SaiConsumo();
                sleep(this.delay);

            }
        } catch (InterruptedException e) {
        }
    }
}

//--------------------------------------------------------
// Produtor
class Produtor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    UsuarioImpressao monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Produtor(int id,int delayTime, UsuarioImpressao m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (; ; ) {
                String requisicao = this.monitor.EntraProducao(this.id);

                if (requisicao.equals("FDI")){
                    this.monitor.buffer.printAll();
                }else this.monitor.buffer.insere(requisicao);
                this.monitor.SaiProducao();
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

//--------------------------------------------------------
// Classe principal
class ProduzConsome {
    static final int C = 3;
    static final int P = 1;

    public static void main (String[] args) {
        int i;
        UsuarioImpressao monitor = new UsuarioImpressao();            // Monitor
        Consumidor[] c = new Consumidor[C];       // Threads Consumidoras
        Produtor[] p = new Produtor[P];   // Threads Produtoras

        for (i=0; i< C; i++) {
            c[i] = new Consumidor(i+1, (i+1)*1000, monitor);
            c[i].start();
        }
        for (i=0; i< P; i++) {
            p[i] = new Produtor(i+1, 0, monitor);
            p[i].start();
        }
    }
}
