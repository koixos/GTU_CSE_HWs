public class Audio extends ObjectBase implements NonVisual, Playable {
    public Audio() { super(); }

    public Audio(String name, String info, int duration) {
        super(name, info, "audio");
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
