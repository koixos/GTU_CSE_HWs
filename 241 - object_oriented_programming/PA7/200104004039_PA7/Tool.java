import java.util.ArrayList;
import java.util.List;

public abstract class Tool {
    ObjectBase currentObject() {
        if (this.list.isEmpty()) {
            System.out.println("There is not such an item!");
            return new ObjectBase("NULL", "NULL", "NULL");
        }
        return this.list.get(indOfCurrent);
    }

    void add(ObjectBase obj) { this.getList().add(obj); }

    void remove(ObjectBase obj) {
        if (this.list.isEmpty()) {
            System.out.println("The list is already empty!");
            return;
        }
        if (!this.list.remove(obj))
            System.out.println("There is not such an item found in this list!");
    }

    void showList() {
        if (this.list.isEmpty()) {
            System.out.println("No item found!");
            return;
        }
        for (ObjectBase obj : list) {
            obj.info();
            System.out.println(" . ");
        }
    }

    void next(String type) {
        int tempInd = indOfCurrent;
        for (int i = indOfCurrent+1; i < list.size(); i++) {
            if (list.get(i).getType().compareTo(type) == 0) {
                indOfCurrent = i;
                break;
            }
        }
        if (tempInd == indOfCurrent)
            System.out.println("There is not any other " + type + " item in the list!");
    }

    void previous(String type) {
        int tempInd = indOfCurrent;
        for (int i = indOfCurrent-1; i >= 0; i--) {
            if (list.get(i).getType().compareTo(type) == 0) {
                indOfCurrent = i;
                break;
            }
        }
        if (tempInd == indOfCurrent)
            System.out.println("There is not any other " + type + " item in the list!");
    }

    abstract String getType();

    public List<ObjectBase> getList() { return list; }

    public void setList(List<ObjectBase> list) {
        for (ObjectBase obj : list)
            if (obj.getType().compareTo("image") == 0 || obj.getType().compareTo("text") == 0)
                this.list.add(obj);
    }

    public int getIndOfCurrent() { return indOfCurrent; }

    public void setIndOfCurrent(int indOfCurrent) { this.indOfCurrent = indOfCurrent; }

    private List<ObjectBase> list = new ArrayList<>();
    private int indOfCurrent = 0;
}
