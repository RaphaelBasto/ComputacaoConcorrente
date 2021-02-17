
import java.util.List;

//Classe com atributos necessários para um quicksort: lista, inicio e fim da lista

final public class SortItem {
    private final int inicio;
    private final int fim;
    private final List<Integer> list;

    public SortItem(List<Integer> list, int inicio, int fim) {
        this.list = list;
        this.inicio = inicio;
        this.fim = fim;
    }

    public List<Integer> getList() {
        return this.list;
    }

    public int getInicio() {
        return this.inicio;
    }

    public int getFim() {
        return this.fim;
    }

}
