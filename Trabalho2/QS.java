
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class QS {
    private static final int N_RUNS = 5;
    
    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);
        int[] listSizes;

        System.out.println("Digite 0 para executar o programa com 12 tamanhos de lista diferentes (até 50M)\nou\ndigite 1 para executar o programa com 1 tamanho de lista à sua escolha e printar essa lista ordenada: ");

        int inputDoUsuario1 = scanner.nextInt();

        // Cria lista com diferentes tamanhos para facilitar o relatório ou cria um tamanho de lista para ver a execução
        if (inputDoUsuario1==1){
            System.out.println("Agora digite o tamanho de lista desejado (Recomenda-se um tamanho pequeno):");
            int inputDoUsuario2 = scanner.nextInt();
            listSizes = new int[] {inputDoUsuario2};
        }else listSizes = new int[]{100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000};

        try {
            if (inputDoUsuario1==0){
                System.out.println("\nExecutando:\n");
                System.out.println("Tam    | Básico | Concorrente");
                for (int listSize : listSizes) {
                    List<Integer> list = geraLista(listSize);
                    double basicTime = runQuickSort(list, inputDoUsuario1);
                    double threadsTime = runConcurrentQuickSort(list, inputDoUsuario1);
                    System.out.println(listSize + " \t  " + basicTime + " \t\t  " + threadsTime);
                }
            }
            if(inputDoUsuario1==1) {
                System.out.println("Se deseja criar uma lista de sua preferência, digite 0. Se deseja que ela seja criada aleatoriamente, digite 1:");
                int inputDoUsuario2 = scanner.nextInt();
                if (inputDoUsuario2==0){
                    for (int listSize : listSizes) {
                        List<Integer> list = geraListaNaoAleatoria(listSize);
                        System.out.println("\nExecutando:\n");
                        double basicTime = runQuickSort(list, inputDoUsuario1);
                        double threadsTime = runConcurrentQuickSort(list, inputDoUsuario1);
                        System.out.println("Tam    | Básico | Concorrente");
                        System.out.println(listSize + " \t  " + basicTime + " \t\t  " + threadsTime);
                    }
                }
                if (inputDoUsuario2==1){
                    System.out.println("\nExecutando:\n");
                    for (int listSize : listSizes) {
                        List<Integer> list = geraLista(listSize);
                        double basicTime = runQuickSort(list, inputDoUsuario1);
                        double threadsTime = runConcurrentQuickSort(list, inputDoUsuario1);
                        System.out.println("Tam    | Básico | Concorrente");
                        System.out.println(listSize + " \t  " + basicTime + " \t\t  " + threadsTime);
                    }
                }
            }
        } catch (Exception e) {
            StringWriter sw = new StringWriter();
            e.printStackTrace(new PrintWriter(sw));

            System.out.println("- ERROR -");
            System.out.println(e.getMessage());
            System.out.println(sw.toString());
        }

        System.exit(0);
    }

    private static List<Integer> geraLista(int size) {
        ArrayList<Integer> list = new ArrayList<>(size);

        Random rand = new Random();
        
        for (int i = 0; i < size; i++) {
            Integer r = rand.nextInt(size+1) ;
            list.add(r);
        }

        return list;
    }

    private static List<Integer> geraListaNaoAleatoria(int size) {
        ArrayList<Integer> list = new ArrayList<>(size);
        Scanner scanner = new Scanner(System.in);

        for (int i = 0; i < size; i++) {
            if (i==0) System.out.println("Digite o primeiro elemento da lista:");
            else if (i==size-1)  System.out.println("Digite o ultimo elemento da lista:");
            else System.out.println("Digite o proximo elemento da lista:");
            int inputDoUsuario2 = scanner.nextInt();
            list.add(inputDoUsuario2);
        }

        return list;
    }

    private static double runQuickSort(List<Integer> originalList, int inputDoUsuario1) {
        double startTime, stopTime;
        double totalTime = 0;
        Quicksort quicksort = new Quicksort();

        // Para facilitar o cálculo, o programa roda N_RUNS vezes para calcular a média.
        for (int run = 0; run < N_RUNS; run++) {
            List<Integer> list = new ArrayList<>(originalList);
            startTime = System.currentTimeMillis();

            quicksort.sort(list);
            
            stopTime = System.currentTimeMillis();
            totalTime += stopTime - startTime;
            if (inputDoUsuario1==1){
                for (Integer i : list){
                    System.out.print(i + " ");
                }
                System.out.println();
            }
        }



        return totalTime / N_RUNS;
    }

    private static double runConcurrentQuickSort(List<Integer> originalList, int inputDoUsuario1) throws Exception {
        double startTime, stopTime;
        double totalTime = 0;
        Quicksort quicksort = new Quicksort();

        // Exception encontrada na internet

        Thread.UncaughtExceptionHandler h = (th, e) -> {
            StringWriter sw = new StringWriter();
            e.printStackTrace(new PrintWriter(sw));
            String exceptionStackTrace = sw.toString();

            System.out.println("- ERRO NA THREAD -");
            System.out.println(e.getMessage());
            System.out.println(exceptionStackTrace);
            System.exit(1);
        };

        // Para facilitar o cálculo, o programa roda N_RUNS vezes para calcular a média.
        for (int run = 0; run < N_RUNS; run++) {
            List<Integer> list = new ArrayList<>(originalList);

            // Cria o gerenciador de threads
            int n_THREADS = 16;
            BlockingQueue<SortItem> queue = new ArrayBlockingQueue<>(n_THREADS);
            QuicksortCallback callback = new QuicksortCallback(queue);
            QuicksortManager manager = new QuicksortManager(n_THREADS, quicksort, queue, callback);

            // Cria as threads e as adiciona ao gerenciador
            for (int i = 0; i < n_THREADS; i++) {
                QuicksortThread thread = new QuicksortThread(quicksort);
                thread.setUncaughtExceptionHandler(h);
                manager.addThread(thread);
            }
            
            startTime = System.currentTimeMillis();
            
            manager.sort(list);
            
            stopTime = System.currentTimeMillis();
            totalTime += stopTime - startTime;
            if (inputDoUsuario1==1){
                for (Integer i : list){
                    System.out.print(i + " ");
                }
                System.out.println();
            }

        }
        
        return totalTime / N_RUNS;
    }
}
