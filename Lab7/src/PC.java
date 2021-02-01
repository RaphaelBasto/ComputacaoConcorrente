class Buffer{
    private final String[] buffer;

    public Buffer() {
        this.buffer = new String[10];
    }

    public synchronized void print(){
        System.out.println("Esse é o buffer agora:");
        for (String s : buffer) {
            System.out.print(s + " ");
        }
        System.out.print("\n");
    }

    public void remove(){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] != null){
                buffer[i] = null;
                break;
            }
        }
    }

    public void insere(String s){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] == null){
                buffer[i] = s;
                break;
            }
        }
    }

    public boolean isFull(){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] == null){
                return false;
            }
        }
        return true;
    }

    public boolean isEmpty(){
        for (int i = 0; i < buffer.length; i++){
            if (buffer[i] != null){
                return false;
            }
        }
        return true;
    }
}


class PC {
    private int consome, produz;
    private final Buffer buffer;

    // Construtor
    PC() {
        this.consome = 0;
        this.produz = 0;
        this.buffer = new Buffer();
    }

    public Buffer getBuffer() {
        return buffer;
    }

    // Entrada para Consumidores
    public synchronized void EntraConsumo(int id) {
        try {
            while (this.produz > 0 || buffer.isEmpty()) {
                System.out.println ("le.leitorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.consome++;  //registra que ha mais um Consumidor
            System.out.println ("le.leitorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para Consumidores
    public synchronized void SaiConsumo(int id) {
        this.consome--; //registra que um Consumidor saiu
        if (this.consome == 0)
            this.notify(); //libera Produtor (caso exista Produtor bloqueado)
        System.out.println ("le.leitorSaindo("+id+")");
    }

    // Entrada para Produtores
    public synchronized void EntraProducao(int id) {
        try {
            while ((this.consome > 0) || (this.produz > 0) || buffer.isFull()) {
                System.out.println ("le.escritorBloqueado("+id+")");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.produz++; //registra que ha um Produtor
            System.out.println ("le.escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para Produtores
    public synchronized void SaiProducao(int id) {
        this.produz--; //registra que o Produtor saiu
        notifyAll(); //libera Consumidores e Produtores (caso existam Consumidores ou Produtores bloqueados)
        System.out.println ("le.escritorSaindo("+id+")");
    }
}

// Consumidor
class Consumidor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    PC monitor;//objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Consumidor(int id, int delayTime, PC m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraConsumo(this.id);

                this.monitor.getBuffer().remove();

                System.out.println("Thread " + id + " removeu um elemento!");

                this.monitor.getBuffer().print();

                this.monitor.SaiConsumo(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Produtor
class Produtor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    PC monitor; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Produtor(int id, int delayTime, PC m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        int n = 0;
        String s = "T " + id + " N " + n;
        try {
            for (;;) {
                this.monitor.EntraProducao(this.id);

                this.monitor.getBuffer().insere(s);

                System.out.println("Thread " + id + " inseriu " + s);

                n++;

                this.monitor.getBuffer().print();

                this.monitor.SaiProducao(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Classe principal
class ProduzConsome {
    static final int C = 1;
    static final int P = 3;

    public static void main (String[] args) {
        int i;
        PC monitor = new PC();            // Monitor
        Consumidor[] c = new Consumidor[C];       // Threads Consumidoras
        Produtor[] p = new Produtor[P];   // Threads Produtoras

        for (i=0; i< C; i++) {
            c[i] = new Consumidor(i+1, (i+1)*500, monitor);
            c[i].start();
        }
        for (i=0; i< P; i++) {
            p[i] = new Produtor(i+1, (i+1)*500, monitor);
            p[i].start();
        }
    }
}
