
public class QuicksortThread extends Thread {
    private final Quicksort quicksort;
    private SortItem item;

    public QuicksortThread(Quicksort quicksort) {
        this.quicksort = quicksort;
    }

    public void setSortItem(SortItem item) {
        this.item = item;
    }

    public void run() {
        try {
            quicksort.sort(item.getList(), item.getInicio(), item.getFim());
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}
