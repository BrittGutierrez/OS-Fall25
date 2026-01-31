public class Buffer {

    // helper -> return letter and index
    public static class Slot {
        public final char value;
        public final int index;

        public Slot(char value, int index) {
            this.value = value;
            this.index = index;
        }
    }

    //circular buffer 
    private final char[] data;
    private int head;   // next spot consumer will read from
    private int tail;   // next spot producer will write to
    private int count;  // how many items are currently stored

    public Buffer(int size) {
        data = new char[size];
        head = 0;
        tail = 0;
        count = 0;
    }

    // Producer calls this to put one letter into the buffer.
    public synchronized int insert(char c, int producerId) throws InterruptedException {
        // If buffer is full, can't put anything in, must wait
        while (count == data.length) {
            wait();
        }

        //drop the char in tail position
        data[tail] = c;
        int indexInserted = tail;

        //move tail forward in a circle
        tail = (tail + 1) % data.length;

        //one more item is now in the buffer
        count++;

        //notify the waiting threads (producers/consumers) something changed
        notifyAll();

        // Send back the index we used so Producer can print it
        return indexInserted;
    }

    //Consumer calls this to take one letter out of the buffer
    public synchronized Slot remove(int consumerId) throws InterruptedException {
        // buffer is empty, nothing to grab yet, must wait
        while (count == 0) {
            wait();
        }

        //take the char from head position
        char value = data[head];
        int indexRemoved = head;

        //move head forward in a circle
        head = (head + 1) % data.length;

        //one less item is in the buffer now
        count--;

        //wake up any waiting threads,state changed
        notifyAll();

        //return both the char and index so Consumer can print
        return new Slot(value, indexRemoved);
    }
}
