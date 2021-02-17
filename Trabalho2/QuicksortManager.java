import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;

// Gerenciador do pool de threads
public class QuicksortManager extends Thread {
    private final List<QuicksortThread> threads;
    private final int nThreads;
    private final QuicksortCallback callback;
    private final Quicksort quicksort;
    private final BlockingQueue<SortItem> queue;
    
    public QuicksortManager(int nThreads, Quicksort quicksort, BlockingQueue<SortItem> queue, QuicksortCallback callback) {
        this.threads = new ArrayList<>();
        this.nThreads = nThreads;
        this.quicksort = quicksort;
        this.queue = queue;
        this.callback = callback;
    }

    public void addThread(QuicksortThread thread) {
        threads.add(thread);
    }

    public void sort(List<Integer> list) throws Exception {
        SortItem firstItem = new SortItem(list, 0, list.size() - 1);
        queue.add(firstItem);

        int TamAnterior = 0;
        while (queue.size() < nThreads) {
            if (TamAnterior >= queue.size()) {
                // Too many threads for this list
                break;
            } else {
                TamAnterior = queue.size();
            }

            SortItem item = queue.take();
            quicksort.sort(list, item.getInicio(), item.getFim(), callback);
        }
        
        // Run threads
        int workingThreads = 0;
        for (QuicksortThread thread : threads) {
            if (queue.isEmpty()) {
                break;
            }

            workingThreads++;
            thread.setSortItem(queue.take());
            thread.start();
        }
        for (QuicksortThread thread : threads) {
            if (workingThreads == 0) {
                break;
            }

            thread.join();
            workingThreads--;
        }
    }
}
