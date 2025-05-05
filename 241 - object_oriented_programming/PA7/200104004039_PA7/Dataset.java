import java.util.ArrayList;
import java.util.List;

public class Dataset {
    public void register(Tool tool) { this.tools.add(tool); }

    public void unregister(Tool tool) {
        if (!this.tools.remove(tool))
            System.out.println("There is not such an observer in the database!");
    }

    public void add(ObjectBase obj) {
        this.mediaObjects.add(obj); // adding the object to database
        for (int i = 0; i < obj.getClass().getInterfaces().length; i++) { // adding the object one of the tools according to its type
            if (obj.getClass().getInterfaces()[i].getTypeName().equals("NonPlayable")) {
                for (Tool tool : tools)
                    if (tool.getType().equals("Viewer"))
                        tool.add(obj);
                return;
            } else if (obj.getClass().getInterfaces()[i].getTypeName().equals("Playable")) {
                for (Tool tool : tools)
                    if (tool.getType().equals("Player"))
                        tool.add(obj);
                return;
            }
        }
    }

    public void remove(ObjectBase obj) {
        this.mediaObjects.remove(obj); // removing the object from database
        for (int i = 0; i < obj.getClass().getInterfaces().length; i++) { // removing the object from its tool according to its type
            if (obj.getClass().getInterfaces()[i].getTypeName().equals("NonPlayable")) {
                for (Tool tool : tools)
                    if (tool.getType().equals("Viewer"))
                        tool.remove(obj);
                return;
            } else if (obj.getClass().getInterfaces()[i].getTypeName().equals("Playable")) {
                for (Tool tool : tools)
                    if (tool.getType().equals("Player"))
                        tool.remove(obj);
                return;
            }
        }
    }

    public List<ObjectBase> getMediaObjects() { return mediaObjects; }

    public void setMediaObjects(List<ObjectBase> mediaObjects) { this.mediaObjects = mediaObjects; }

    public List<Tool> getTools() { return tools; }

    public void setTools(List<Tool> tools) { this.tools = tools; }

    private List<ObjectBase> mediaObjects = new ArrayList<>(); // notifications
    private List<Tool> tools = new ArrayList<>(); // observers
}
