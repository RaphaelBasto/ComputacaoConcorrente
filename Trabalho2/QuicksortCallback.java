
import java.util.List;
import java.util.concurrent.BlockingQueue;

class QuicksortCallback {
    private final BlockingQueue<SortItem> queue;

    public QuicksortCallback(BlockingQueue<SortItem> queue) {
        this.queue = queue;
    }

    public void call(List<Integer> list, int inicio, int fim, int partition) {
        queue.add(new SortItem(list, inicio, partition));
        queue.add(new SortItem(list, partition + 1, fim));
    }
}
