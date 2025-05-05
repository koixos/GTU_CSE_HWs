public class Video extends ObjectBase implements Visual, Playable {
    public Video() { super(); }
    public Video(String name, String info, int duration) {
        super(name, info, "video");
        this.duration = duration;
    }

    public void info() {
        super.info();
        System.out.println("Duration: " + this.duration);
    }

    public int getDuration() { return duration; }

    public void setDuration(int duration) { this.duration = duration; }

    private int duration = 0; // seconds
}
