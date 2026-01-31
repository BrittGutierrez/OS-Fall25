import java.time.Instant;

public class Consumer implements Runnable {

    private final Buffer buffer; // shared buffer
    private final int itemsToConsume;   
    private final int id;               
    private int numOfConsonants;  

    public Consumer(Buffer buffer, int itemsToConsume, int id) {
        this.buffer = buffer;
        this.itemsToConsume = itemsToConsume;
        this.id = id;
        this.numOfConsonants = 0; 
    }

    //check if letter is consonant
    private static boolean isConsonant(char c) {
        return "AEIOU".indexOf(c) == -1;  // vowels = NOT consonants
    }

    private static String nowTimestamp() {
        return Coordinator.getTime();
    }

    public int getNumOfConsumedConsonants() {
        return numOfConsonants;
    }

    @Override
    public void run() {
        // loop only to itemsToConsume
        for (int i = 0; i < itemsToConsume; i++) {
            try {
                //try to take letter buffer
                Buffer.Slot slot = buffer.remove(id);

                char ch = slot.value;   // char taken
                int index = slot.index; // index in circular buffer

                // consanant? tally it!
                if (isConsonant(ch)) {
                    numOfConsonants++;
                }

                // grab timestamp
                String ts = nowTimestamp();

                //its all about format babyy!
                System.out.printf(
                    "[Consumer %d]:  consumed %2c at index %2d at time %s%n",
                    id, ch, index, ts
                );

                System.out.flush(); 

            } catch (InterruptedException e) {
                //If the thread gets interrupted, stop cleanly and elegantly :)
                Thread.currentThread().interrupt();
                break;
            }
        }
    }
}
