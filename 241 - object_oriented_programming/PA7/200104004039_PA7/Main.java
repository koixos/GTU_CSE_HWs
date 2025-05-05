import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        Dataset ds = new Dataset(); // This is our dataset

        MediaPlayer mp1 = new MediaPlayer(); // observers
        MediaPlayer mp2 = new MediaPlayer();

        Viewer v1 = new Viewer(); // observers
        Viewer v2 = new Viewer();

        ds.register(mp1); // registering observers to dataset
        ds.register(mp2);
        ds.register(v1);
        ds.register(v2);

        ds.add(new Image("imagename1", "other info1", 5, 5)); // creating notifications
        ds.add(new Image("imagename2", "other info2", 6, 6));
        ds.add(new Image("imagename3", "other info3", 7, 7));
        ds.add(new Image("imagename4", "other info4", 8, 8));
        ds.add(new Image("imagename5", "other info5", 9, 9));

        ds.add(new Audio("audioname1", "other info1", 15));
        ds.add(new Audio("audioname2", "other info2", 30));
        ds.add(new Audio("audioname3", "other info3", 45));

        ds.add(new Video("videoname1", "other info1", 5));
        ds.add(new Video("videoname2", "other info2", 10));
        ds.add(new Video("videoname3", "other info3", 15));

        ds.add(new Text("textname1", "other info1"));
        ds.add(new Text("textname2", "other info2"));
        ds.add(new Text("textname3", "other info3"));

        // MEDIA PLAYER
        testClass(mp1, mp2, ds);
        System.out.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        // VIEWER
        testClass(v1, v2, ds);

        return;
    }

    public static void testClass(Tool t1, Tool t2, Dataset ds) {
        System.out.println("List of " + t1.getType() +"-1\n-----------------------");
        t1.showList();
        System.out.println("-----------------------\nList of " + t2.getType() + "-2\n-----------------------");
        t2.showList();

        System.out.println("-----------------------\nInfo of current object in " + t1.getType() +"-1\n-----------------------");
        t1.currentObject().info();

        ObjectBase tempObj = t1.currentObject();
        System.out.println("-----------------------\nCurrent object of " + t1.getType() + "-1 is\n\tremoved from database...");
        ds.remove(t1.currentObject());
        System.out.println("-----------------------\nUpdated database of " + t1.getType() + "-1\n-----------------------");
        t1.showList();
        System.out.println("-----------------------\nUpdated database of " + t2.getType() + "-2\n-----------------------");
        t2.showList();

        System.out.println("-----------------------\nTrying to remove the same item again...\n-----------------------");
        ds.remove(tempObj);

        System.out.println("-----------------------\nInfo of current object in "+ t2.getType() + "-2\n-----------------------");
        ObjectBase curr = t2.currentObject();
        curr.info();

        String type = "";
        if (t1.getType().equals("Player"))
            type = "video";
        else
            type = "text";
        System.out.println("-----------------------\nJumping to next " + type + " object...\n-----------------------");
        t2.next(type);
        curr = t2.currentObject();
        curr.info();

        if (t1.getType().equals("Player"))
            type = "audio";
        else
            type = "image";
        System.out.println("-----------------------\nTrying to jump to next " + type + " object...\n-----------------------");
        t2.next(type);

        if (t1.getType().equals("Player"))
            type = "audio";
        else
            type = "image";
        System.out.println("-----------------------\nJumping to previous " + type + " object...\n-----------------------");
        t2.previous(type);
        curr = t2.currentObject();
        curr.info();

        System.out.println("-----------------------\nUnregistering " + t1.getType() +"-1 from database...");
        ds.unregister(t1);

        System.out.println("-----------------------\nAdding a new object to database...");
        ObjectBase newObj;
        if (t1.getType().equals("Player"))
            newObj = new Audio("NEW", "NEW", 500);
        else
            newObj = new Image("NEW", "NEW", 150, 350);
        ds.add(newObj);

        System.out.println("-----------------------\nUpdated database of " + t2.getType() + "-2\n-----------------------");
        t2.showList();

        System.out.println("-----------------------\nChecking if the unregistered\n\tobserver got the update...");
        System.out.println("-----------------------\nUpdated database of " + t1.getType() + "-1\n-----------------------");
        t1.showList();
    }
}