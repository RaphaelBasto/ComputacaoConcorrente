
import java.util.List;

public class Quicksort {
    public void sort(List<Integer> list) {
        sort(list, 0, list.size() - 1);
    }

    public void sort(List<Integer> list, int inicio, int fim) {
        if (inicio < fim) {
            int partition = partition(list, inicio, fim);
            sort(list, inicio, partition);
            sort(list, partition + 1, fim);
        }
    }


    public void sort(List<Integer> list, int inicio, int fim, QuicksortCallback callback) {
        if (inicio < fim) {
            int partition = partition(list, inicio, fim);
            callback.call(list, inicio, fim, partition);
        }
    }

    private int partition(List<Integer> list, int inicio, int fim) {
        int pivotId = encontraPivot(inicio, fim);
        int i = inicio - 1;
        int j = fim + 1;
        int pivot = list.get(pivotId);

        while (true) {
            i += 1;
            while (i < list.size() && list.get(i) < pivot) {
                i += 1;
            }
            do {
                j -= 1;
            } while (j >= 0 && list.get(j) > pivot);

            if (i >= j) {
                break;
            } else {
                swap(list, i, j);
            }
        }

        return j;
    }

    private void swap(List<Integer> list, int i, int j) {
        Integer e = list.get(i);
        list.set(i, list.get(j));
        list.set(j, e);
    }

    private int encontraPivot(int inicio, int fim) {
        return (fim - inicio) / 2 + inicio;
    }
}
