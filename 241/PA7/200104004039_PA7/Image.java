public class Image extends ObjectBase implements Visual, NonPlayable {
    public Image() { super(); }

    public Image(String name, String info, int height, int width) {
        super(name, info, "image");
        this.height = height;
        this.width = width;
    }

    public void info() {
        super.info();
        System.out.println("Height: " + this.height);
        System.out.println("Width: " + this.width);
    }

    public int getHeight() { return height; }

    public void setHeight(int height) { this.height = height; }

    public int getWidth() { return width; }

    public void setWidth(int width) { this.width = width; }

    private int height = 0; // pixels
    private int width = 0; // pixels
}
